cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME ccgs)
project(${PROJECT_NAME} CXX)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options("-Wno-unknown-warning-option")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  message("Debug build")
endif()

option(CCGS_BUILD_TEST "Build CCGS unit tests." ON)

find_package(LATEX)
if(LATEX_FOUND)
  include(CMake/UseLATEX.cmake)
  add_latex_document(CCG_description.tex TARGET_NAME ccgs_description)
endif()

set(CCGS_SOURCE
  src/args.cc
  src/card.cc
  src/cards.cc
  src/common.cc
  src/game.cc
  src/random.cc

  src/args.hpp
  src/card.hpp
  src/cards.hpp
  src/common.hpp
  src/game.hpp
  src/player.hpp
  src/random.hpp
)

add_library(ccgslib STATIC ${CCGS_SOURCE})

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/spdlog)
target_link_libraries(ccgslib spdlog)

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
