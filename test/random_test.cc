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
