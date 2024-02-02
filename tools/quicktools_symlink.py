import argparse
from pathlib import Path


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
    
    dir_symlinks(args.link, args.target)
        