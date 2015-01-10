# lib/CMakeLists.txt
INCLUDE_DIRECTORIES(include)
# fetch all cpp files
file(GLOB_RECURSE SRC_FILES *.cpp)

ADD_LIBRARY(camera_daemon_lib  ${SRC_FILES})

