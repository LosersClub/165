project(ProjectThree CXX)

set(CMAKE_CXX STANDARD 11)
set(ProjectThree_LZ_HEADERS )
set(ProjectThree_LZ_SOURCES compress.cpp)

set(ProjectThree_EXPAND_HEADERS )
set(ProjectThree_EXPAND_SOURCES expand.cpp)

add_executable(LZ ${ProjectThree_LZ_SOURCES} ${ProjectThree_LZ_HEADERS})
add_executable(EXPAND ${ProjectThree_EXPAND_SOURCES} ${ProjectThree_EXPAND_HEADERS})