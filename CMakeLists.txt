# CMake integration by Christian Frisson
cmake_minimum_required(VERSION 2.8)
PROJECT(shotdetect)

SET(CMAKE_C_COMPILER "/usr/bin/clang")
SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


# Get git version information for automatic embedding in version string:

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
# Take the first 12 characters of the SHA1 as short identifier:
STRING(SUBSTRING ${GIT_SHA1} 0 11 GIT_SHA1_SHORT)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version.cc.in" "${CMAKE_CURRENT_BINARY_DIR}/version.cc" @ONLY)
list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/version.cc" src/version.h)

# Miscellaneous compilation options:

OPTION(USE_WXWIDGETS "Compile GUI app with wxWidgets, otherwise commandline app" ON)
OPTION(USE_POSTGRESQL "Compile with PostgreSQL support" OFF)

# Dependency: pkg-config (required if cross-compiling with MXE)

IF ( MINGW AND CMAKE_TOOLCHAIN_FILE)
	FIND_PACKAGE (PkgConfig)
	IF(NOT PKG_CONFIG_FOUND)
		MESSAGE(FATAL_ERROR "pkgconfig required for cross-compiling with MXE for windows")
	ENDIF()
	SET(PKG_CONFIG_EXECUTABLE ${PKG_CONFIG_EXECUTABLE} CACHE STRING "pkg-config")
ENDIF()

# Dependency: FFmpeg (required)

FIND_PACKAGE( FFmpeg )
IF ( MINGW AND CMAKE_TOOLCHAIN_FILE)
	PKG_CHECK_MODULES (FFMPEG_PKG REQUIRED libavcodec libavfilter libavutil libavdevice libavformat libavformat libswscale)
	IF (FFMPEG_PKG_FOUND)
		MESSAGE( "FFmpeg cflags found through pkg-config: ${FFMPEG_PKG_CFLAGS}" )
		MESSAGE( "FFmpeg ldflags found through pkg-config: ${FFMPEG_PKG_LDFLAGS}" )
		STRING(REGEX REPLACE ";" " " FFMPEG_PKG_CFLAGS "${FFMPEG_PKG_CFLAGS}")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FFMPEG_PKG_CFLAGS}")
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FFMPEG_PKG_CFLAGS}")
		SET(FFMPEG_LIBRARIES ${FFMPEG_LIBRARY} ${FFMPEG_PKG_LDFLAGS})
		MESSAGE("FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES}")
	ELSE (FFMPEG_PKG_FOUND)
		MESSAGE( FATAL_ERROR "FFmpeg cflags/ldflags not found through pkg-config." )
	ENDIF (FFMPEG_PKG_FOUND)
ENDIF()
IF(FFMPEG_LIBAVFORMAT_FOUND)
	MESSAGE(STATUS "avformat library found: ${FFMPEG_LIBAVFORMAT_LIBRARIES}")
ELSE()
	MESSAGE(STATUS "avformat library not found.")
ENDIF()
IF(FFMPEG_LIBAVCODEC_FOUND)
	MESSAGE(STATUS "avcodec library found: ${FFMPEG_LIBAVCODEC_LIBRARIES}")
ELSE()
	MESSAGE(STATUS "avcodec library not found.")
ENDIF()
IF(FFMPEG_LIBAVUTIL_FOUND)
	MESSAGE(STATUS "avutil library found: ${FFMPEG_LIBAVUTIL_LIBRARIES}")
ELSE()
	MESSAGE(STATUS "avutil library not found.")
ENDIF()
IF(FFMPEG_LIBSWSCALE_FOUND)
	MESSAGE(STATUS "swscale library found: ${FFMPEG_LIBSWSCALE_LIBRARIES}")
ELSE()
	MESSAGE(STATUS "swscale library not found.")
ENDIF()
IF(FFMPEG_FOUND)
	INCLUDE_DIRECTORIES(${FFMPEG_INCLUDE_DIR} ${FFMPEG_INCLUDE_DIRS})
	MESSAGE("Found all FFmpeg libraries in ${FFMPEG_LIBRARIES}.")
ELSE()
	MESSAGE(FATAL_ERROR "Some FFmpeg libraries are missing.")
