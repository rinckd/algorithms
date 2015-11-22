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
        static void EigenTests()
        {
            Eigen::MatrixXd m(2, 2);
            m(0, 0) = 3;
            m(1, 0) = 2.5;
            m(0, 1) = -1;
            m(1, 1) = m(1, 0) + m(0, 1);
            std::cout << m << std::endl;
        }
    };
protected:
    MatrixTests() {}
    virtual ~MatrixTests() {}
};


TEST_F(MatrixTests, Test2)
{

    Matrix::EigenTests();
    std::vector<int> values;
    int sort = 0;
    EXPECT_EQ(0.0F, 0.0F);
}