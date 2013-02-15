# Test executables
add_executable(xdgmime_test
		xdgmime_test.c)
target_link_libraries(xdgmime_test xdgmime)
#INSTALL(TARGETS xdgmime_test DESTINATION bin)

