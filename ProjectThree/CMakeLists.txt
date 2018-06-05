project(ProjectThree CXX)

set(CMAKE_CXX STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++0x")
set(ProjectThree_LZ_HEADERS suffix_array.h file.h window.h bit_stream.h hash_encoder.h)
set(ProjectThree_LZ_SOURCES compress.cpp suffix_array.cpp file.cpp window.cpp bit_stream.cpp hash_encoder.cpp)

set(ProjectThree_EXPAND_HEADERS file.h window.h bit_stream.h)
set(ProjectThree_EXPAND_SOURCES expand.cpp file.cpp window.cpp bit_stream.cpp)

add_executable(LZ ${ProjectThree_LZ_SOURCES} ${ProjectThree_LZ_HEADERS})
add_executable(EXPAND ${ProjectThree_EXPAND_SOURCES} ${ProjectThree_EXPAND_HEADERS})