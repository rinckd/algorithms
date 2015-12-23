#include "gtest/gtest.h"
#include "Algorithms.h"
#include <chrono>
#include <numeric>

using namespace Algorithms;

class Utilities : public ::testing::Test
{
protected:
    virtual void TearDown() {}
    virtual void SetUp() {}
public:
    Utilities() : Test() {}
    virtual ~Utilities() {}
};

// These tests do pass, but are not just meant to be run for Validation.
// They are ways to quickly get logging information out of system
//   for debugging purposes.



TEST_F(Utilities, SpeedTests)
{
    EXPECT_EQ(0.0F, 0.0F);
}