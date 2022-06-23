project(engine C)
cmake_minimum_required(VERSION 2.8)

message(STATUS "install dirctory:" ${INSTALL_DIR})

set(CMAKE_FIND_ROOT_PATH ${INSTALL_DIR})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#OUTPUT
set(EXECUTABLE_OUTPUT_PATH $ENV{INSTALL_DIR})

find_package(PkgConfig)

#libwebp
pkg_check_modules(LIBWEBP REQUIRED libwebp)
if (LIBWEBP_FOUND)
    message(STATUS "libwebp library dirctory:" ${LIBWEBP_INCLUDE_DIRS})
    message(STATUS "libwebp include dirctory:" ${LIBWEBP_LIBRARY_DIRS})
    message(STATUS "libwebp library:" ${LIBWEBP_LIBRARIES})
endif ()

#ffmpeg
pkg_check_modules(FFMPEG REQUIRED libavcodec
                                  libavdevice
                                  libavfilter
                                  libavformat
                                  libavutil
                                  libswscale)
if (FFMPEG_FOUND)
    message(STATUS "ffmpeg library dirctory:" ${FFMPEG_INCLUDE_DIRS})
    message(STATUS "ffmpeg include dirctory:" ${FFMPEG_LIBRARY_DIRS})
    message(STATUS "ffmpeg library:" ${FFMPEG_LIBRARIES})
endif ()

#define
#add_definitions(-DCURL_HAVE_)

# target include
include_directories(${LIBWEBP_INCLUDE_DIRS} ${FFMPEG_INCLUDE_DIRS})
link_directories(${LIBWEBP_LIBRARY_DIRS} ${FFMPEG_LIBRARY_DIRS})


#add sub
#add_subdirectory(easy_sample)
set(SRCS src/ffmpegprobe.c
         src/main.c
         src/tool/convertRgbaToWebp.c)

#product
add_executable(ffmpeg_probe ${SRCS})

#include
target_include_directories(ffmpeg_probe PUBLIC include)

#link
TARGET_LINK_LIBRARIES(ffmpeg_probe  webp
                                    avcodec
                                    avdevice
                                    avfilter
                                    avformat
                                    avutil
                                    swscale)