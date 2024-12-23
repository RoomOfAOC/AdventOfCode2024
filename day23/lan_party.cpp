//
// Created by Harold on 2024/12/23.
//

#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <numeric>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bron_kerbosch_all_cliques.hpp>

#include "../utils/str_op.h"
#include "../utils/debug.h"

struct inter_connection_t
{
    inter_connection_t(std::string const& n1, std::string const& n2, std::string const& n3)
    {
        str[0] = n1;
        str[1] = n2;
        str[2] = n3;

        std::sort(str.begin(), str.end());
    }

    bool operator==(inter_connection_t const& other) const { return str == other.str; }

    struct hasher
    {
        size_t operator()(inter_connection_t const& ic) const
        {
            return std::hash<std::string>()(ic.str[0]) + std::hash<std::string>()(ic.str[1]) +
                   std::hash<std::string>()(ic.str[2]);
        }
    };

    bool contains_t() const { return str[0][0] == 't' || str[1][0] == 't' || str[2][0] == 't'; }

private:
    std::array<std::string, 3> str;
};

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;

    size_t sum = 0;
    std::unordered_map<std::string, std::unordered_set<std::string>> connections;
    while (std::getline(ifs, line))
    {
        auto n1 = line.substr(0, 2);
        auto n2 = line.substr(3);
        connections[n1].insert(n2);
        connections[n2].insert(n1);
    }
    std::unordered_set<inter_connection_t, inter_connection_t::hasher> inter_connections;
    for (auto const& [k, v] : connections)
        for (auto const& v1 : v)
            for (auto const& v2 : connections[v1])
                if (connections[v2].find(k) != connections[v2].end())
                    inter_connections.insert(inter_connection_t{k, v1, v2});
    size_t cnt = 0;
    for (auto const& ic : inter_connections)
        if (ic.contains_t()) cnt++;
    return cnt;
}

// boost interface is much more verbose than python networkx ...
using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;
using V = Graph::vertex_descriptor;
using Clique = std::deque<V>;
using Cliques = std::vector<Clique>;
std::string part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::unordered_map<std::string, std::unordered_set<std::string>> edges;
    while (std::getline(ifs, line))
    {
        auto n1 = line.substr(0, 2);
        auto n2 = line.substr(3);
        edges[n1].insert(n2);
    }

    Graph g;
    std::unordered_map<std::string, V> vertex_descriptors;
    std::unordered_map<V, std::string> dv;
    for (auto const& [v, _] : edges)
    {
        vertex_descriptors[v] = boost::add_vertex(g);
        dv[vertex_descriptors[v]] = v;
    }
    for (auto const& [v, vs] : edges)
        for (auto const& vv : vs)
            boost::add_edge(vertex_descriptors[v], vertex_descriptors[vv], g);

    Cliques cliques;
    struct
    {
        Cliques& target;
        void clique(std::deque<V> clique, Graph const&) const { target.push_back(std::move(clique)); }
    } collect{cliques};

    boost::bron_kerbosch_all_cliques(g, collect, 1);
    //std::cout << cliques << '\n';

    auto me = std::max_element(cliques.cbegin(), cliques.cend(),
                               [](auto const& c1, auto const& c2) { return c1.size() < c2.size(); });
    std::vector<std::string> res(me->size());
    std::transform(me->cbegin(), me->cend(), res.begin(), [&dv](auto const i) { return dv[i]; });
    std::sort(res.begin(), res.end());

    return std::reduce(res.cbegin(), res.cend(), std::string{},
                       [](auto const& s, auto const& r) { return s.empty() ? r : s + ',' + r; });
}

int main(int argc, char* argv[])
{
    std::cout << "How many contain at least one computer with a name that starts with t?\n"
              << part_1(argv[1]) << std::endl;
    std::cout << "What is the password to get into the LAN party?\n" << part_2(argv[1]) << std::endl;

    return 0;
}