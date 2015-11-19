#include "stdafx.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>

class MergeSortTests : public testing::Test
{
protected:
    MergeSortTests() : inversion_counts(0) {}
    virtual ~MergeSortTests() {}
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
    std::vector<int> Merge(std::vector<int> left, std::vector<int> right)
    {
        std::vector<int> result;
        while (left.size() > 0U && right.size() > 0U)
        {
            if (left.size() > 0U && right.size() > 0U)
            {
                if (left.front() <= right.front())
                {
                    result.push_back(left.front());
                    left.erase(left.begin());
                }
                else
                {
                    inversion_counts += static_cast<int>(right.size() - left.size()) + 1;
                    result.push_back(right.front());
                    right.erase(right.begin());
                }
            }
        }
        for (auto i = 0U; i < left.size(); ++i)
        {
            result.push_back(left[i]);
        }
        for (auto element : right)
        {
            result.push_back(element);
        }
        return result;
    }

    std::vector<int> MergeSort(std::vector<int> arr)
    {
        if (arr.size() <= 1)
        {
            return arr;
        }
        auto middle = arr.size() / 2;
        std::vector<int> left(arr.begin(), arr.begin() + middle);
        std::vector<int> right(arr.begin() + middle, arr.end());
        auto merged_left = MergeSort(left);
        auto merged_right = MergeSort(right);
        auto result = Merge(merged_left, merged_right);
        return result;
    }

    int inversion_counts = 0;
};


TEST_F(MergeSortTests, Test2)
{
    std::vector<int> values;
    OpenFile("IntegerArray2.txt", &values);
    auto sorted = MergeSort(values);
    int sort = 0;
    EXPECT_EQ(0.0F, 0.0F);
}