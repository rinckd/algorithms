#include "stdafx.h"
#include <gtest/gtest.h>
#include <memory>

class RedBlackTreeTests : public testing::Test
{
public:
    template<class T>
    class RedBlackTree
    {
        enum Color { Red, Black };

        struct Node
        {
            Node(Color c, std::shared_ptr<const Node> const & lft, T val,std::shared_ptr<const Node> const & rgt): 
                color_(c), 
                left_(lft), 
                value_(val), 
                right_(rgt) {}
            Color color_;
            std::shared_ptr<const Node> left_;
            T value_;
            std::shared_ptr<const Node> right_;
        };
        explicit RedBlackTree(std::shared_ptr<const Node> const & node) : root_(node) {}

    public:
        RedBlackTree() {}
        RedBlackTree(Color c, RedBlackTree const & left, T val, RedBlackTree const & right)
            : root_(std::make_shared<const Node>(c, left.root_, val, right.root_))
        {
            assert(left.IsEmpty() || left.root() < val);
            assert(right.IsEmpty() || val < right.root());
        }
        RedBlackTree(std::initializer_list<T> init)
        {
            RedBlackTree t;
            for (T v : init)
            {
                t = t.Insert(v);
            }
            root_ = t.root_;
        }
        Color rootColor() const
        {
            assert(!IsEmpty());
            return root_->color_;
        }
        template<class I>
        RedBlackTree(I b, I e)
        {
            RedBlackTree t;
            for_each(b, e, [&t](T const & v) {
                t = t.Insert(v);
            });
            root_ = t.root_;
        }
        bool IsEmpty() const { return !root_; }
        T root() const
        {
            assert(!IsEmpty());
            return root_->value_;
        }
        RedBlackTree left() const
        {
            assert(!IsEmpty());
            return RedBlackTree(root_->left_);
        }
        RedBlackTree right() const
        {
            assert(!IsEmpty());
            return RedBlackTree(root_->right_);
        }
        bool IsMember(T x) const
        {
            if (IsEmpty())
                return false;
            T y = root();
            if (x < y)
                return left().IsMember(x);
            else if (y < x)
                return right().IsMember(x);
            else
                return true;
        }
        RedBlackTree Insert(T x) const
        {
            RedBlackTree t = InsertNode(x);
            return RedBlackTree(Black, t.left(), t.root(), t.right());
        }
        // 1. No red node has a red child.
        void assert1() const
        {
            if (!IsEmpty())
            {
                auto lft = left();
                auto rgt = right();
                if (rootColor() == Red)
                {
                    assert(lft.IsEmpty() || lft.rootColor() == Black);
                    assert(rgt.IsEmpty() || rgt.rootColor() == Black);
                }
                lft.assert1();
                rgt.assert1();
            }
        }
        // 2. Every path from root to empty node contains the same
        // number of black nodes.
        int countB() const
        {
            if (IsEmpty())
                return 0;
            int lft = left().countB();
            int rgt = right().countB();
            assert(lft == rgt);
            return (rootColor() == Black) ? 1 + lft : lft;
        }
    private:
        RedBlackTree InsertNode(T x) const
        {
            assert1();
            if (IsEmpty())
                return RedBlackTree(Red, RedBlackTree(), x, RedBlackTree());
            T y = root();
            Color c = rootColor();
            if (rootColor() == Black)
            {
                if (x < y)
                    return BalanceNodes(left().InsertNode(x), y, right());
                else if (y < x)
                    return BalanceNodes(left(), y, right().InsertNode(x));
                else
                    return *this; // no duplicates
            }
            else
            {
                if (x < y)
                    return RedBlackTree(c, left().InsertNode(x), y, right());
                else if (y < x)
                    return RedBlackTree(c, left(), y, right().InsertNode(x));
                else
                    return *this; // no duplicates
            }
        }
        // Called only when parent is black
        static RedBlackTree BalanceNodes(RedBlackTree const & left, T value, RedBlackTree const & right)
        {
            if (left.doubledLeft())
                return RedBlackTree(Red
                    , left.left().paint(Black)
                    , left.root()
                    , RedBlackTree(Black, left.right(), value, right));
            else if (left.doubledRight())
                return RedBlackTree(Red
                    , RedBlackTree(Black, left.left(), left.root(), left.right().left())
                    , left.right().root()
                    , RedBlackTree(Black, left.right().right(), value, right));
            else if (right.doubledLeft())
                return RedBlackTree(Red
                    , RedBlackTree(Black, left, value, right.left().left())
                    , right.left().root()
                    , RedBlackTree(Black, right.left().right(), right.root(), right.right()));
            else if (right.doubledRight())
                return RedBlackTree(Red
                    , RedBlackTree(Black, left, value, right.left())
                    , right.root()
                    , right.right().paint(Black));
            else
                return RedBlackTree(Black, left, value, right);
        }
        bool doubledLeft() const
        {
            return !IsEmpty()
                && rootColor() == Red
                && !left().IsEmpty()
                && left().rootColor() == Red;
        }
        bool doubledRight() const
        {
            return !IsEmpty()
                && rootColor() == Red
                && !right().IsEmpty()
                && right().rootColor() == Red;
        }
        RedBlackTree paint(Color c) const
        {
            assert(!IsEmpty());
            return RedBlackTree(c, left(), root(), right());
        }
    private:
        std::shared_ptr<const Node> root_;
    };

    template<class T>
    void static PrintTree(RedBlackTree<T> const & t, int offset = 0)
    {
        if (!t.IsEmpty())
        {
            for (int i = 0; i < offset; ++i)
                std::cout << " ";
            std::cout << t.root() << std::endl;
            if (t.rootColor() == 0)
            {
                for (int i = 0; i < offset; ++i)
                    std::cout << " ";
                std::cout << "Red"<< std::endl;
            }
            else
            {
                for (int i = 0; i < offset; ++i)
                    std::cout << " ";
                std::cout << "Black" << std::endl;
            }
            PrintTree(t.left(), offset);
            PrintTree(t.right(), offset + 4);
        }
    }

    template<class T>
    RedBlackTree<T> TreeUnion(RedBlackTree<T> const & a, RedBlackTree<T> const & b)
    {
        // a u b = a + (b \ a)
        RedBlackTree<T> tree_result = a;
        forEach(b, [&tree_result, &a](T const & v) 
        {
            if (!a.IsMember(v))
                tree_result.Insert(v);
        });
        return tree_result;
    }



protected:
    RedBlackTreeTests() {}
    virtual ~RedBlackTreeTests() {}
};


TEST_F(RedBlackTreeTests, Test2)
{
    RedBlackTree<int> t{ 50, 40, 30, 10, 20, 30, 100, 0, 45, 55, 25, 15 };
    PrintTree(t);
    EXPECT_EQ(0.0F, 0.0F);
}