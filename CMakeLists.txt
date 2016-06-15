project(ecp)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall")

add_executable(ecp iteratedtabusearch.cpp binarysearch.cpp basictabusearch.cpp voisin.cpp swap.cpp onemove.cpp coloration.cpp graphe.cpp main.cpp)

install(TARGETS ecp RUNTIME DESTINATION bin)
