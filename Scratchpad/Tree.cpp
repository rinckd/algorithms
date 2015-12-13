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
                right_(rgt), 
                node_value_(val)
            {}

            std::shared_ptr<const Node> left_;
            std::shared_ptr<const Node> right_;
			T node_value_;
        };
        explicit Tree(std::shared_ptr<const Node> const & node) : tree_root_(node) {}
    public:
        Tree() {}
        Tree(Tree const & lft, T val, Tree const & rgt)
            : tree_root_(std::make_shared<const Node>(lft.tree_root_, val, rgt.tree_root_)) {}
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
		int MinimumDepth(Tree<T> const& root)
		{
			if (root.IsEmpty())
			{
				return 0;
			}
			if (root.left().IsEmpty() && root.right().IsEmpty())
			{
				return 1;
			}
			auto left = -1;
			auto right = -1;
			if (!root.left().IsEmpty())
			{
				left = MinimumDepth(root.left()) + 1;
			}
			if (!root.right().IsEmpty())
			{
				right = MinimumDepth(root.right()) + 1;
			}

			if (right >= 0 && right < left)
			{
				return right;
			}
			else
			{
				if (left >= 0)
				{
					return left;
				}
				return right;
			}
		}

		std::vector<std::vector<T> > RunPathSum(Tree<T> const& root, T sum )
        {
			std::vector<std::vector<T> > result;
			std::vector<T> v;
			GeneratePathSum(root, sum, v, result);
			return result;
        }
		void GeneratePathSum(Tree<T> const& root, T sum, std::vector<T> v, std::vector<std::vector<T> >& result)
        {
			if (root.IsEmpty()) 
			{
				return;
			}

			if (root.left().IsEmpty() && root.right().IsEmpty()) 
			{
				if (root.node_value() == sum) 
				{
					v.push_back(root.node_value());
					result.push_back(v);
				}
				return;
			}

			v.push_back(root.node_value());
			if (!root.left().IsEmpty()) 
			{
				GeneratePathSum(root.left(), sum - root.node_value(), v, result);
			}
			if (!root.right().IsEmpty()) 
			{
				GeneratePathSum(root.right(), sum - root.node_value(), v, result);
			}
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
    auto t = Tree<int>().Insert(4).Insert(200).Insert(280).Insert(300).Insert(6).Insert(1).Insert(2);
	auto result = t.RunPathSum(t, 7);
    std::cout << "Is member 1? " << t.IsMember(1) << std::endl;
    std::cout << "Is member 10? " << t.IsMember(10) << std::endl;
	std::ofstream myfile;
	myfile.open("example.txt");
	Tree<int>::PrintTree(t, myfile, 0);
    EXPECT_EQ(0.0F, 0.0F);
}