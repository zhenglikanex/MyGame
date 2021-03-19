set(file_list 
Kanex.hpp
Buffer.hpp
BinaryArchive.hpp
BinaryStream.hpp
Access.hpp
Archive.hpp

Internal/Traits.hpp
Internal/Types.hpp

Types/Map.hpp
Types/Pair.hpp
Types/Set.hpp
Types/String.hpp
Types/Vector.hpp

main.cpp
)

foreach(_file ${file_list})
    make_source_group(${_file})
endforeach()

add_library(Kanex
    ${file_list}
)