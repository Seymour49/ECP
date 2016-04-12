project(ecp)

set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall")

add_executable(ecp coloration.cpp graphe.cpp main.cpp)

install(TARGETS ecp RUNTIME DESTINATION bin)
