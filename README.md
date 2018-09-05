## ALSA Connector
This is a re-write of the ALSA connection tool available in most GNU/Linux distributions, and it uses
the Qt framework, and CMake to build the application. I have developed this as a learning exercise for 
writing CMake scripts, developing an API and for building Qt applications with KDevelop.

To build this application, you will need a recent copy of CMake, the Qt SDK. The complete build process
varies amongst systems, but on Unix systems you could simply create a build directory, then run
cmake with '..', and then run 'make -j[num_cores + 1]'.
