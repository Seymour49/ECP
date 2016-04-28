project(ecp)

set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall")

add_executable(ecp basictabusearch.cpp voisin.cpp swap.cpp onemove.cpp coloration.cpp graphe.cpp main.cpp)

install(TARGETS ecp RUNTIME DESTINATION bin)
