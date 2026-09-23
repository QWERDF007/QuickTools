# 工具脚本说明

本目录存放 QuickTools 的运行时依赖链接脚本。旧的 `.bat` 和单项 `*_symlink.py` 脚本已移除，统一通过 `link_dependencies.py` 执行。

## `link_dependencies.py`

`link_dependencies.py` 用于开发阶段准备 build 目录运行环境。它会把项目模块 DLL、第三方运行库（OpenCV、CUDA、SQLite、Python 等）以及 `py_module` 目录链接到 `build/bin`，方便直接从构建目录运行 `QuickTools.exe` 或测试用例。

它会执行以下步骤：
- 将 `build/QuickTools` 目录链接到 `build/bin/QuickTools`（用于 QML 模块解析）。
- 将 `build/QuickTools/<module>/QuickTools_*.dll` 链接到 `build/bin`。
- 将 `src/py_module` 链接到 `build/bin/py_module`。
- 将 `assets` 和 `docs` 目录链接到 `build/bin`（如果存在）。
- 按 `tools/dependencies.yaml` 把第三方运行库链接到配置的目标目录（默认为 `build/bin`）。
- 优先创建符号链接，权限不足时自动回退为硬链接（文件）或 Junction（目录），再次失败回退为直接复制。

### 常用命令

```powershell
python tools\link_dependencies.py
```

指定 Python 3.12 解释器运行：

```powershell
& "D:\Software\anaconda3\envs\py312\python.exe" tools\link_dependencies.py
```

### 常用参数

| 参数 | 说明 |
|------|------|
| `--build-dir` / `-BuildDir` | CMake 构建目录，默认 `build`。 |
| `--config` / `-Config` | 目标配置，支持 `release` 或 `debug`，默认 `release`。 |
| `--dependencies` | 依赖清单路径，默认 `tools/dependencies.yaml`。 |
| `--skip-external` | 跳过 `dependencies.yaml` 中声明的第三方依赖，仅链接项目产物。 |
