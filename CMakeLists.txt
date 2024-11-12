cmake_minimum_required(VERSION 3.10)

project(rodgers VERSION 3.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(rodgers.exe main.cpp)


target_link_libraries(rodgers.exe PRIVATE curl dpp)
