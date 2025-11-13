cmake_minimum_required ( VERSION 3.1.0...4.0.0 )
project(RayTracingPratice)

set(CMAKE_CXX_STANDARD 17)

set (SOURCE_ONEWEEKEND
src/OneWeekend/main.cpp
src/OneWeekend/vec3.h
src/OneWeekend/color.h
src/OneWeekend/ray.h
src/OneWeekend/hittable.h
src/OneWeekend/sphere.h
src/OneWeekend/hittable_list.h)

add_executable(oneWeekend ${SOURCE_ONEWEEKEND})