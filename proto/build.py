#start F:/demo/proto/protoc/protoc.exe --proto_path=F:/demo/proto/message/  *.proto

#usr/bin/env python
#-*- coding: utf-8 -*-

import os
import shutil

def main():				
	cur_dir = os.getcwd()
	#os.chdir('F:\demo\proto')

	exe_path = cur_dir + '\protoc\protoc.exe'
	proto_path = " --proto_path={}\message ".format(cur_dir)
	cpp_path = "{}/../Framework/Proto".format(cur_dir)
	csharp_path = "{}/../Unity/ARPG/Assets/Resources/Scripts/proto".format(cur_dir)
	cpp_out = " --cpp_out={} ".format(cpp_path)
	csharp_out = " --csharp_out={} ".format(csharp_path)
	if not os.path.exists(cpp_path):
		os.makedirs(cpp_path)
	if os.path.exists(csharp_path):
		shutil.rmtree(csharp_path)
		os.makedirs(csharp_path)
	else:
		os.makedirs(csharp_path)
	
	dir = "{}\message".format(cur_dir)
	for dirpath, dirnames, filenames in os.walk(dir):
		for file in filenames:
			os.system(exe_path + proto_path + cpp_out + cur_dir + '/message/' + file)
			os.system(exe_path + proto_path + csharp_out + cur_dir +'/message/' + file)
			
	os.system("pause")
			
if '__main__' == __name__:
    main()
	