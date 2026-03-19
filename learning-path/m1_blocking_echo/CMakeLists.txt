cmake_minimum_required(VERSION 3.10)
project(m1_blocking_echo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(m1_echo
    main.cpp
    BlockingEchoServer.cpp
)
