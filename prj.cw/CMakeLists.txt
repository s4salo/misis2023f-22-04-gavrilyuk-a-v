add_executable(two_bin_methods "two_bin_methods.cpp")

target_link_libraries(two_bin_methods PRIVATE ${OpenCV_LIBS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET two_bin_methods PROPERTY CXX_STANDARD 20)
endif()
