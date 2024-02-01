import argparse
import platform
from pathlib import Path
import os



def opencv_symlinks(opencv_home, link_dir, config):
    dll_stems = [
        'opencv_world480'
    ]
    
    link_dir = Path(link_dir)
    target_dir = Path(opencv_home) / 'bin'

    if platform.system() == 'Windows':
        prefix = ''
        if config == 'debug':
            suffix = 'd.dll'
        else:
            suffix = '.dll'
    else:
        prefix = 'lib'
        suffix = '.so'

    for stem in dll_stems:
        target = target_dir / (prefix + stem + suffix)
        link = link_dir / target.name
        target = target.absolute()
        link = link.absolute()
        if link.exists():
            link.unlink()
        if not target.exists():
            print(f'failed to create symlink, not found {target}')
            continue
        else:
            print(f'create symlink {link} to {target}')
        link.symlink_to(target)
    
    

if __name__ == '__main__':
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('opencv_home', type=str, metavar='DIR', help='directory to opencv home')
    parser.add_argument('--link', type=str, default='./build/bin', metavar='LINK', help='directory to runtime output')
    parser.add_argument('-cfg', '--config', type=str, default='debug', choices=['release', 'debug'], help='windows config, debug or release')
    args = parser.parse_args()
    
    opencv_symlinks(args.opencv_home, args.link, args.config)
        