#将文件归组
function(make_source_group file_name)
	#判断是否是绝对路径
	if(IS_ABSOLUTE ${file_name})
		#提取相对路劲
		file(RELATIVE_PATH rel_path ${CMAKE_CURRENT_SOURCE_DIR} ${file_name})
	else()
		set(rel_path ${file_name})
	endif()
	
	#得到文件的路劲部分
	set(_path "")
	get_filename_component(_path ${rel_path} PATH)

	if("${_path}" STREQUAL "")
		
	else()
		#替换“/” - "\\"
		string(REPLACE "/" "\\" group_path ${_path})
		#分组
		source_group(${group_path} FILES ${file_name})
	endif()
	
endfunction(make_source_group)

#自动添加头文件路径
function(auto_include_dir header_file)
	#判断是否是绝对路径
	if(IS_ABSOLUTE ${header_file})
		#提取相对路劲
		file(RELATIVE_PATH rel_path ${CMAKE_CURRENT_SOURCE_DIR} ${header_file})
	else()
		set(rel_path ${header_file})
	endif()
	
	#得到文件的路劲部分
	get_filename_component(_path ${rel_path} PATH)
	include_directories(${_path})
endfunction(auto_include_dir)

function(add_dir_header_and_src path)
	#遍历添加文件
	FILE(GLOB_RECURSE _SRC_LIST ${path}/*.c ${path}/*.cpp)
	FILE(GLOB_RECURSE _HEADER_LIST ${path}/*.h ${path}/*.hpp)
	
	foreach(_file ${_SRC_LIST})
		list(APPEND LOCAL_SRC_LIST ${_file})
		make_source_group(${_file})
	endforeach()
	set(SRC_LIST ${SRC_LIST} ${LOCAL_SRC_LIST} PARENT_SCOPE)

	foreach(_file ${_HEADER_LIST})
		list(APPEND LOCAL_HEADER_LIST ${_file} )
		make_source_group(${_file})
		auto_include_dir(${_file})
	endforeach()
	set(HEADER_LIST ${HEADER_LIST} ${LOCAL_HEADER_LIST} PARENT_SCOPE)
endfunction(add_dir_header_and_src)


