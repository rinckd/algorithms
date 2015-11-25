#include "stdafx.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>

class DynamicProblemTests : public testing::Test
{
public:
    class Rectangle
    {
    public:    
        using RangeOverlap = std::pair<int, int>;
        auto x() const { return x_; }
        auto y() const { return y_; }
        auto width() const { return width_; }
        auto height() const { return height_; }
        Rectangle(int x, int y, int width, int height) :
            x_(x), y_(y), width_(width), height_(height) {}
        // bottom_left corner
        int x_;
        int y_;
        int width_;
        int height_;
    };
    static Rectangle::RangeOverlap FindRangeOverlap(int point1, int length1, int point2, int length2)
    {
        auto highest_start_point = std::max(point1, point2);
        auto lowest_end_point = std::min(point1 + length1, point2 + length2);
        if (highest_start_point >= lowest_end_point)
        {
            return std::make_pair(0, 0);
        }
        auto overlap_length = lowest_end_point - highest_start_point;
        return std::make_pair(highest_start_point, overlap_length);

    }
    static Rectangle FindIntersection(const Rectangle& rectangle1, const Rectangle& rectangle2)
    {
        auto x_overlap = FindRangeOverlap(rectangle1.x(), rectangle1.width(), rectangle2.x(), rectangle2.width());
        auto y_overlap = FindRangeOverlap(rectangle1.y(), rectangle1.height(), rectangle2.y(), rectangle2.height());
        if (x_overlap.second == 0 || y_overlap.second == 0)
        {
            Rectangle null_intersection(0, 0, 0, 0);
            return null_intersection; // no overlap. width and height are 0
        }
        
        Rectangle intersection(x_overlap.first, y_overlap.first, x_overlap.second, y_overlap.second);
        return intersection;
    }
protected:
    DynamicProblemTests() {}
    virtual ~DynamicProblemTests() {}
};


TEST_F(DynamicProblemTests, RectangleIntersection)
{
    auto rectangle1 = Rectangle(0, 10, 20, 10);
    auto rectangle2 = Rectangle(10, 0, 10, 20);
    auto intersect = FindIntersection(rectangle1, rectangle2);
    EXPECT_EQ(intersect.width(), 10);
    EXPECT_EQ(intersect.height(), 10);
    EXPECT_EQ(intersect.x(), 10);
    EXPECT_EQ(intersect.y(), 10);
}