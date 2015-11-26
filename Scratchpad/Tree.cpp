#include "stdafx.h"
#include <gtest/gtest.h>
#include <memory>
#include <initializer_list>
#include <queue>
#include <fstream>

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
            assert(lft.IsEmpty() || lft.node_value() < val);
            assert(rgt.IsEmpty() || val < rgt.node_value());
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
        bool IsEmpty() const { return !tree_root_; }
        T node_value() const
        {
            assert(!IsEmpty());
            return tree_root_->node_value_;
        }
        Tree left() const
        {
            assert(!IsEmpty());
            return Tree(tree_root_->left_);
        }
        Tree right() const
        {
            assert(!IsEmpty());
            return Tree(tree_root_->right_);
        }
        bool IsMember(T x) const
        {
            if (IsEmpty())
                return false;
            T y = node_value();
            if (x < y)
                return left().IsMember(x);
            else if (y < x)
                return right().IsMember(x);
            else
                return true;
        }
        Tree Insert(T x) const
        {
            if (IsEmpty())
                return Tree(Tree(), x, Tree());
            T y = node_value();
            if (x < y)
                return Tree(left().Insert(x), y, right());
            else if (y < x)
                return Tree(left(), y, right().Insert(x));
            else
                return *this; // no duplicates
        }
        void static PrintNullNode(int key, int nullcount, std::ofstream& output_stream)
        {
            output_stream << "    null" << nullcount << " [shape=point];\n";
            output_stream << "    " << key << " -> null" << nullcount << ";\n";
        }

        void static PrintNode(Tree<T> const & t, std::ofstream& output_stream)
        {
            static int nullcount = 0;

            if (!t.left().IsEmpty())
            {
                output_stream << "    " << t.node_value() << " -> " << t.left().node_value() << ";\n";
                PrintNode(t.left(), output_stream);
            }
            else
            {
                PrintNullNode(t.node_value(), nullcount++, output_stream);
            }

            if (!t.right().IsEmpty())
            {
                output_stream << "    " << t.node_value() << " -> " << t.right().node_value() << ";\n";
                PrintNode(t.right(), output_stream);
            }
            else
            {
                PrintNullNode(t.node_value(), nullcount++, output_stream);
            }
        }
        void static PrintTree(Tree<T> const & t, std::ofstream& output_stream, int offset = 0)
        {
            output_stream << "digraph BST {\n";
            output_stream << "    node [fontname=\"Arial\"];\n";
            if (t.IsEmpty())
            {
                output_stream << "\n";
            }
            else if (t.right().IsEmpty() && t.left().IsEmpty())
            {
                output_stream << "    " << t.node_value() << ";\n";
            }
            else
            {
                PrintNode(t, output_stream);
            }
            output_stream << "}\n";
        }
    private:
        std::shared_ptr<const Node> tree_root_;
    };

    template<class T, class F>
    void forEach(Tree<T> t, F f) {
        if (!t.IsEmpty()) {
            forEach(t.left(), f);
            f(t.node_value());
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
    std::ofstream myfile;
    myfile.open("example.txt");
    Tree<int>::PrintTree(tree, myfile, 0);
    EXPECT_EQ(0.0F, 0.0F);
}

TEST_F(BinaryTreeTests, Test2)
{
    auto t = Tree<int>().Insert(2).Insert(4).Insert(3).Insert(5).Insert(6).Insert(1);
    std::cout << "Is member 1? " << t.IsMember(1) << std::endl;
    std::cout << "Is member 10? " << t.IsMember(10) << std::endl;
    //Tree<int>::PrintTree(t);
    EXPECT_EQ(0.0F, 0.0F);
}