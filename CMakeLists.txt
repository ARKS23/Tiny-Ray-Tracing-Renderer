cmake_minimum_required ( VERSION 3.1.0...4.0.0 )
project(RayTracingPratice)

set(CMAKE_CXX_STANDARD 17)

set (SOURCE_ONEWEEKEND src/oneWeekend/main.cpp)
add_executable(oneWeekendTarget ${SOURCE_ONEWEEKEND})
target_include_directories(oneWeekendTarget PUBLIC 
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/oneWeekend
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/common)