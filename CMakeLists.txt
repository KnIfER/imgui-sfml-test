cmake_minimum_required(VERSION 3.8)
project(imgui-vlc)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_PATH}:${CMAKE_MODULE_PATH}" C:/yin/SFML-2.4.2/cmake/Modules)
set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_COLOR_MAKEFILE ON)

add_subdirectory(external)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBVLC libvlc)

include_directories(
  ${IMGUI_SFML_INCLUDE_DIRS}
  ${LIBVLC_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
  imgui-sfml
  ${LIBVLC_LIBRARIES}
)
