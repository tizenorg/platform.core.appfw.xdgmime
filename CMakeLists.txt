CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

PROJECT(xdgmime C)
SET(VERSION_MAJOR 1)
SET(VERSION "${VERSION_MAJOR}.1.0")

### Global setting ###

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR ${LIB_INSTALL_DIR})
SET(INCLUDEDIR "\${prefix}/include")

# Build type : Release
IF("${CMAKE_BUILD_TYPE}" STREQUAL "")
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF()
MESSAGE("Build type: ${CMAKE_BUILD_TYPE}")


# Set required packages
INCLUDE(FindPkgConfig)

#pkg_check_modules(libpkgs REQUIRED )
#FOREACH(flag ${libpkgs_CFLAGS})
#	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
#ENDFOREACH(flag)

# Compiler flags
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -Wl,-zdefs" )
SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -fvisibility=hidden")
SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -Wall -Wmissing-prototypes -Wno-sign-compare -g -DHAVE_MMAP")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")
SET(CMAKE_C_FLAGS_RELEASE "-O2")

SET(CMAKE_SKIP_BUILD_RPATH true)

# Get uname value to set 'TARGET' definition 
# TODO: Is this needed?
FIND_PROGRAM(UNAME NAMES uname)
EXEC_PROGRAM("${UNAME}" ARGS "-m" OUTPUT_VARIABLE "ARCH")
IF("${ARCH}" STREQUAL "arm")
	ADD_DEFINITIONS("-DTARGET")
	MESSAGE("add -DTARGET")
ENDIF("${ARCH}" STREQUAL "arm")

ADD_DEFINITIONS("-DPREFIX=\"${CMAKE_INSTALL_PREFIX}\"")

# Linker flags
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed")

### Build ###

# xdgmime
add_library(xdgmime SHARED
	xdgmime/src/xdgmime.c
	xdgmime/src/xdgmimeglob.c
	xdgmime/src/xdgmimeint.c
	xdgmime/src/xdgmimemagic.c
	xdgmime/src/xdgmimealias.c
	xdgmime/src/xdgmimeparent.c
	xdgmime/src/xdgmimecache.c
	xdgmime/src/xdgmimeicon.c
	xdgmime/src/xdgmimeglobs2.c
	)
target_link_libraries(xdgmime ${libpkgs_LDFLAGS})
SET_TARGET_PROPERTIES(xdgmime PROPERTIES SOVERSION ${VERSION_MAJOR})
SET_TARGET_PROPERTIES(xdgmime PROPERTIES VERSION ${VERSION})

# pkgconfig file
CONFIGURE_FILE(xdgmime.pc.in xdgmime.pc @ONLY)

### Install ###
INSTALL(TARGETS xdgmime DESTINATION ${LIB_INSTALL_DIR} COMPONENT RuntimeLibraries)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/xdgmime/src/xdgmime.h DESTINATION include)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/xdgmime.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)

# test
add_subdirectory(test)

