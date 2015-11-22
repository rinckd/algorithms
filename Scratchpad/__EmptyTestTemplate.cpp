#include "stdafx.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>

class EmptyTemplateTests : public testing::Test
{
public:
    class MyClass
    {

    };
protected:
    EmptyTemplateTests() {}
    virtual ~EmptyTemplateTests() {}
};


TEST_F(EmptyTemplateTests, Test2)
{
    EXPECT_EQ(0.0F, 0.0F);
}