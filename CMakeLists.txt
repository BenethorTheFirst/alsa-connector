cmake_minimum_required(VERSION 2.8.12)
project(alsa_connector)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "/home/ben/Qt/5.10.1/gcc_64/lib/cmake")
set (CMAKE_CXX_STANDARD 11)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
add_subdirectory("libaconnect")
include_directories(
  "include"
  "libaconnect/include"
  "/usr/include/alsa")
set(SOURCES
 "src/main.cpp"
 "src/MainWindow.cpp"
 "src/IOConnectorGraph.cpp"
 "src/ConnectionManager.cpp")
add_executable(alsa_connector ${SOURCES})
target_compile_options(alsa_connector PRIVATE -fPIC -Wall -Wextra -Wpedantic)
target_link_libraries(alsa_connector libaconnect Qt5::Widgets)
