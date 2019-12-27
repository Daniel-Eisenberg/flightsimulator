cmake_minimum_required(VERSION 3.12)
project(ex3)

set(CMAKE_CXX_STANDARD 11)

add_executable(ex3 main.cpp Tcp_Server.cpp Tcp_Server.h Client_Side.cpp Client_Side.h Command.h Command.cpp Variable.cpp Variable.h ex3.cpp ex3.h)