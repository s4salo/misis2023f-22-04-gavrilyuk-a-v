add_subdirectory("ImageProcessor")
add_executable(prj_test "prj_test.cpp")
target_link_libraries(prj_test sabin)

install(TARGETS prj_test DESTINATION bin)

install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/
		DESTINATION bin
		CONFIGURATIONS Debug
		FILES_MATCHING PATTERN "*.dll*"
)
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/
		CONFIGURATIONS Release
		DESTINATION bin
		FILES_MATCHING PATTERN "*.dll*"
)
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}/
		DESTINATION bin
		CONFIGURATIONS RelWithDebInfo
		FILES_MATCHING PATTERN "*.dll*"
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/prj.cw/testpics/ 
DESTINATION pics)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/prj.cw/testconfigs/ 
DESTINATION configs)

