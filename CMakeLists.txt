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

add_subdirectory(doc)

set(CCGS_SOURCE
  src/args.cc
  src/card.cc
  src/cards_pool.cc
  src/cards_queue.cc
  src/common.cc
  src/deck.cc
  src/export.cc
  src/game.cc
  src/game_params.cc
  src/game_result.cc
  src/game_state.cc
  src/table.cc
  src/random.cc
  src/trait_impl.cc
  src/traits.cc

  src/args.hpp
  src/card.hpp
  src/cards_pool.hpp
  src/cards_queue.hpp
  src/common.hpp
  src/deck.hpp
  src/export.hpp
  src/game.hpp
  src/game_params.hpp
  src/game_result.hpp
  src/game_state.hpp
  src/player.hpp
  src/random.hpp
  src/table.hpp
  src/trait.hpp
  src/trait_impl.hpp
  src/traits.hpp
)

add_library(ccgslib STATIC ${CCGS_SOURCE})

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/spdlog)
target_include_directories(ccgslib PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(ccgslib spdlog)

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/libcsv)
target_link_libraries(ccgslib libcsv)

add_executable(${PROJECT_NAME} src/main.cc)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE ccgslib)

if(CCGS_BUILD_TEST)
  add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest EXCLUDE_FROM_ALL)
  enable_testing()

  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  include_directories(${PROJECT_SOURCE_DIR}/src)

  add_subdirectory(test)
endif()
