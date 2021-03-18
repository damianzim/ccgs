cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME ccgs)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(CCGS_BUILD_TEST "Build CCGS unit tests." ON)

find_package(LATEX)
if (LATEX_FOUND)
  include(CMake/UseLATEX.cmake)
  add_latex_document(CCG_description.tex TARGET_NAME ccgs_description)
endif()

set(CCGS_SOURCE
  src/common.cc

  src/args.hpp
  src/common.hpp
)

add_library(ccgslib STATIC ${CCGS_SOURCE})

add_executable(${PROJECT_NAME} src/main.cc)
target_include_directories(${PROJECT_NAME} PRIVATE src/)
target_link_libraries(${PROJECT_NAME} PRIVATE ccgslib)

if(CCGS_BUILD_TEST)
  add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest EXCLUDE_FROM_ALL)
  enable_testing()

  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  include_directories(${PROJECT_SOURCE_DIR}/src)

  add_subdirectory(tests)
endif()
