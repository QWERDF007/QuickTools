"""QuickTools 运行时依赖链接脚本。

开发阶段用它把 build/QuickTools 中的项目 DLL、dependencies.yaml 中声明的第三方
运行库以及 py_module、assets 等链接到 build/bin，方便直接从构建目录启动程序或运行测试。
默认链接 release 版本。
"""

from __future__ import annotations

import argparse
import locale
import os
import shutil
import subprocess
import sys
from pathlib import Path

if str(Path(__file__).resolve().parent) not in sys.path:
    sys.path.insert(0, str(Path(__file__).resolve().parent))

from dependency_utils import (
    REPO_ROOT,
    PROJECT_NAME,
    build_dll_variant_sets,
    dependency_destinations,
    dependency_matches_config,
    dependency_patterns,
    dll_matches_config,
    expand_dependency_pattern,
    immediate_project_dlls,
    is_project_dll,
    link_dir,
    link_file,
    load_dependencies,
    platform_key,
    resolve_dependency_root,
    resolve_project_path,
    warn,
)


def parse_args() -> argparse.Namespace:
    """解析命令行参数。"""

    parser = argparse.ArgumentParser(description="Link QuickTools runtime dependencies.")
    parser.add_argument("--build-dir", "-BuildDir", default="build", help="CMake build directory.")
    parser.add_argument("--config", "-Config", choices=["release", "debug"], default="release", help="Build configuration.")
    parser.add_argument("--dependencies", default="tools/dependencies.yaml", help="Path to dependencies.yaml.")
    parser.add_argument("--skip-external", action="store_true", help="Skip linking external dependencies.")
    return parser.parse_args()


def link_project_outputs(build_dir: Path, config: str) -> None:
    """把项目模块输出链接到 build/bin。

    Args:
        build_dir: CMake 构建目录。
        config: 目标配置，支持 release 或 debug。
    """

    module_root = build_dir / PROJECT_NAME
    bin_dir = build_dir / "bin"
    if not module_root.is_dir():
        warn(f"skip {PROJECT_NAME} links, missing {module_root}")
        return

    # Windows 下可执行文件是 QuickTools.exe，目录链接到 build/bin/QuickTools 作为 QML 模块目录
    if os.name == "nt":
        link_dir(module_root, bin_dir / PROJECT_NAME)
        qml_root = build_dir / "qml"
        if qml_root.is_dir():
            link_dir(qml_root, bin_dir / "qml")

    # 项目 DLL 从模块输出目录链接到 build/bin；release 模式会过滤成对的 *d.dll
    candidates = immediate_project_dlls(module_root)
    debug_names, release_names = build_dll_variant_sets(candidates)
    for dll in candidates:
        if not dll_matches_config(dll, config, debug_names, release_names):
            continue
        link_file(dll, bin_dir / dll.name)


def link_external_dependencies(build_dir: Path, dependency_file: Path, config: str) -> None:
    """按 dependencies.yaml 把第三方运行库链接到目标目录。"""

    platform = platform_key()
    dependencies = load_dependencies(dependency_file)
    for dep in dependencies:
        if not dependency_matches_config(dep, config):
            continue

        patterns = dependency_patterns(dep, platform)
        if not patterns:
            continue

        destinations = [resolve_project_path(dest) for dest in dependency_destinations(dep)]
        if not destinations:
            continue

        root = resolve_dependency_root(dep, build_dir)
        if root is None:
            warn(f"skip dependency {dep.get('name', '<unnamed>')}, root {dep.get('root')} was not found")
            continue
        if not root.exists():
            warn(f"skip dependency {dep.get('name', '<unnamed>')}, missing root: {root}")
            continue

        matched: list[Path] = []
        for pattern in patterns:
            matches = expand_dependency_pattern(root, pattern, config)
            matched.extend(matches)

        debug_names, release_names = build_dll_variant_sets(matched)
        for runtime in matched:
            if platform == "windows" and is_project_dll(runtime):
                continue
            if platform == "windows" and not dll_matches_config(runtime, config, debug_names, release_names):
                continue
            for destination in destinations:
                link_file(runtime, destination / runtime.name)


def link_py_module(build_dir: Path) -> None:
    """把 src/py_module 目录链接到 build/bin/py_module。"""

    source = REPO_ROOT / "src" / "py_module"
    if not source.is_dir():
        warn(f"skip py_module link, missing {source}")
        return
    link_dir(source, build_dir / "bin" / "py_module")


def link_assets(build_dir: Path) -> None:
    """把 assets 目录链接到 build/bin/assets。"""

    source = REPO_ROOT / "assets"
    if not source.is_dir():
        return
    link_dir(source, build_dir / "bin" / "assets")


def link_docs(build_dir: Path) -> None:
    """把 docs 目录链接到 build/bin/docs。"""

    source = REPO_ROOT / "docs"
    if not source.is_dir():
        return
    link_dir(source, build_dir / "bin" / "docs")


def deploy_qt_runtime(build_dir: Path, dependency_file: Path, config: str) -> None:
    """在 Windows 下使用 windeployqt 部署 Qt 运行时插件与 QML 模块。"""

    if platform_key() != "windows":
        return

    target_exe = build_dir / "bin" / f"{PROJECT_NAME}.exe"
    if not target_exe.is_file():
        return

    dependencies = load_dependencies(dependency_file)
    qt_dep = next((dep for dep in dependencies if dep.get("name") == "qt"), None)
    if not qt_dep:
        return

    qt_root = resolve_dependency_root(qt_dep, build_dir)
    windeployqt = (qt_root / "bin" / "windeployqt.exe") if qt_root else None
    if not windeployqt or not windeployqt.is_file():
        which_path = shutil.which("windeployqt")
        windeployqt = Path(which_path) if which_path else None

    if not windeployqt or not windeployqt.is_file():
        warn("skip windeployqt: executable not found")
        return

    cmd = [
        str(windeployqt),
        f"--{config}",
        "--qmldir",
        str(REPO_ROOT / "src"),
        "--qmldir",
        str(REPO_ROOT / "3rdparty" / "QuickUI" / "src"),
        str(target_exe),
    ]
    try:
        result = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            encoding=locale.getpreferredencoding(False),
            errors="replace",
        )
        if result.returncode == 0:
            print("deploy Qt runtime success via windeployqt")
        else:
            warn(f"windeployqt finished with code {result.returncode}")
    except Exception as exc:
        warn(f"failed to run windeployqt: {exc}")


def main() -> int:
    """执行依赖链接主流程。"""

    args = parse_args()
    build_dir = resolve_project_path(args.build_dir)
    dependency_file = resolve_project_path(args.dependencies)

    link_project_outputs(build_dir, args.config)
    print("link QuickTools dll success")

    link_py_module(build_dir)
    print("link py_module success")

    link_assets(build_dir)
    link_docs(build_dir)

    if args.skip_external:
        print("skip external dependencies")
    elif dependency_file.is_file():
        link_external_dependencies(build_dir, dependency_file, args.config)
        print("link external dependencies success")
        deploy_qt_runtime(build_dir, dependency_file, args.config)
    else:
        warn(f"skip external dependency links, missing {dependency_file}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
