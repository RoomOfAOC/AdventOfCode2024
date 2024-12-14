//
// Created by Harold on 2024/12/14.
//

#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <iostream>

#include "../utils/point2d.h"
#include "../utils/debug.h"

using point_t = Point2D<int>;

struct robot_t
{
    point_t p;
    point_t v;
};

std::ostream& operator<<(std::ostream& os, robot_t const& r)
{
    return os << "{Pos: " << r.p << ", Velocity: " << r.v << "}\n";
}

size_t part_1(char const* fn, int width, int height)
{
    std::ifstream ifs(fn);
    std::string line;
    int x{}, y{}, vx{}, vy{};
    std::vector<robot_t> robots;
    while (std::getline(ifs, line))
        if (std::sscanf(line.c_str(), "p=%d,%d v=%d,%d", &x, &y, &vx, &vy)) robots.push_back(robot_t{{x, y}, {vx, vy}});
    //std::cout << robots << '\n';

    auto move = [width, height](robot_t& r) {
        r.p.x = (r.p.x + r.v.x + width) % width;
        r.p.y = (r.p.y + r.v.y + height) % height;
    };

    for (auto i = 0; i < 100; i++)
        for (auto& r : robots)
            move(r);

    // left-top
    auto lt = std::count_if(robots.cbegin(), robots.cend(),
                            [width, height](auto const& r) { return r.p.x < width / 2 && r.p.y < height / 2; });
    // right-top
    auto rt = std::count_if(robots.cbegin(), robots.cend(),
                            [width, height](auto const& r) { return r.p.x > width / 2 && r.p.y < height / 2; });
    // left-bottom
    auto lb = std::count_if(robots.cbegin(), robots.cend(),
                            [width, height](auto const& r) { return r.p.x < width / 2 && r.p.y > height / 2; });
    // right-bottom
    auto rb = std::count_if(robots.cbegin(), robots.cend(),
                            [width, height](auto const& r) { return r.p.x > width / 2 && r.p.y > height / 2; });

    return lt * rt * lb * rb;
}

using set_t = std::unordered_set<point_t, point_t::Hasher>;

size_t part_2(char const* fn, int width, int height)
{
    std::ifstream ifs(fn);
    std::string line;
    int x{}, y{}, vx{}, vy{};
    std::vector<robot_t> robots;
    while (std::getline(ifs, line))
        if (std::sscanf(line.c_str(), "p=%d,%d v=%d,%d", &x, &y, &vx, &vy)) robots.push_back(robot_t{{x, y}, {vx, vy}});

    auto move = [width, height](robot_t& r) {
        r.p.x = (r.p.x + r.v.x + width) % width;
        r.p.y = (r.p.y + r.v.y + height) % height;
    };

    set_t rset;
    for (auto const& r : robots)
        rset.insert(r.p);

    size_t t = 0;
    while (rset.size() != robots.size())
    {
        rset.clear();
        for (auto& r : robots)
            move(r);
        for (auto const& r : robots)
            rset.insert(r.p);
        t++;
    }

    return t;
}

int main(int argc, char* argv[])
{
    std::cout << "What will the safety factor be after exactly 100 seconds have elapsed?\n"
              << part_1(argv[1], 101, 103) << std::endl;
    std::cout << "What is the fewest number of seconds that must elapse for the robots to display the Easter egg?\n"
              << part_2(argv[1], 101, 103) << std::endl;

    return 0;
}