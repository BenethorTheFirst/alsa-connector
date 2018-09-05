project(libaconnect)
set (CMAKE_CXX_STANDARD 11)
set(SOURCES
  "src/aconnect.cpp"
)
include_directories(
  "include"
  "/usr/include/alsa")
add_library(libaconnect ${SOURCES})
target_link_libraries(libaconnect asound)
