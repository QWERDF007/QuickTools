import argparse
import platform
from pathlib import Path
from utils import symlink, find_pattern_in_cmake



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
        symlink(target, link)
    
    

if __name__ == '__main__':
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('-cfg', '--config', type=str, default='debug', choices=['release', 'debug'], help='windows config, debug or release')
    args = parser.parse_args()
    # 编写正则表达式来匹配 OPENCV_HOME 的路径
    pattern = r'set\(OpenCV_HOME "([^"]+)"\)'
    opencv_home = find_pattern_in_cmake(Path('./cmake/ConfigOpenCV.cmake'), pattern)
    print('find opencv home:', opencv_home)
    if opencv_home:
        opencv_symlinks(opencv_home, './build/bin', args.config)
        