//
// Created by Harold on 2024/12/6.
//

#include <fstream>
#include <string>
#include <array>
#include <unordered_set>
#include <algorithm>
#include <iostream>

#include "../utils/mat.h"

using map_t = Mat<char>;

struct state_t
{
    Point pos{};
    int dir{};

    bool operator==(state_t const& other) const { return pos == other.pos && dir == other.dir; }
    struct state_hasher
    {
        size_t operator()(state_t const& s) const { return Point::PointHasher()(s.pos) ^ std::hash<int>()(s.dir); }
    };
};

using set_t = std::unordered_set<Point, Point::PointHasher>;
using state_set_t = std::unordered_set<state_t, state_t::state_hasher>;

long part_1(char const* fn)
{
    static std::array<Point, 4> dirs{
        Point{-1, 0},
        {0, 1},
        {1, 0},
        {0, -1},
    };

    std::ifstream ifs(fn);
    map_t m;
    read_all(ifs, m);

    state_t s;
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
            if (m(i, j) == '^') s.pos = Point(i, j);

    set_t dp;
    while (m.valid_pos(s.pos))
    {
        if (m(s.pos) == '#')
        {
            s.pos -= dirs[s.dir];
            s.dir = (s.dir + 1) % dirs.size();
        }
        dp.insert(s.pos);
        s.pos += dirs[s.dir];
    }
    return dp.size();
}

long part_2(char const* fn)
{
    static std::array<Point, 4> dirs{
        Point{-1, 0},
        {0, 1},
        {1, 0},
        {0, -1},
    };

    std::ifstream ifs(fn);
    map_t m;
    read_all(ifs, m);

    Point start_pos{};
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
            if (m(i, j) == '^') start_pos = Point(i, j);

    state_t s{start_pos, 0};
    set_t dp;
    while (m.valid_pos(s.pos))
    {
        if (m(s.pos) == '#')
        {
            s.pos -= dirs[s.dir];
            s.dir = (s.dir + 1) % dirs.size();
        }
        dp.insert(s.pos);
        s.pos += dirs[s.dir];
    }
    dp.erase(start_pos);

    state_set_t ss;
    int cnt = 0;
    for (auto const& p : dp)
    {
        m(p) = 'O';
        s.pos = start_pos;
        s.dir = 0;
        ss.clear();
        while (m.valid_pos(s.pos))
        {
            if (m(s.pos) == '#' || m(s.pos) == 'O')
            {
                s.pos -= dirs[s.dir];
                s.dir = (s.dir + 1) % dirs.size();
            }
            if (ss.find(s) != ss.end())
            {
                cnt++;
                break;
            }
            ss.insert(s);
            s.pos += dirs[s.dir];
        }
        m(p) = '.';
    }
    return cnt;
}

int main(int argc, char* argv[])
{
    std::cout << "How many distinct positions will the guard visit before leaving the mapped area?\n"
              << part_1(argv[1]) << std::endl;
    std::cout << "How many different positions could you choose for this obstruction?\n"
              << part_2(argv[1]) << std::endl;

    return 0;
}