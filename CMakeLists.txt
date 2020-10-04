cmake_minimum_required (VERSION 2.6)
project (cg)
add_executable(cg src/main.cpp src/color.cpp src/camera.cpp src/light.cpp src/shape.cpp src/tracer.cpp src/vector.cpp)
target_include_directories(cg PUBLIC include)