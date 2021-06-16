#
# This file is part of the CCGS (https://github.com/damianzim/ccgs)
# distribution.
# Copyright (c) 2021 Piotr Jasik & Damian Zimon.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.15)
project(${PROJECT_NAME})

set(CCGS_TESTS_SOURCE
  args_test.cc
  cards_pool_test.cc
  common_test.cc
  game_params_test.cc
  random_test.cc
  traits_test.cc
)

add_executable(ccgs_tests ${CCGS_TESTS_SOURCE})
target_link_libraries(ccgs_tests ccgslib gtest gtest_main gmock)

add_test(unit_tests ccgs_tests)
