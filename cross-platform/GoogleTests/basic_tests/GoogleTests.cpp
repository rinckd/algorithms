#include <MoveClassExample.h>
#include "gtest/gtest.h"
#include "Algorithms.h"


class GoogleTests : public ::testing::Test
{
protected:
    virtual void TearDown() {}
    virtual void SetUp() {}
public:
    GoogleTests() : Test() {}
    virtual ~GoogleTests() {}
};



TEST_F(GoogleTests, Test)
{
    EXPECT_EQ(1, 300);
}

