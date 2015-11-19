// Scratchpad.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include <string>
#include <fstream>

class GoogleTests : public testing::Test
{
protected:
    GoogleTests() 
    {
        // You can do set-up work for each test here.
    }

    virtual ~GoogleTests()
    {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:
    virtual void SetUp() override
    {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown() override
    {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    void OpenFile(const std::string& file_name, std::vector<float>* values)
    {
        std::ifstream input(file_name);
        for (std::string line; getline(input, line); )
        {
            values->emplace_back(std::stof(line));
        }
    }
    void OpenFile(const std::string& file_name, std::vector<int>* values)
    {
        std::ifstream input(file_name);
        for (std::string line; getline(input, line); )
        {
            values->emplace_back(std::stoi(line));
        }
    }

};

TEST_F(GoogleTests, huhuh)
{
    std::vector<int> values;
    OpenFile("IntegerArray2.txt", &values);
    int k = 0;
    EXPECT_EQ(0.0F, 0.0F);
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}