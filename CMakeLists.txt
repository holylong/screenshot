# MIT License
# screenshot
# Copyright (c) 2022 hooy

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required (VERSION 3.8)

project(screenshot VERSION 0.0.1 LANGUAGES CXX)
# project("ScreenShot")

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR D:/commonlib/opencv/opencv3416/opencv/build)

# yuv convert
add_subdirectory(3rdparty/libyuv)
# h264 encode
add_subdirectory(3rdparty/openh264)
# add libjpeg
# add_subdirectory(3rdparty/libjpeg-turbo)

#include openh264 dir
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/openh264/codec/api/wels)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/openh264/codec/common/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libyuv/include)

# recurse
# file(GLOB_RECURSE  dxgi_SRC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
# file(GLOB_RECURSE main_SRC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

#source
file(GLOB dxgi_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB main_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

# add executable
add_executable(ScreenSt ${main_SRC} ${dxgi_SRC})

target_include_directories(ScreenSt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#test capture use opencv
# find_package(OpenCV REQUIRED)
find_package(OpenCV)
if(OpenCV_FOUND)
include_directories(${OpenCV_INCLUDE_DIRS})
message(STATUS "==>>${OpenCV_INCLUDE_DIRS}")
message(STATUS "==>>${OpenCV_LIBRARIES}")
add_definitions(-DUSE_OPENCV)
endif()

target_link_libraries(ScreenSt PUBLIC  ${OpenCV_LIBRARIES} dxgi d3d11 shcore)

target_link_libraries(ScreenSt PUBLIC welsencplus welsdecplus)
target_link_libraries(ScreenSt PUBLIC yuv)

IF(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/openh264/Debug/welsencplus.dll" AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/Debug")
add_custom_command(TARGET ScreenSt
                  POST_BUILD
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/openh264/Debug/welsencplus.dll ${CMAKE_CURRENT_BINARY_DIR}/Debug
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/openh264/Debug/welsdecplus.dll ${CMAKE_CURRENT_BINARY_DIR}/Debug)
endif()

IF(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/openh264/Release/welsencplus.dll" AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/Release")
add_custom_command(TARGET ScreenSt
                  POST_BUILD
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/openh264/Release/welsencplus.dll ${CMAKE_CURRENT_BINARY_DIR}/Release
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/openh264/Release/welsdecplus.dll ${CMAKE_CURRENT_BINARY_DIR}/Release)
endif()