ENDIF()

# Dependency: GD (required)

FIND_PACKAGE(GD REQUIRED)
IF ( MINGW AND CMAKE_TOOLCHAIN_FILE)
	EXEC_PROGRAM(${CMAKE_FIND_ROOT_PATH}/bin/gdlib-config ARGS "--cflags" OUTPUT_VARIABLE GD_PKG_CFLAGS) 
	EXEC_PROGRAM(${CMAKE_FIND_ROOT_PATH}/bin/gdlib-config ARGS "--libs" OUTPUT_VARIABLE GD_PKG_LDFLAGS)
	MESSAGE( "gd cflags found through gdlib-config: ${GD_PKG_CFLAGS}" )
	MESSAGE( "gd ldflags found through gdlib-config: ${GD_PKG_LDFLAGS}" )
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GD_PKG_CFLAGS}")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GD_PKG_CFLAGS}")
	SET(GD_LIBRARIES ${GD_LIBRARY} ${GD_PKG_LDFLAGS})
	MESSAGE("GD_LIBRARIES ${GD_LIBRARIES}")
ENDIF()
IF(GD_FOUND)
	INCLUDE_DIRECTORIES(${GD_INCLUDE_DIR})
	MESSAGE("Found libgd: ${GD_LIBRARIES} in ${GD_INCLUDE_DIR}")
ELSE()
	MESSAGE(FATAL_ERROR "Couldn't find libgd")
ENDIF()

# Dependency: libxml2 (required)

FIND_PACKAGE(LibXml2 2.7 REQUIRED)
IF ( MINGW AND CMAKE_TOOLCHAIN_FILE)
	PKG_CHECK_MODULES (LIBXML2_PKG REQUIRED libxml-2.0)
	IF (LIBXML2_PKG_FOUND)
		MESSAGE( "LibXml2 cflags found through pkg-config: ${LIBXML2_PKG_CFLAGS}" )
		MESSAGE( "LibXml2 ldflags found through pkg-config: ${LIBXML2_PKG_LDFLAGS}" )
		STRING(REGEX REPLACE ";" " " LIBXML2_PKG_CFLAGS "${LIBXML2_PKG_CFLAGS}")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LIBXML2_PKG_CFLAGS}")
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${LIBXML2_PKG_CFLAGS}")
		SET(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY} ${LIBXML2_PKG_LDFLAGS})
		MESSAGE("LIBXML2_LIBRARIES ${LIBXML2_LIBRARIES}")
	ELSE (LIBXML2_PKG_FOUND)
		MESSAGE( FATAL_ERROR "LibXml2 cflags/ldflags not found through pkg-config." )
	ENDIF (LIBXML2_PKG_FOUND)
ENDIF()
IF(LIBXML2_FOUND)
	MESSAGE("Found libxml2: ${LIBXML2_LIBRARIES} in ${LIBXML2_INCLUDE_DIR}")
	INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})
ELSE()
	MESSAGE(FATAL_ERROR "Couldn't find libxml2")
ENDIF()

# Dependency: libxslt (required)

FIND_PACKAGE(LibXslt REQUIRED)
IF(LIBXSLT_FOUND)
	MESSAGE("Found libxslt: ${LIBXSLT_LIBRARIES} in ${LIBXSLT_INCLUDE_DIR}")
	INCLUDE_DIRECTORIES(${LIBXSLT_INCLUDE_DIR})
ELSE()
	MESSAGE(FATAL_ERROR "Couldn't find libxslt")
ENDIF()

# Dependency: wxWidgets (optional)
IF(USE_WXWIDGETS)
	FIND_PACKAGE(wxWidgets)# COMPONENTS core base)
	IF(wxWidgets_FOUND)
		MESSAGE("Found wxWidgets: ${wxWidgets_LIBRARIES} in ${wxWidgets_INCLUDE_DIRS}")
		INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})
		ADD_DEFINITIONS(-DWXWIDGETS)
		INCLUDE("${wxWidgets_USE_FILE}")
	ELSE()
		MESSAGE(FATAL_ERROR "Couldn't find wxWidgets. Set USE_WXWIDGETS to OFF or install wxWidgets.")
	ENDIF()
