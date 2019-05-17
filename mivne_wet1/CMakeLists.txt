cmake_minimum_required(VERSION 3.8)
project(mivne)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES library1.cpp
        library1.h
        Splay_Tree.h
        LinkedList.h
        Arena.cpp 
        Arena.h
        main1.cpp)
add_executable(mivne ${SOURCE_FILES})