#include "stdafx.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include <Eigen/Dense>

class MatrixTests : public testing::Test
{
public:
    class Matrix
    {
    public:
	    static double DotProduct()
        {
			Eigen::VectorXd v(5);
			v << 1.0, 1.0, 1.0, 1.0, 1.0;
			Eigen::VectorXd v2(5);
			v2 << 10.0, 20.0, 0.0, 40.0, -100.0;
			return v.dot(v2);
        }
    };
protected:
    MatrixTests() {}
    virtual ~MatrixTests() {}
};


TEST_F(MatrixTests, DotProduct)
{
    auto return_value = Matrix::DotProduct();
    EXPECT_EQ(-30.0, return_value);
}