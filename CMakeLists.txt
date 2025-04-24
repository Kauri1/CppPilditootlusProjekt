cmake_minimum_required(VERSION 3.30)
project(PilditootlusProjekt)

set(CMAKE_CXX_STANDARD 23)

set(OpenCV_DIR "C:/opencv/build")

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(PilditootlusProjekt main.cpp)


target_link_libraries( PilditootlusProjekt ${OpenCV_LIBS} )