CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)

PROJECT(jpg2video)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_CXX_FLAGS "-o3 -w")
set(CMAKE_CXX_LINK_FLAGS "-pg")

ADD_EXECUTABLE(jpg2video Main.cpp)

TARGET_LINK_LIBRARIES(jpg2video opencv_core opencv_highgui opencv_imgproc opencv_video)