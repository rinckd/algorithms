#include "stdafx.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include <forward_list>
#include <queue>
#include <stack>
#include <list>
#include <unordered_map>
#include <chrono>


class Edge
{
private:
	double weight_;
	int vertex_id_;
public:
	Edge(double weight, int id) : weight_(weight), vertex_id_(id) {}
	double weight() const { return weight_; }
	int vertex_id() const { return vertex_id_; }
};

class Graph
{
private:
	int num_of_vertices;
	std::vector<std::vector<int>> Adj;
public:
	Graph(int V);
	void addEdge(int from, int to);
	void BFS(int Start);
	void DFS(int Start);
};

Graph::Graph(int V)
{
	this->num_of_vertices = V;
	Adj.push_back(std::vector<int>{V});
}
void Graph::addEdge(int from, int to)
{
	Adj[from].push_back(to);
}

void Graph::BFS(int Start)
{
	bool* visited = new bool[this->num_of_vertices]();
	std::queue<int> queue;
	queue.push(Start);
	std::vector<int>::iterator i;
	std::cout << "BFS: ";
	while (!queue.empty())
	{
		Start = queue.front();
		std::cout << Start << " ";
		visited[Start] = true;
		queue.pop();
		for (i = Adj[Start].begin(); i != Adj[Start].end(); i++)
		{
			if (!visited[*i])
				queue.push(*i);
		}
	}
	std::cout << std::endl;
}

void Graph::DFS(int Start)
{
	bool* visited = new bool[this->num_of_vertices]();
	std::stack<int> stack;
	stack.push(Start);
	std::vector<int>::iterator i;
	std::cout << "DFS: ";
	while (!stack.empty())
	{
		int top = stack.top();
		std::cout << top << " ";
		stack.pop();
		visited[top] = true;
		for (i = Adj[top].begin(); i != Adj[top].end(); i++)
		{
			if (!visited[*i])
				stack.push(*i);
		}
	}
	std::cout << std::endl;
}
class GraphTests : public testing::Test
{
protected:
	GraphTests() {}
	virtual ~GraphTests() {}
	void OpenFile(const std::string& file_name, std::vector<float>* values)
	{
		std::ifstream input(file_name);
		for (std::string line; getline(input, line); )
		{
			values->emplace_back(std::stof(line));
		}
	}
	void OpenFile(const std::string& file_name, std::vector<int>* values)
	{
		std::ifstream input(file_name);
		for (std::string line; getline(input, line); )
		{
			values->emplace_back(std::stoi(line));
		}
	}

	using nodeId = unsigned int;
	template <typename T_NodeValue, typename T_Weight>
	class Graph
	{
	public:
		using Nodes = std::unordered_map<nodeId, T_NodeValue >;
		using Destination = std::pair <nodeId, T_Weight>;
		using Edges = std::unordered_multimap<nodeId, Destination>;

		Graph() {};

		T_NodeValue & getNode(nodeId id)
		{
			typename Nodes::iterator it = nodes_.find(id);
			if (it != nodes_.end())
				return it->second;
			else
				throw;
		};

		void insertNode(const nodeId id, const T_NodeValue t)
		{
			nodes_.insert(Nodes::value_type(id, t));
		};

		auto edges(nodeId id) { return edges_.equal_range(id); }
		void insertEdge(const nodeId source_node, const nodeId destination_node, T_Weight v)
		{
			edges_.insert(Edges::value_type(source_node, std::make_pair(destination_node, v)));
		}

	private:
		Nodes nodes_;
		Edges edges_;
	};

	struct subset
	{
		nodeId parent, rank;
	};

	int find(struct subset subsets[], nodeId i) const
	{
		// find root and make root as parent of i (path compression)
		if (subsets[i].parent != i)
		{
			subsets[i].parent = find(subsets, subsets[i].parent);
		}
		return subsets[i].parent;
	}

	// A function that does union of two sets of x and y
	// (uses union by rank)
	void Union(struct subset subsets[], int x, int y) const
	{
		nodeId xroot = find(subsets, x);
		nodeId yroot = find(subsets, y);

		// Attach smaller rank tree under root of high rank tree
		// (Union by Rank)
		if (subsets[xroot].rank < subsets[yroot].rank)
			subsets[xroot].parent = yroot;
		else if (subsets[xroot].rank > subsets[yroot].rank)
			subsets[yroot].parent = xroot;

		// If ranks are same, then make one as root and increment
		// its rank by one
		else
		{
			subsets[yroot].parent = xroot;
			subsets[xroot].rank++;
		}
	}

};


TEST_F(GraphTests, AdjacencyList)
{
	auto start = std::chrono::steady_clock::now();
	auto g = Graph<int, double>();
	g.insertNode(1, 0);
	g.insertEdge(1, 2, 0.6);
	g.insertEdge(1233, 4, 0.9);
	g.insertNode(2, 0);
	g.insertEdge(2, 22, 0.1);
	g.insertEdge(1, 3, 0.3);
	unsigned int n = g.getNode(1);
	std::cout << "node n=" << n << std::endl;
	auto range = g.edges(1);
	for (auto it = range.first; it != range.second; ++it)
	{
		std::cout << "e=(" << it->first << "," << it->second.first << ",w=" << it->second.second << ")" << std::endl;
	}
	int sort = 0;
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	auto time_in_ms = std::chrono::duration <double, std::milli>(diff).count();
	EXPECT_EQ(0.0F, 0.0F);
}