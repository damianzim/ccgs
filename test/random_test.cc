//
// This file is part of the CCGS (https://github.com/damianzim/ccgs)
// distribution.
// Copyright (c) 2021 Piotr Jasik & Damian Zimon.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include "random.hpp"

#include "gtest/gtest.h"

namespace {

class DeviceGeneratorTest : public ::testing::Test {
 protected:
  DeviceGenerator gen_;
};

}  // namespace

TEST_F(DeviceGeneratorTest, InvalidInterval) {
  ASSERT_EQ(gen_.RandomBetween(1, 0), -1);
}

TEST_F(DeviceGeneratorTest, ExactValue) {
  ASSERT_EQ(gen_.RandomBetween(0, 0), 0);
  ASSERT_EQ(gen_.RandomBetween(1, 1), 1);
  ASSERT_EQ(gen_.RandomBetween(0xFCE, 0xFCE), 0xFCE);

  ASSERT_EQ(gen_.RandomMax(0), 0);
}
