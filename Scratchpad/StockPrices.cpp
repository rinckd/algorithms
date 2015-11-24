#include "stdafx.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>

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
protected:
	StockPricesTest() {}
	virtual ~StockPricesTest() {}
};


TEST_F(StockPricesTest, MaxProfit)
{
	std::vector<int> stock_prices_yesterday{ 10, 7, 5, 8, 11, 9 };	
	EXPECT_EQ(StockPrices::MaxProfit(stock_prices_yesterday), 6);
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