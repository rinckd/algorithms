#include "stdafx.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <chrono>
#include <queue>

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
        struct DistanceNodes
        {
            DistanceNodes() : distance_(0), node_(0) {}
            int distance_;
            NodeId node_;
        };
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
            graph_backup_ = graph_;
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
        void GraphRefresh()
        {
            graph_ = graph_backup_;
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
            if (internal_ == 0)
            {
                internal_++;
                source_node = 1;
                destination_node = 7;
            }
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
        void CheckBest()
        {
            if (best_cut_.size() == 0)
            {
                best_cut_ = graph_;
            }
            else
            {
                if (graph_.size() > 0)
                {
                    if (graph_.begin()->second.size() < best_cut_.begin()->second.size())
                    {
                        best_cut_ = graph_;
                    }
                }
            }
        }
        int MinCut()
        {
            return best_cut_.begin()->second.size();
        }

        int BreadthFirstSearch(NodeId source_node, NodeId destination_node)
        {
            std::queue<DistanceNodes> search_queue;
            std::set<NodeId> visited_nodes;
            DistanceNodes source;
            source.distance_ = 0;
            source.node_ = source_node;
            search_queue.push(source);
            do
            {
                auto search_node = search_queue.front();
                search_queue.pop();
                if (visited_nodes.find(search_node.node_) == visited_nodes.end())
                {
                    visited_nodes.emplace(search_node.node_);
                    for (auto neighbor : graph_[search_node.node_])
                    {
                        if (neighbor == destination_node)
                        {
                            return search_node.distance_ + 1;
                        }
                        DistanceNodes new_neighbor;
                        new_neighbor.distance_ = search_node.distance_ + 1;
                        new_neighbor.node_ = neighbor;
                        search_queue.push(new_neighbor);
                    }
                }
             } 
            while (search_queue.size() > 0);

            // not found;
            return -1;

        }

    private:
        T_Graph graph_;
        T_Graph best_cut_;
        T_Graph graph_backup_;
        int internal_;

    };

};


TEST_F(SimpleGraphTests, KargerMinCut)
{
    //    3-----4--5-----6
    //    | \ / |  | \ / |
    //    | / \ |  | / \ |
    //    2-----1--7-----8
    // - Expected result: 2
    //     min cuts are[(1, 7), (4, 5)]
    auto start = std::chrono::steady_clock::now();
	auto graph = Graph();
	graph.ReadAdjacencyList("kargerMinCutSimple.txt", ListTypes::kNoWeights);
    for (auto i = 1; i < 200; ++i)
    {
        do
        {
            graph.KargerStep();
        } 
        while (graph.NumberOfNodes() > 2);
        graph.CheckBest();
        graph.GraphRefresh();
    }
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    auto time_in_ms = std::chrono::duration <double, std::milli>(diff).count();
    EXPECT_EQ(graph.MinCut(), 2);
}

TEST_F(SimpleGraphTests, test)
{
    auto graph = Graph();
    graph.ReadAdjacencyList("kargerMinCutSimple.txt", ListTypes::kNoWeights);
    auto result = graph.BreadthFirstSearch(2, 6);
    EXPECT_EQ(result, 3);
}

