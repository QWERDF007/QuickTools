# 指定 pybind11 的 python环境
set(PYTHON_EXECUTABLE "E:/Softwares/Anaconda3/envs/test/python.exe")
if(EXISTS ${PYTHON_EXECUTABLE})
  message(STATUS "Found Python executable: ${PYTHON_EXECUTABLE}")
else()
  message(FATAL_ERROR "Python executable not found: ${PYTHON_EXECUTABLE}")
endif()