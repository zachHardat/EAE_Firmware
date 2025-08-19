cmake_minimum_required(VERSION 3.16)
project(EAE_Firmware VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_TESTING "Build tests" ON)

# Build an executable
add_subdirectory(src)

# Tests
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
