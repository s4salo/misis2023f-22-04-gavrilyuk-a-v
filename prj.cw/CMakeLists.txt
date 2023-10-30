add_executable (core "core.cpp")

target_link_libraries(core PRIVATE ${OpenCV_LIBS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET core PROPERTY CXX_STANDARD 20)
endif()
