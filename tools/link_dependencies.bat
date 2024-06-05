@echo off
:checkPrivileges
NET FILE 1>NUL 2>NUL
if '%errorlevel%' == '0' (
    echo Running with Administrator privileges.
    REM Your batch commands here
) else (
    echo This script needs to be run with Administrator privileges.
    echo Please re-run this script as an Administrator.
    exit /B
)

python tools\quicktools_symlink.py

if %errorlevel% neq 0 (
    echo "link quicktools dll failed"
    exit /b 1
) else (
    echo "link quicktools dll success"
)

python tools\opencv_symlink.py

if %errorlevel% neq 0 (
    echo "link opencv dll failed"
    exit /b 1
) else (
    echo "link opencv dll success"
)

python tools\python_symlink.py

if %errorlevel% neq 0 (
    echo "link python dll failed"
    exit /b 1
) else (
    echo "link python dll success"
)