# code/CMakeLists.txt

INCLUDE_DIRECTORIES(
  include
  ${CMAKE_SOURCE_DIR}/lib/include
  /usr/local/include
  )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
  
set(cmake_cxx_flags "-o3 -w -Wwrite-strings")
set(cmake_cxx_link_flags "-pg")

ADD_EXECUTABLE(camera_daemon Main.cpp)

TARGET_LINK_LIBRARIES(camera_daemon camera_daemon_lib opencv_core opencv_highgui opencv_imgproc opencv_video)