//
// Created by Harold on 2024/12/8.
//

#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <iostream>

#include "../utils/mat.h"
#include "../utils/debug.h"

using map_t = Mat<char>;
using antenna_map_t = std::unordered_map<char, std::vector<Point>>;
using point_set_t = std::unordered_set<Point, Point::PointHasher>;
using point_map_t = std::unordered_map<Point, std::unordered_set<Point, Point::PointHasher>, Point::PointHasher>;

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    map_t m;
    read_all(ifs, m);
    antenna_map_t as;
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
            if (m(i, j) != '.') as[m(i, j)].push_back(Point(i, j));
    //std::cout << m << as << '\n';
    point_set_t ps;
    for (auto const& [k, v] : as)
    {
        if (v.size() < 2) continue;
        for (auto i = 0; i < v.size() - 1; i++)
            for (auto j = i + 1; j < v.size(); j++)
            {
                auto dir = v[i] - v[j];
                auto p1 = v[i] + dir;
                auto p2 = v[j] - dir;
                //std::cout << v[i] << ", " << v[j] << ", " << dir << ", " << p1 << ", " << p2 << '\n';
                if (m.valid_pos(p1)) ps.insert(p1);
                if (m.valid_pos(p2)) ps.insert(p2);
            }
    }

    return ps.size();
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    map_t m;
    read_all(ifs, m);
    antenna_map_t as;
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
            if (m(i, j) != '.') as[m(i, j)].push_back(Point(i, j));
    point_set_t ps;
    for (auto const& [k, v] : as)
    {
        if (v.size() < 2) continue;
        for (auto i = 0; i < v.size() - 1; i++)
            for (auto j = i + 1; j < v.size(); j++)
            {
                auto dir = v[i] - v[j];
                auto p1 = v[i];
                auto p2 = v[j];
                while (m.valid_pos(p1))
                {
                    ps.insert(p1);
                    p1 += dir;
                }
                while (m.valid_pos(p2))
                {
                    ps.insert(p2);
                    p2 -= dir;
                }
            }
    }
    return ps.size();
}

int main(int argc, char* argv[])
{
    std::cout << "How many unique locations within the bounds of the map contain an antinode?\n"
              << part_1(argv[1]) << std::endl;
    std::cout << "How many unique locations within the bounds of the map contain an antinode?\n"
              << part_2(argv[1]) << std::endl;

    return 0;
}