cmake_minimum_required(VERSION 3.14)

project(
  "USTC_CG_2025_Framework2D"
  VERSION 1.0
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

message(STATUS "Started CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...\n")
#set(BUILD_SHARED_LIBS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# set()定义的变量默认是目录作用域
# add_subdirectory()进入子目录时，父目录的变量会自动继承到子目录
set(FRAMEWORK2D_DIR "${PROJECT_SOURCE_DIR}")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/third_party")
set(BINARY_DIR "${PROJECT_SOURCE_DIR}/bin")
set(LIBRARY_DIR "${PROJECT_SOURCE_DIR}/libs")

# 输出路径信息
#message(STATUS "FRAMEWORK2D_DIR: ${FRAMEWORK2D_DIR}")
#message(STATUS "INCLUDE_DIR: ${INCLUDE_DIR}")
#message(STATUS "THIRD_PARTY_DIR: ${THIRD_PARTY_DIR}")
#message(STATUS "BINARY_DIR: ${BINARY_DIR}")
#message(STATUS "LIBRARY_DIR: ${LIBRARY_DIR}")

#父目录的变量会自动继承到子目录:THIRD_PARTY_DIR
add_subdirectory(third_party)

set(THIRD_PARTY_LIBS glad imgui CACHE INTERNAL "Third-party targets")

message(STATUS "Third-party libs: ${THIRD_PARTY_LIBS}")

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
#父目录的变量会自动继承到子目录:THIRD_PARTY_DIR
add_subdirectory(src)
