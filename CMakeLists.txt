cmake_minimum_required(VERSION 3.15)
project(Algorithm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR src)
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp")
foreach(SOURCE_FILE ${SOURCE_FILES})
    get_filename_component(TARGET_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(${TARGET_NAME} ${SOURCE_FILE})
endforeach()
