cmake_minimum_required(VERSION 3.15)
project(${PROJECT_NAME})

set(CCGS_TESTS_SOURCE
  args_test.cc
  cards_test.cc
  game_test.cc
  random_test.cc
)

add_executable(ccgs_tests ${CCGS_TESTS_SOURCE})
target_link_libraries(ccgs_tests ccgslib gtest gtest_main gmock)

add_test(unit_tests ccgs_tests)
