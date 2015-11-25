#include "stdafx.h"
#include <gtest/gtest.h>
#include <memory>
#include <initializer_list>

class BinaryTreeTests : public testing::Test
{
public:
    template<class T>
    class Tree
    {
        struct Node
        {
            Node(std::shared_ptr<const Node> const & lft, T val, std::shared_ptr<const Node> const & rgt) : 
                left_(lft), 
                node_value_(val), 
                right_(rgt)
            {}

            std::shared_ptr<const Node> left_;
            T node_value_;
            std::shared_ptr<const Node> right_;
        };
        explicit Tree(std::shared_ptr<const Node> const & node) : tree_root_(node) {}
    public:
        Tree() {}
        Tree(Tree const & lft, T val, Tree const & rgt)
            : tree_root_(std::make_shared<const Node>(lft.tree_root_, val, rgt.tree_root_))
        {
            assert(lft.isEmpty() || lft.root() < val);
            assert(rgt.isEmpty() || val < rgt.root());
        }
        Tree(std::initializer_list<T> init_values)
        {
            Tree tree;
            for (T v : init_values)
            {
                tree = tree.Insert(v);
            }
            tree_root_ = tree.tree_root_;
        }
        bool isEmpty() const { return !tree_root_; }
        T root() const
        {
            assert(!isEmpty());
            return tree_root_->node_value_;
        }
        Tree left() const
        {
            assert(!isEmpty());
            return Tree(tree_root_->left_);
        }
        Tree right() const
        {
            assert(!isEmpty());
            return Tree(tree_root_->right_);
        }
        bool IsMember(T x) const
        {
            if (isEmpty())
                return false;
            T y = root();
            if (x < y)
                return left().IsMember(x);
            else if (y < x)
                return right().IsMember(x);
            else
                return true;
        }
        Tree Insert(T x) const
        {
            if (isEmpty())
                return Tree(Tree(), x, Tree());
            T y = root();
            if (x < y)
                return Tree(left().Insert(x), y, right());
            else if (y < x)
                return Tree(left(), y, right().Insert(x));
            else
                return *this; // no duplicates
        }
        void static PrintTree(Tree<T> const & t, int offset = 0)
        {
            if (!t.isEmpty())
            {
                for (int i = 0; i < offset; ++i)
                    std::cout << " ";
                std::cout << t.root() << std::endl;
                PrintTree(t.left(), offset);
                PrintTree(t.right(), offset + 4);
            }
        }
    private:
        std::shared_ptr<const Node> tree_root_;
    };

    template<class T, class F>
    void forEach(Tree<T> t, F f) {
        if (!t.IsEmpty()) {
            forEach(t.left(), f);
            f(t.root());
            forEach(t.right(), f);
        }
    }


protected:
    BinaryTreeTests() {}
    virtual ~BinaryTreeTests() {}
};
 

TEST_F(BinaryTreeTests, SimpleUnbalanced)
{
    Tree<int> tree{ 1, 2, 3};
    tree = tree.Insert(400);
    Tree<int>::PrintTree(tree);
    EXPECT_EQ(0.0F, 0.0F);
}

TEST_F(BinaryTreeTests, Test2)
{
    auto t = Tree<int>().Insert(2).Insert(4).Insert(3).Insert(5).Insert(6).Insert(1);
    std::cout << "Is member 1? " << t.IsMember(1) << std::endl;
    std::cout << "Is member 10? " << t.IsMember(10) << std::endl;
    Tree<int>::PrintTree(t);
    EXPECT_EQ(0.0F, 0.0F);
}