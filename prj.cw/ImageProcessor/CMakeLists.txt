add_library(sabin ImageProcessor.cpp include/ImageProcessor/ImageProcessor.hpp)
set_property(TARGET sabin PROPERTY CXX_STANDARD 20)
target_include_directories(sabin PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(sabin ${OpenCV_LIBS} nlohmann_json::nlohmann_json)
install(TARGETS sabin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/prj.cw/testpics/ 
DESTINATION pics)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/prj.cw/testconfigs/ 
DESTINATION configs)
