cmake_minimum_required(VERSION 3.12)
project(ex3)

set(CMAKE_CXX_STANDARD 11)
set(GCC_ENABLE_WARNINGS "-O1 -Wall -Werror -Wno-unused-result")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_ENABLE_WARNINGS}")

add_executable(ex3 main.cpp Tcp_Server.cpp Tcp_Server.h Client_Side.cpp Client_Side.h Command.h Command.cpp Variable.cpp
        Variable.h Interpreter.h Interpreter.cpp Expression.h MathParser.h Lexer.h Lexer.cpp MathParser.cpp
        Token.cpp Token.h DatabaseManager.cpp DatabaseManager.h Parser.cpp Parser.h)