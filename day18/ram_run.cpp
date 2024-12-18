//
// Created by Harold on 2024/12/18.
//

#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "../utils/mat.h"
#include "../utils/str_op.h"
#include "../utils/debug.h"

using map_t = Mat<char>;

struct state_t
{
    Point pos{};
    int steps{};

    bool operator<(state_t const& other) const { return steps > other.steps; }
};

size_t part_1(char const* fn, int width, int height, int bytes)
{
    std::ifstream ifs(fn);
    std::string line;
    int x{}, y{};
    map_t m(width, height, '.');
    for (auto i = 0; i < bytes; i++)
    {
        std::getline(ifs, line);
        auto c = line.find(',');
        x = parse_num<int>(line.substr(0, c));
        y = parse_num<int>(line.substr(c + 1));
        m(y, x) = '#';
    }

    //std::cout << m << '\n';

    Point start_pos(0, 0), end_pos(width - 1, height - 1);
    std::unordered_set<Point, Point::PointHasher> visited;
    std::priority_queue<state_t> pq;
    pq.push(state_t{start_pos, 0});
    std::vector<Point> neighbors;
    neighbors.reserve(4);
    while (!pq.empty())
    {
        auto state = pq.top();
        pq.pop();

        if (state.pos == end_pos) return state.steps;

        if (visited.find(state.pos) != visited.end()) continue;

        visited.insert(state.pos);

        neighbors.clear();
        m.find_4_neighbors(state.pos, neighbors);
        for (auto const& n : neighbors)
            if (m(n) == '.' && visited.find(n) == visited.end()) pq.push(state_t{n, state.steps + 1});
    }

    return 0;
}

std::string part_2(char const* fn, int width, int height, int bytes)
{
    std::ifstream ifs(fn);
    std::string line;
    int x{}, y{};
    map_t m(width, height, '.');
    for (auto i = 0; i < bytes; i++)
    {
        std::getline(ifs, line);
        auto c = line.find(',');
        x = parse_num<int>(line.substr(0, c));
        y = parse_num<int>(line.substr(c + 1));
        m(y, x) = '#';
    }

    Point start_pos(0, 0), end_pos(width - 1, height - 1);
    std::unordered_set<Point, Point::PointHasher> visited;
    std::deque<Point> q;
    std::vector<Point> neighbors;
    neighbors.reserve(4);

    auto can_pass = [&]() {
        visited.clear();
        q.clear();

        q.push_back(start_pos);
        while (!q.empty())
        {
            auto pos = q.front();
            q.pop_front();

            if (pos == end_pos) return true;

            if (visited.find(pos) != visited.end()) continue;

            visited.insert(pos);

            neighbors.clear();
            m.find_4_neighbors(pos, neighbors);
            for (auto const& n : neighbors)
                if (m(n) == '.' && visited.find(n) == visited.end()) q.push_back(n);
        }
        return false;
    };

    while (std::getline(ifs, line))
    {
        auto c = line.find(',');
        x = parse_num<int>(line.substr(0, c));
        y = parse_num<int>(line.substr(c + 1));
        m(y, x) = '#';
        if (!can_pass()) return std::to_string(x) + "," + std::to_string(y);
    }

    return {};
}

int main(int argc, char* argv[])
{
    std::cout << "what is the minimum number of steps needed to reach the exit?\n"
              << part_1(argv[1], 71, 71, 1024) << std::endl;
    std::cout
        << "What are the coordinates of the first byte that will prevent the exit from being reachable from your starting position?\n"
        << part_2(argv[1], 71, 71, 1024) << std::endl;

    return 0;
}