cmake_minimum_required(VERSION 3.12)
project(untitled3 C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled3 main.c "Train_ Automation.h" Train_Automation.c)

SET(CMAKE_CXX_FLAGS -pthread)

target_link_libraries(${PROJECT_NAME} pthread)