cmake_minimum_required(VERSION 3.10)
project(GameProject)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB SOURCES "src/*.cpp", "src/spells/*.cpp", "src/managment/*.cpp", "src/characters/*.cpp", "src/field/*.cpp")

add_executable(GameProject ${SOURCES})