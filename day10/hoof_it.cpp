//
// Created by Harold on 2024/12/10.
//

#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "../utils/mat.h"
#include "../utils/debug.h"

using map_t = Mat<int>;
using set_t = std::unordered_set<Point, Point::PointHasher>;

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    map_t m;
    read_all(ifs, m);
    std::vector<Point> neighbors;
    neighbors.reserve(4);
    set_t peaks;
    size_t cnt = 0;
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
            if (m(i, j) == 0)
            {
                Point cur{i, j};
                std::queue<Point> q;
                q.push(cur);
                peaks.clear();
                while (!q.empty())
                {
                    cur = q.front();
                    q.pop();
                    if (m(cur) == 9) peaks.insert(cur);
                    neighbors.clear();
                    m.find_4_neighbors(cur, neighbors);
                    if (neighbors.empty()) continue;
                    for (auto n : neighbors)
                        if (m(n) - m(cur) == 1) q.push(n);
                }
                cnt += peaks.size();
            }

    return cnt;
}

struct state_t
{
    Point pos;
    std::vector<Point> path;
};

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    map_t m;
    read_all(ifs, m);
    std::vector<Point> neighbors;
    neighbors.reserve(4);
    std::vector<std::vector<Point>> paths;
    size_t cnt = 0;
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
            if (m(i, j) == 0)
            {
                state_t cur{};
                cur.pos = Point(i, j);
                cur.path.reserve(9);
                cur.path.push_back(cur.pos);
                std::queue<state_t> q;
                q.push(cur);
                paths.clear();
                while (!q.empty())
                {
                    cur = q.front();
                    q.pop();
                    if (m(cur.pos) == 9) paths.push_back(cur.path);
                    neighbors.clear();
                    m.find_4_neighbors(cur.pos, neighbors);
                    if (neighbors.empty()) continue;
                    for (auto n : neighbors)
                        if (m(n) - m(cur.pos) == 1)
                        {
                            auto nxt = cur;
                            nxt.pos = n;
                            nxt.path.push_back(n);
                            q.push(nxt);
                        }
                }
                cnt += paths.size();
            }
    return cnt;
}

int main(int argc, char* argv[])
{
    std::cout << "What is the sum of the scores of all trailheads on your topographic map?\n"
              << part_1(argv[1]) << std::endl;
    std::cout << "What is the sum of the ratings of all trailheads?\n" << part_2(argv[1]) << std::endl;

    return 0;
}