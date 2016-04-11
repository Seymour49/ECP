project(ecp)

add_executable(ecp graphe.cpp main.cpp)

install(TARGETS ecp RUNTIME DESTINATION bin)
