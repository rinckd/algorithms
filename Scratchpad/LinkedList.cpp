#include "stdafx.h"
#include <gtest/gtest.h>
#include <memory>

class LinkedListTests : public testing::Test
{
public:
	template <typename T>
	struct Node
	{
		Node() : data_(T()) {}
		explicit Node(const T& value) : data_(value) {}

		T data_;
		std::shared_ptr<Node<T>> next_;
	};

	template <typename T>
	class LinkedList
	{
	public:
		LinkedList(std::initializer_list<T> init) : 
			head_(nullptr)
		{
			for (auto element : init)
			{
				push_back(element);
			}
		}
		void push_back(const T& value)
		{
			if (head_)
			{
				std::shared_ptr<Node<T>> currentNode(head_);
				while (currentNode->next_ != nullptr)
				{
					currentNode = currentNode->next_;
				}
				currentNode->next_ = std::make_shared<Node<T>>(value);
			}
			else
			{
				head_ = std::make_shared<Node<T>>(value);
			}
		}



		void print() const
		{
			std::shared_ptr<Node<T>> currentNode(head_);
			while (currentNode->next_ != nullptr)
			{
				std::cout << currentNode->data_ << " ";
				currentNode = currentNode->next_;
			}
			std::cout << currentNode->data_;
			std::cout << std::endl;
		}
	private:
		std::shared_ptr<Node<T>> head_;
	};

protected:
	LinkedListTests() {}
	virtual ~LinkedListTests() {}
};


TEST_F(LinkedListTests, Test2)
{
	LinkedList<int> lst1{ 1, 2, 3, 4, 5, 6, 7 };
	lst1.print();
	EXPECT_EQ(0.0F, 0.0F);
}