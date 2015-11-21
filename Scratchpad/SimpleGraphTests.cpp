#include "stdafx.h"
#include <gtest/gtest.h>
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
        using T_Graph = std::unordered_multimap<NodeId, AdjacencyList>;

        Graph() : internal_(0) {}

        auto edges(NodeId id) const { return edges_.equal_range(id); }
        auto NumberOfEdges() const { return edges_.size(); }

        void InsertEdge(const NodeId source_node, const NodeId destination_node, T_EdgeWeight v)
        {
            edges_.insert(T_Graph::value_type(source_node, std::make_pair(destination_node, v)));
        }
        void ReadAdjacencyList(const std::string& file_name, const ListTypes& list_type, Graph *graph)
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
                        graph->InsertEdge(node_index, destination_node_index, 0.0); // no weights
                    }
                }
            }
        }
        void ReplaceNodeReference(NodeId old_node, NodeId new_node)
        {
            for (const auto & node : nodes_)
            {
                auto range = edges_.equal_range(node.first);
                for (auto it = range.first; it != range.second; ++it)
                {
                    if (it->second.first == old_node)
                    {
                        it->second.first = new_node;
                    }
                }
            }
        }
        void RemoveSelfLoops(NodeId node)
        {
            auto edges = edges_.equal_range(node);
            for (auto it = edges.first; it != edges.second;)
            {
                if (it->second.first == node)
                {
                    it = edges_.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
        void KargerStep()
        {
            auto source_node = 1;
            auto destination_node = 2;
            if (internal_ == 0)
            {
                source_node = 1;
                destination_node = 2;
                internal_++;
            }
            else if (internal_ == 1)
            {
                source_node = 1;
                destination_node = 4;
            }

            // Start with node source_node. Merge destination_node into source_node by attaching
            //   node destination_node's adjacency list to node source_node
            auto destination_edges = edges(destination_node);
            for (auto it = destination_edges.first; it != destination_edges.second; ++it)
            {
                InsertEdge(source_node, it->second.first, it->second.second);
            }
            // Scan adjacency lists, and replace node destination_node reference with source_node
            ReplaceNodeReference(destination_node, source_node);
            // Remove all self-loops in source_node's list
            RemoveSelfLoops(source_node);
            // Remove Node 2
            RemoveNode(destination_node);
        }
    private:
        T_Graph edges_;
        int internal_;

    };

};


TEST_F(SimpleGraphTests, AdjacencyList)
{
    auto start = std::chrono::steady_clock::now();
    auto g = Graph<int, double>();
    g.ReadAdjacencyList("kargerMinCut.txt", ListTypes::kNoWeights, &g);


    unsigned int n = g.node_label(1);
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

