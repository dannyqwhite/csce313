cmake_minimum_required(VERSION 3.17)
project(MP2_Source)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MP2_Source
      makefile
	dataserver.cpp
mutex.hpp
reqchannel.cpp
mutex_guard.hpp
reqchannel.hpp
simpleclient.cpp)