ENDIF()

# Dependency: PostgreSQL (optional)
IF(USE_POSTGRESQL)
	FIND_PACKAGE(PostgreSQL)
	IF(PostgreSQL_FOUND)
		MESSAGE("Found PostgreSQL: ${PostgreSQL_LIBRARIES} in ${PostgreSQL_INCLUDE_DIR}")
		INCLUDE_DIRECTORIES(${PostgreSQL_INCLUDE_DIR})
	ELSE()
		MESSAGE(FATAL_ERROR "Couldn't find PostgreSQL. Set USE_POSTGRESQL to OFF or install PostgreSQL.")
	ENDIF()
ENDIF()

# shotdetect

SET(TARGET_NAME "shotdetect")
INCLUDE_DIRECTORIES(.)

# shotdetect library

SET(${TARGET_NAME}_LIBRARY_SRCS src/film.cc src/graph.cc src/image.cc src/shot.cc src/xml.cc)
SET(${TARGET_NAME}_LIBRARY_HDRS  src/film.h src/graph.h src/image.h src/shot.h src/xml.h)
IF(USE_POSTGRESQL)
	SET(${TARGET_NAME}_LIBRARY_SRCS ${${TARGET_NAME}_LIBRARY_SRCS} src/bdd.cc)
	SET(${TARGET_NAME}_LIBRARY_HDRS ${${TARGET_NAME}_LIBRARY_HDRS} src/bdd.h)
ENDIF()
ADD_LIBRARY(${TARGET_NAME} ${${TARGET_NAME}_LIBRARY_SRCS} ${${TARGET_NAME}_LIBRARY_HDRS})
TARGET_LINK_LIBRARIES(${TARGET_NAME} ${FFMPEG_LIBRARIES} ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES} ${GD_LIBRARIES})
IF(USE_WXWIDGETS AND wxWidgets_FOUND)
	TARGET_LINK_LIBRARIES(${TARGET_NAME} ${wxWidgets_LIBRARIES})
ENDIF()
IF(USE_POSTGRESQL AND PostgreSQL_FOUND)
	TARGET_LINK_LIBRARIES(${TARGET_NAME} ${PostgreSQL_LIBRARY})
ENDIF()

# shotdetect application: gui or commandline

IF(APPLE)
	SET(APP_TYPE "MACOSX_BUNDLE")
ELSEIF(WIN32)
	SET(APP_TYPE "WIN32")
ENDIF()

IF(USE_WXWIDGETS AND wxWidgets_FOUND)
    LIST(APPEND ${TARGET_NAME}_GUI_SRCS src/main.cc src/ui/dialog_help.cc src/ui/dialog_shotdetect.cc src/ui/process_video_thread.cc)
    LIST(APPEND ${TARGET_NAME}_GUI_HDRS src/ui/dialog_help.h src/ui/dialog_shotdetect.h src/ui/process_video_thread.h)
    ADD_EXECUTABLE(${TARGET_NAME}-gui ${APP_TYPE} ${SOURCES} ${${TARGET_NAME}_GUI_SRCS} ${${TARGET_NAME}_GUI_HDRS})
	TARGET_LINK_LIBRARIES(${TARGET_NAME}-gui ${TARGET_NAME})
    # Make this target optional to install:
    SET(TARGETS_TO_INSTALL ${TARGET_NAME}-gui)
ELSE()
    LIST(APPEND ${TARGET_NAME}_COMMANDLINE_SRCS src/commandline.cc)
    ADD_EXECUTABLE(${TARGET_NAME}-cmd ${SOURCES} ${${TARGET_NAME}_COMMANDLINE_SRCS})
	TARGET_LINK_LIBRARIES(${TARGET_NAME}-cmd ${TARGET_NAME})
    # Make this target optional to install:
    SET(TARGETS_TO_INSTALL ${TARGET_NAME}-cmd)
ENDIF()


# Routines for installing shotdetect.
# Taken from official documentation (http://www.cmake.org/cmake/help/cmake2.6docs.html#command:install)
install( 
    TARGETS ${TARGETS_TO_INSTALL}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)
