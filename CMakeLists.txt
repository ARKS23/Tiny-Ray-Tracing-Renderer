cmake_minimum_required ( VERSION 3.1.0...4.0.0 )
project(RayTracingPratice)

set(CMAKE_CXX_STANDARD 17)

set (SOURCE_ONEWEEKEND
src/OneWeekend/main.cpp
src/OneWeekend/vec3.h
src/OneWeekend/color.h)

add_executable(oneWeekend ${SOURCE_ONEWEEKEND})