import os

def find_folders_with_cmake(startpath):
    for root, dirs, files in os.walk(startpath):
        if 'CMakeLists.txt' in files:
            print(root)

# 替换为你的起始目录路径
startpath = 'F:\\USTC_CG_HW\\Framework2D'
find_folders_with_cmake(startpath)