cmake_minimum_required(VERSION 3.20)

project(gavrilyuk-a-v-22-04)

set(CMAKE_CXX_STANDARD 17)
 
find_package(OpenCV REQUIRED)

add_subdirectory ("prj.cw")
add_subdirectory("prj.codeforces")

