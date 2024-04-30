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
    dir_symlinks('./build/bin/QuickTools', './build/QuickTools')
    dir_symlinks('./build/bin/docs', './docs')
        