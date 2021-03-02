cmake_minimum_required(VERSION 3.16)
project(Project2)

set(CMAKE_CXX_STANDARD 11)

#add_executable(Project2 benchmarks/external_cal.cpp rpthread.cpp rpthread.h)
#add_executable(Project2 benchmarks/parallel_cal.cpp rpthread.cpp rpthread.h)
add_executable(Project2 benchmarks/vector_multiply.cpp rpthread.cpp rpthread.h)
#add_executable(Project2 main.cpp rpthread.cpp rpthread.h)

target_link_libraries(${PROJECT_NAME} rt)