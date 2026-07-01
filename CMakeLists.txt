cmake_minimum_required(VERSION 3.20.0)
project(IHM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")
set(OpenCV_DIR "C:/Program Files/OpenCV/opencv-4.10.0/build/install")

if (MSVC)
    add_compile_options(/utf-8)
endif()

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${PROJECT_SOURCE_DIR}/include )

FILE (GLOB ALL_SOURCES "${PROJECT_SOURCE_DIR}/include/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp" )
add_executable(IHM ${ALL_SOURCES})

target_link_libraries( IHM ${OpenCV_LIBS} )
