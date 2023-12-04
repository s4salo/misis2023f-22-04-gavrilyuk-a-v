add_subdirectory("ImageProcessor")
add_executable(test1 "test1.cpp")
add_executable(main "main.cpp")
target_link_libraries(test1 sabin)
target_link_libraries(main sabin)

