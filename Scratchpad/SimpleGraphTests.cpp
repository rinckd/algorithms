#include "stdafx.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <chrono>

class SimpleGraphTests : public testing::Test
{
public:
    enum class ListTypes
    {
        kIncludesWeights,
        kNoWeights
    };
protected:
    SimpleGraphTests() {}
    virtual ~SimpleGraphTests() {}

    class Graph
    {
    public:
        using NodeId = unsigned int;
        using AdjacencyList = std::vector<NodeId>;
        using T_Graph = std::unordered_map<NodeId, AdjacencyList>;

        Graph() : internal_(0)
        {
			auto time_seed = static_cast<unsigned int>(time(nullptr));
			srand(time_seed);
        }

        auto NumberOfNodes() const { return graph_.size(); }

        void InsertEdge(const NodeId source_node, const NodeId destination_node)
        {
			auto it = graph_.find(source_node);
			if (it != graph_.end())
			{
				it->second.emplace_back(destination_node);
			}
        }
		void AppendAdjacencyList(const NodeId source_node, const NodeId second_node)
        {
			auto destination_edges = graph_.find(second_node);
			auto adj_list = destination_edges->second;
			auto it = graph_.find(source_node);
			if (it != graph_.end())
			{
				it->second.insert(it->second.end(), adj_list.begin(), adj_list.end());
			}	        
        }
		void InsertAdjancencyList(const AdjacencyList& adjacency_list, const NodeId node)
        {
			graph_.insert(T_Graph::value_type(node, adjacency_list));
        }
        void ReadAdjacencyList(const std::string& file_name, const ListTypes& list_type)
        {
            std::ifstream input(file_name);
            for (std::string line; getline(input, line); )
            {
                std::stringstream linestream(line);
                auto i = 0; // first line is node number
                auto node_index = 0;
                AdjacencyList adjacency_list;
                NodeId node_id;
                for (std::string tab_delim; getline(linestream, tab_delim, '\t');)
                {
                    if (i == 0)
                    {
                        node_index = atoi(tab_delim.c_str());                        
                        i++;
                    }
                    else
                    {
                        auto destination_node_index = atoi(tab_delim.c_str());
						adjacency_list.emplace_back(destination_node_index);
                    }
                }
				InsertAdjancencyList(adjacency_list, node_index);
            }
        }
        void ReplaceNodeReference(NodeId old_node, NodeId new_node)
        {
            for (auto& node : graph_)
            {
				for (auto& adj_list : node.second)
				{
					if (adj_list == old_node)
					{
						adj_list = new_node;
					}
				}
            }
        }
        void RemoveSelfLoops(NodeId node)
        {
            auto it = graph_.find(node);
			it->second.erase(remove(it->second.begin(), it->second.end(), node), it->second.end());
        }
		void RemoveNode(NodeId node)
        {
			graph_.erase(node);
        }
        void KargerStep()
        {
			auto random_index = rand() % graph_.size();
			auto item = graph_.begin();
			advance(item, random_index);
			auto adj_list = item->second;
			int random_edge = rand() % adj_list.size();
            auto source_node = item->first;
            auto destination_node = adj_list[random_edge];
            // Start with node source_node. Merge destination_node into source_node by attaching
            //   node destination_node's adjacency list to node source_node
			AppendAdjacencyList(source_node, destination_node);
            // Scan adjacency lists, and replace node destination_node reference with source_node
            ReplaceNodeReference(destination_node, source_node);
            // Remove all self-loops in source_node's list
            RemoveSelfLoops(source_node);
            // Remove Node 2
            RemoveNode(destination_node);
        }
    private:
        T_Graph graph_;
        int internal_;

    };

};


TEST_F(SimpleGraphTests, AdjacencyList)
{
    auto start = std::chrono::steady_clock::now();
	auto graph = Graph();
	graph.ReadAdjacencyList("SimpleGraph.txt", ListTypes::kNoWeights);
	do
	{
		graph.KargerStep();
	} while (graph.NumberOfNodes() > 2);



    int sort = 0;
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    auto time_in_ms = std::chrono::duration <double, std::milli>(diff).count();
    EXPECT_EQ(0.0F, 0.0F);
}

