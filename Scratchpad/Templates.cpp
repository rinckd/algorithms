#include "stdafx.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>

class TemplateTests : public testing::Test
{
public:	
	struct mathematics
	{
		template <typename scalar_t> scalar_t sq(scalar_t x) const
		{
			return x * x;
		}
	};

	template <typename scalar_t> struct more_mathematics
	{
		template <typename other_t> static scalar_t product(scalar_t x, other_t y)
		{
			return x*y;
		}
	};

	struct base
	{
		base() {}
		template <typename T>
		base(T x) {}

	};

	struct derived : base
	{
		derived() {} 
		derived(const derived& that) : base(that) {}
	};

	template <typename T>
	class MyClass
	{
		
	};
protected:
	TemplateTests() {}
	virtual ~TemplateTests() {}
};


TEST_F(TemplateTests, Test2)
{
	auto A = mathematics().sq(3.14);
	double B = more_mathematics<double>().product(3.14, 4);

	std::vector<int> values;
	int sort = 0;
	EXPECT_EQ(0.0F, 0.0F);
}