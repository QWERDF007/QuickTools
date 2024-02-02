import os
import sys
import ctypes

def get_file_path(path, suffix) -> list:
    """ 递归遍历目录获取指定后缀的文件路径

    Args:
        path (str): 待遍历的目录
        suffix (str): 文件后缀

    Returns:
        [str]: 具有指定后缀的文件路径列表
    """
    file_list = []
    for root, dirs, files in os.walk(path):
        for file in files:
            if file.endswith(suffix):
                file_list.append(os.path.join(root, file))  
    return file_list



def is_admin() -> bool:
    """判断是否管理员权限

    Returns:
        bool: True/False
    """
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False
    
def runas_admin():
    """_summary_
    """
    if not is_admin():
        ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, __file__, None, 1)
        
