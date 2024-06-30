# 指定 pybind11 的 python环境
set(PYTHON_EXECUTABLE "H:/Software/Anaconda3/envs/ad/python.exe")
file(EXISTS ${PYTHON_EXECUTABLE} PYTHON_EXISTS)
if(NOT PYTHON_EXISTS)
  message(FATAL_ERROR "Python executable not found: ${PYTHON_EXECUTABLE}")
endif()