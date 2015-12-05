#include "stdafx.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include <map>

class StockPricesTest : public testing::Test
{
public:
	class StockPrices
	{

	public:
		static int MaxProfit(const std::vector<int>& stock_prices)
		{
			auto min_price = stock_prices.at(0);
			auto max_profit = stock_prices.at(1) - stock_prices.at(0);
			for (auto& current_price : stock_prices)
			{
				min_price = std::min(min_price, current_price);
				auto potential_profit = current_price - min_price;
				max_profit = std::max(max_profit, potential_profit);
			}
			return max_profit;
		}

	};

	class ProductsOfInts
	{
	public:
		static std::vector<int> ProductOfOtherNumbers(const std::vector<int>& numbers)
		{
			std::vector<int> return_vector(numbers.size(), 0);
			auto product_so_far = 1;
			auto i = 0;
			do
			{
				return_vector[i] = product_so_far;
				product_so_far *= numbers[i];
				i++;
			} while (i < static_cast<int>(numbers.size()));			
			product_so_far = 1;
			i = static_cast<int>(numbers.size()) - 1;
			do
			{
				return_vector[i] *= product_so_far;
				product_so_far *= numbers[i];
				i--;
			} while (i >= 0);
			return return_vector;
		}
	};


	class HiCal
	{
	public:
		using MeetingTimes = std::pair<int, int>;	
		static std::vector<MeetingTimes> OverlapMeetings(std::vector<MeetingTimes>& meetings)
		{
			sort(meetings.begin(), meetings.end());
			std::vector<MeetingTimes> merged_meetings;
			auto i = 0;
			auto previous_meeting_start = 0;
			auto previous_meeting_end = 0;
			for (auto& meeting: meetings)
			{
				if (i == 0)
				{
					previous_meeting_start = meeting.first;
					previous_meeting_end = meeting.second;
				}
				else
				{
					auto current_meeting_start = meeting.first;
					auto current_meeting_end = meeting.second;
					if (current_meeting_start <= previous_meeting_end)
					{
						previous_meeting_end = std::max(current_meeting_end, previous_meeting_end);
					}
					else
					{
						merged_meetings.emplace_back(previous_meeting_start, previous_meeting_end);
						previous_meeting_start = current_meeting_start;
						previous_meeting_end = current_meeting_end;
					}
				}
				i++;				
			}
			merged_meetings.emplace_back(previous_meeting_start, previous_meeting_end);
			return merged_meetings;
		}
	};

	int static FindRotationPoint(std::vector<std::string>& words)
	{
		auto first_word = words.at(0);
		auto floor_index = 0U;
		auto ceiling_index = words.size() - 1;
		do
		{
			auto guess_index = floor_index + (ceiling_index - floor_index) / 2;
			if (words.at(guess_index).at(0) > first_word.at(0))
			{
				// go right
				floor_index = guess_index;
			}
			else
			{
				// go left
				ceiling_index = guess_index;
			}
			if (floor_index + 1 == ceiling_index)
			{
				return ceiling_index;
			}
		} while (floor_index < ceiling_index);

		return 0;
	}

	class Fibonacci
	{
		std::map<int, int> memoized_;
	public:
		int CalcFibonacci(int series)
		{
			if (series <= 1)
			{
				return series;
			}
			auto map_value = memoized_.find(series);
			if (map_value != memoized_.end())
			{
				return map_value->second;
			}

			auto result = CalcFibonacci(series - 1) + CalcFibonacci(series - 2);
			memoized_.insert(std::pair<int, int>(series, result));
			return result;
		}
		int CalcFibIterative(int series)
		{
			auto start_value = 0;
			auto next_value = 1;
			auto cumulative_value = 0;
			for (int i = 0; i < series; i++)
			{
				cumulative_value = start_value + next_value;
				start_value = next_value;
				next_value = cumulative_value;
			}
			return cumulative_value;
		}
	};


protected:
	StockPricesTest() {}
	virtual ~StockPricesTest() {}
};

TEST_F(StockPricesTest, RotationPoint)
{
	std::vector<std::string> word_dict{ "k", "v", "a", "b","c","d","e","g","i" };
	auto test = FindRotationPoint(word_dict);
	EXPECT_EQ(test, 2);
}
TEST_F(StockPricesTest, MaxProfit)
{
	std::vector<int> stock_prices_yesterday{ 10, 7, 5, 8, 11, 9 };	
	EXPECT_EQ(StockPrices::MaxProfit(stock_prices_yesterday), 6);
}

TEST_F(StockPricesTest, Fibonacci)
{
	Fibonacci fib;
	auto test = fib.CalcFibIterative(11);
	auto j = 0;
}

TEST_F(StockPricesTest, ProductsOfInts)
{
	std::vector<int> example_array{ 1, 2, 6, 5, 9 };
	std::vector<int> test_result{ 540, 270, 90, 108, 60 };

	EXPECT_EQ(ProductsOfInts::ProductOfOtherNumbers(example_array), test_result);
}

TEST_F(StockPricesTest, Scheduling )
{
	std::vector<HiCal::MeetingTimes> t;
	t.emplace_back(std::make_pair(1, 3));
	t.emplace_back(std::make_pair(2, 4));
	std::vector<HiCal::MeetingTimes> expected_value;
	expected_value.emplace_back(std::make_pair(1, 4));

	auto return_value = HiCal::OverlapMeetings(t);
	EXPECT_EQ(return_value, expected_value);
}