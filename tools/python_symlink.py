from pathlib import Path
import re
from utils import symlink, find_pattern_in_cmake


if __name__ == '__main__':
    # 编写正则表达式来匹配PYTHON_EXECUTABLE的路径
    pattern = r'set\(PYTHON_EXECUTABLE "([^"]+)"\)'
    python_executable = find_pattern_in_cmake(Path('./cmake/ConfigPython.cmake'), pattern)
    print('find python executable:', python_executable)
    if python_executable:
        python_home = python_executable.parent
        target_dir = Path('./build/bin')
        for dll in python_home.glob('python*.dll'):
            symlink(dll, target_dir / dll.name)
        zlib = python_home / 'zlib.dll'
        symlink(zlib, target_dir / zlib.name)
    symlink('./src/py_module', './build/bin/py_module')
