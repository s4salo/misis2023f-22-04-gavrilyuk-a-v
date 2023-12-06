cmake_minimum_required(VERSION 3.20)

project(gavrilyuk-a-v-22-04)

set(CMAKE_CXX_STANDARD 20)
 
find_package(OpenCV REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
option(BUILD_ONLY_LIBRARY "Build only ImageProcessor" off)
if(BUILD_ONLY_LIBRARY)
	add_subdirectory("prj.cw/ImageProcessor") #собрка только библиотеки
else()
	add_subdirectory("prj.codeforces")
	add_subdirectory ("prj.cw") #сборка вместе с тестовыми приложениями
endif()


