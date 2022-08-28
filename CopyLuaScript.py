import os
import shutil

source_path = os.path.abspath(r'.\\Unity\\ARPG\Assets\\StreamingAssets\\Lua')
target_path = os.path.abspath(r'.\\Asset\\Lua')

if not os.path.exists(target_path):
    os.makedirs(target_path)

if os.path.exists(source_path):
    shutil.rmtree(target_path)

shutil.copytree(source_path, target_path)

source_path = os.path.abspath(r'.\\Unity\\ARPG\Assets\\StreamingAssets\\Config')
target_path = os.path.abspath(r'.\\Asset\\Config')

if not os.path.exists(target_path):
    os.makedirs(target_path)

if os.path.exists(source_path):
    shutil.rmtree(target_path)

shutil.copytree(source_path, target_path)