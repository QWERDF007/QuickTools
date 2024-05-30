
from pathlib import Path
from utils import symlink

def sub_dir_symlinks(link_dir, target_dir):
    link_dir = Path(link_dir)
    if not link_dir.exists():
        link_dir.mkdir(parents=True)
    target_dir = Path(target_dir)
    subdirs_to_link = [subdir.stem for subdir in target_dir.iterdir() if subdir.is_dir()]

    for stem in subdirs_to_link:
        link = link_dir / stem
        target = target_dir / stem
        symlink(target, link)

def dir_symlinks(link_dir, target_dir):
    link_dir = Path(link_dir)
    target_dir = Path(target_dir)
    symlink(target_dir, link_dir)

if __name__ == '__main__':
    dir_symlinks('./build/bin/QuickTools', './build/QuickTools')
    dir_symlinks('./build/bin/docs', './docs')
        