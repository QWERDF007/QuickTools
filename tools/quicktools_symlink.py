import argparse
import platform
from pathlib import Path
import os

def get_file_list(dir, suffix):
    # 递归遍历目录获取指定后缀的文件
    files = []
    for root, dirs, filenames in os.walk(dir):
        for filename in filenames:
            if filename.endswith(suffix):
                files.append(os.path.join(root, filename))
    return files
    
def dll_symlinks(link_dir : Path, target_dir : Path, config):

    link_dir = Path(link_dir)
    target_dir = Path(target_dir)
    
    if platform.system() == 'Windows':
        if config == 'debug':
            link_dir = link_dir / 'debug'
            suffix = 'd.dll'
        else:
            link_dir = link_dir / 'release'
            suffix = '.dll'
        target_dir  = target_dir / '3rdparty' / 'cpp_module' / 'windows'
    else:
        link_dir = link_dir / 'lib'
        target_dir  = target_dir / '3rdparty' / 'cpp_module' / 'linux'
        suffix = '.so'

    target_library_files = get_file_list(target_dir, suffix)
    for target in target_library_files:
        target = Path(target)
        link = link_dir / target.name
        if link.exists():
            link.unlink()
        if not target.exists():
            print(f'failed to create symlink, not found {target}')
            continue
        else:
            print(f'create symlink {link} to {target}')
        link.symlink_to(target)

def dir_symlinks(link_dir, target_dir):
    link_dir = Path(link_dir)
    if not link_dir.exists():
        link_dir.mkdir(parents=True)
    target_dir = Path(target_dir)
    symlink_dirs = [subdir.stem for subdir in target_dir.iterdir() if subdir.is_dir()]

    for stem in symlink_dirs:
        link = link_dir / stem
        target = target_dir / stem
        link = link.absolute()
        target = target.absolute()
        if link.exists():
            link.unlink()
        if not target.exists():
            print(f'failed to create symlink, not found {target}')
            continue
        else:
            print(f'create symlink {link} to {target}')
        link.symlink_to(target, target_is_directory=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('--target', type=str, default='./build/QuickTools', metavar='TARGET', help='directory library output')
    parser.add_argument('--link', type=str, default='./build/bin/QuickTools', metavar='LINK', help='directory to runtime output')
    parser.add_argument('-cfg', '--config', type=str, default='debug', choices=['release', 'debug'], help='windows config, debug or release')
    args = parser.parse_args()

    # dll_symlinks(args.link, args.target, args.config)
    dir_symlinks(args.link, args.target)
        