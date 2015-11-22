// Scratchpad.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include "../packages/gtest.1.7.0.1/build/native/include/gtest/gtest.h"
#include <map>

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
    int ChangePossibilitiesTopDown(float amount_left, std::vector<int>& denominations_left)
    {
        if (amount_left == 0)
        {
            return 1;
        }
        if (amount_left < 0)
        {
            return 0;
        }
        if (denominations_left.size() == 0)
        {
            return 0; // we're out of denominations
        }
        std::cout << "checking ways to make: " << amount_left << " with ";
        for (const auto& denomination : denominations_left)
        {
            std::cout << denomination << ", ";
        }
        std::cout << std::endl;
        auto current_coin = denominations_left.back();
        std::vector<int> rest_of_coins(denominations_left.begin(), denominations_left.end() - 1);
        int num_possibilities = 0;
        do
        {
            num_possibilities += ChangePossibilitiesTopDown(amount_left, rest_of_coins);
            amount_left -= current_coin;

        } while (amount_left >= 0);
        return num_possibilities;
    }
    int ChangePossibilitiesMemoized(float amount_left, std::vector<int>& denominations_left)
    {
        // TODO:
        //    # check our memo and short - circuit if we've already solved this one
        //    memo_key = str((amount_left, denominations_left))
        //    if memo_key in self.memo:
        //print "grabbing memo[%s]" % memo_key
        //    return self.memo[memo_key]
        

        if (amount_left == 0)
        {
            return 1;
        }
        if (amount_left < 0)
        {
            return 0;
        }
        if (denominations_left.size() == 0)
        {
            return 0; // we're out of denominations
        }
        std::cout << "checking ways to make: " << amount_left << " with ";
        for (const auto& denomination : denominations_left)
        {
            std::cout << denomination << ", ";
        }
        std::cout << std::endl;
        auto current_coin = denominations_left.back();
        std::vector<int> rest_of_coins(denominations_left.begin(), denominations_left.end() - 1);
        int num_possibilities = 0;
        do
        {
            num_possibilities += ChangePossibilitiesTopDown(amount_left, rest_of_coins);
            amount_left -= current_coin;

        } while (amount_left >= 0);
        // TODO:   # save the answer in our memo so we don't compute it again
        //    self.memo[memo_key] = num_possibilities
        return num_possibilities;
    }

    using T_MapKey = std::pair<std::string, std::vector<float> >;
    std::map<T_MapKey, float> memoization_;
};

TEST_F(GoogleTests, huhuh)
{
    std::vector<int> values;
    OpenFile("IntegerArray2.txt", &values);
    int k = 0;
    EXPECT_EQ(0.0F, 0.0F);
}

TEST_F(GoogleTests, ChangePossibilities)
{
    std::vector<int> test = { 1 , 2, 5 };
    auto test2 = ChangePossibilitiesTopDown(5, test);
    auto k = 1;
}


TEST_F(GoogleTests, ChangePossibilitiesMemoized)
{
    EXPECT_EQ(0.0F, 0.0F);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}