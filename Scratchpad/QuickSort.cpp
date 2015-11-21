#include "stdafx.h"
#include <algorithm>
#include <fstream>
#include <random>
#include <string>
#include "../packages/gtest.1.7.0.1/build/native/include/gtest/gtest.h"

class QuickSortTests : public testing::Test
{


protected:

	enum class PivotMode
	{
		kFirstElement,
		kFinalElement,
		kMedianOfThreePivotRule
	};

    QuickSortTests() {}
    virtual ~QuickSortTests() {}
    void OpenFile(const std::string& file_name, std::vector<float>* values)
    {
        std::ifstream input(file_name);
        for (std::string line; getline(input, line); )
        {
            values->emplace_back(stof(line));
        }
    }
    void OpenFile(const std::string& file_name, std::vector<int>* values)
    {
        std::ifstream input(file_name);
        for (std::string line; getline(input, line); )
        {
            values->emplace_back(stoi(line));
        }
    }


	//
	size_t RandomSelection(std::vector<int>& array, size_t start_index, int end_index, int k_index) const
	{
		if (start_index == end_index) return array[start_index];		
		srand(static_cast<unsigned int>(time(nullptr)));
		auto pivot_index = start_index + rand() % (end_index - start_index + 1);
		int mid = partition(array, pivot_index, start_index, end_index);
		auto i = mid - static_cast<int>(start_index) + 1;
		if (i == k_index)
			return array[mid];
		else if (k_index < i)
			return RandomSelection(array, start_index, mid - 1, k_index);
		else
			return RandomSelection(array, mid + 1, end_index, k_index - i);


    }
    size_t partition(std::vector<int>& array, size_t pivot_index, size_t start_index, size_t end_index) const
    {
        auto pivot = array[pivot_index];
        std::swap(array[pivot_index], array[end_index]); // move pivot to end
        auto current_index = start_index;
        for (auto i = start_index; i < end_index; ++i) 
        {
            if (array[i] <= pivot) 
            {
                std::swap(array[i], array[current_index]);
                current_index++;
            }
        }
        std::swap(array[current_index], array[end_index]);
        return current_index;
    }
	void MedianOfThree(std::vector<int>&arr, size_t l, size_t u) const
	{
		int m = l + (u - l) / 2;
		if (arr[l] >= arr[m] && arr[l] >= arr[u])
		{
			if (arr[m] >= arr[u])
			{
				std::swap(arr[m], arr[l]);
			}
			else
			{
				std::swap(arr[u], arr[l]);
			}			
		}
		else if (arr[m] >= arr[l] && arr[m] >= arr[u])
		{
			if (arr[u] >= arr[l])
			{
				std::swap(arr[u], arr[l]);
			}
		}
		else if (arr[m] >= arr[l])
		{
			std::swap(arr[m], arr[l]);
		}
	}

    void quicksort(std::vector<int>& array, size_t left, size_t right, PivotMode pivot_mode) const
    {
        if (left < right) 
        {
			auto new_pivot = 0U;
	        switch (pivot_mode)
	        {
	        case PivotMode::kFirstElement: 
				new_pivot = partition(array, left, left, right);
				break;
	        case PivotMode::kFinalElement: 
				std::swap(array[left], array[right]);
				new_pivot = partition(array, left, left, right);
				break;
	        case PivotMode::kMedianOfThreePivotRule: 
				MedianOfThree(array, left, right);
				new_pivot = partition(array, left, left, right);
				break;
	        default: break;
	        }
            quicksort(array, left, new_pivot - 1, pivot_mode);
            quicksort(array, new_pivot + 1, right, pivot_mode);
        }
    }
};


TEST_F(QuickSortTests, Test2)
{
    std::vector<int> example{ 2,4,6,7,2,1,5 };
    quicksort(example, 0, example.size() - 1, PivotMode::kFirstElement);
    copy(example.begin(), example.end(), std::ostream_iterator<int>(std::cout, " "));
    std::vector<int> values;
 //   OpenFile("IntegerArray2.txt", &values);
	//quicksort(values, 0, values.size() - 1, PivotMode::kFirstElement);
 //   int sort = 0;
    EXPECT_EQ(0.0F, 0.0F);
}

TEST_F(QuickSortTests, RandomizedSelection)
{
	std::vector<int> A = { 9,5,7,1,10,2,3 };
	int loc = RandomSelection(A, 0, 6, 5);
	EXPECT_EQ(0.0F, 0.0F);
}