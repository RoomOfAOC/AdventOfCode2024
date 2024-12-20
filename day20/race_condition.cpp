//
// Created by Harold on 2024/12/20.
//

#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <iostream>

#include "../utils/mat.h"
#include "../utils/debug.h"

using map_t = Mat<char>;

void parse_map_and_start_end(std::ifstream& ifs, Mat<char>& mat, Point& start, Point& end)
{
    std::string line;
    char c;
    int rows = 0, cols = 0;
    while (std::getline(ifs, line) && !line.empty())
    {
        cols = 0;
        for (auto c : line)
        {
            mat.data.push_back(c);
            if (c == 'S')
            {
                start.x = rows;
                start.y = cols;
            }
            else if (c == 'E')
            {
                end.x = rows;
                end.y = cols;
            }
            cols++;
        }
        rows++;
    }
    mat.rows = rows;
    mat.cols = cols;
    mat.data.resize(mat.rows * mat.cols, 0);
}

struct state_t
{
    Point pos{};
    int steps{};

    bool operator<(state_t const& other) const { return steps > other.steps; }
};

int shortest_distance(Point const& p1, Point const& p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

int shortest_path(map_t const& m, Point const& start, Point const& end)
{
    std::vector<Point> neighbors;
    neighbors.reserve(4);
    std::unordered_set<Point, Point::PointHasher> visited;
    std::priority_queue<state_t> pq;
    pq.push(state_t{start, 0});
    while (!pq.empty())
    {
        auto state = pq.top();
        pq.pop();

        if (state.pos == end) return state.steps;

        if (visited.find(state.pos) != visited.end()) continue;

        visited.insert(state.pos);

        neighbors.clear();
        m.find_4_neighbors(state.pos, neighbors);
        for (auto const& n : neighbors)
            if (m(n) != '#' && visited.find(n) == visited.end()) pq.push(state_t{n, state.steps + 1});
    }
    return std::numeric_limits<int>::max();
}

size_t part_1(char const* fn, int diff)
{
    std::ifstream ifs(fn);
    map_t m;
    Point start, end;
    parse_map_and_start_end(ifs, m, start, end);

    auto shortest = shortest_path(m, start, end);

    //std::cout << shortest << '\n';

    auto threshold = shortest - diff;
    size_t cnt = 0;
    std::vector<Point> neighbors;
    neighbors.reserve(4);
    for (auto i = 1; i < m.rows - 1; i++)
        for (auto j = 1; j < m.cols - 1; j++)
            if (m(i, j) == '#')
            {
                Point cs(i, j);
                auto d = shortest_distance(cs, start) + shortest_distance(cs, end);
                if (d >= threshold) continue;

                neighbors.clear();
                if (m.valid_pos(i - 1, j) && m(i - 1, j) != '#') neighbors.push_back(Point(i - 1, j));
                if (m.valid_pos(i + 1, j) && m(i + 1, j) != '#') neighbors.push_back(Point(i + 1, j));
                if (m.valid_pos(i, j - 1) && m(i, j - 1) != '#') neighbors.push_back(Point(i, j - 1));
                if (m.valid_pos(i, j + 1) && m(i, j + 1) != '#') neighbors.push_back(Point(i, j + 1));
                if (neighbors.size() < 2) continue;

                m(cs) = '.';
                if (auto d = shortest_path(m, start, end); d <= threshold) cnt++;
                m(cs) = '#';
            }

    return cnt;
}

using dist_map_t = std::unordered_map<Point, int, Point::PointHasher>;
dist_map_t dfs_distances(map_t const& m, Point const& start, Point const& end)
{
    dist_map_t dist_map;
    std::queue<state_t> q;
    q.push({start, 0});
    std::vector<Point> neighbors;
    neighbors.reserve(4);
    while (!q.empty())
    {
        auto state = q.front();
        q.pop();

        if (dist_map.find(state.pos) != dist_map.end()) continue;

        dist_map[state.pos] = state.steps;

        if (state.pos == end) continue;

        neighbors.clear();
        m.find_4_neighbors(state.pos, neighbors);
        for (auto const& n : neighbors)
            if (m(n) != '#') q.push({n, state.steps + 1});
    }
    return dist_map;
}

struct jump_t
{
    Point p1;
    Point p2;

    bool operator==(jump_t const& other) const
    {
        return (p1 == other.p1 && p2 == other.p2) || (p1 == other.p2 && p2 == other.p1);
    }
    struct hasher
    {
        size_t operator()(jump_t const& s) const { return Point::PointHasher()(s.p1) + Point::PointHasher()(s.p2); }
    };
};
size_t part_2(char const* fn, int diff, int max_p2p_sd)
{
    std::ifstream ifs(fn);
    map_t m;
    Point start, end;
    parse_map_and_start_end(ifs, m, start, end);

    auto dist_map = dfs_distances(m, start, end);
    //std::cout << dist_map << '\n';

    size_t cnt = 0;
    std::unordered_set<jump_t, jump_t::hasher> combs;
    for (auto [p1, d1] : dist_map)
        for (auto [p2, d2] : dist_map)
        {
            if (p1 == p2) continue;

            jump_t j{p1, p2};
            if (combs.find(j) != combs.end()) continue;
            combs.insert(j);

            auto sd = shortest_distance(p1, p2);
            if ((sd <= max_p2p_sd) && (abs(d1 - d2) >= sd + diff)) cnt++;
        }

    return cnt;
}

// part_1(argv[1], 100) == part_2(argv[1], 100, 2)

int main(int argc, char* argv[])
{
    std::cout << "How many cheats would save you at least 100 picoseconds?\n" << part_1(argv[1], 100) << std::endl;
    std::cout << "How many cheats would save you at least 100 picoseconds?\n" << part_2(argv[1], 100, 20) << std::endl;

    return 0;
}