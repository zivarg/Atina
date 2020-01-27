cmake_minimum_required(VERSION 3.10)
project(atina)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_C_COMPILER /usr/bin/gcc)

set(CMAKE_CXX_COMPILER /usr/bin/g++)

add_executable(atina main.cpp src/inotify.cpp src/inotify.h src/inotifyevent.cpp src/inotifyevent.h src/valueinitializer.h src/valueinitializer.cpp src/atinafd.h src/atinafd.cpp)