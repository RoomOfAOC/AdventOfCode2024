//
// Created by Harold on 2024/12/13.
//

#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <iostream>

#include "../utils/point2d.h"
#include "../utils/debug.h"

using point_t = Point2D<long long>;

struct machine_t
{
    point_t A;
    point_t B;
    point_t P;
};

std::ostream& operator<<(std::ostream& os, machine_t const& m)
{
    return os << "{ Button A: " << m.A << ", Button B: " << m.B << ", Prize: " << m.P << " }\n";
}

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    int x{}, y{};
    std::vector<machine_t> machines;
    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;

        if (std::sscanf(line.c_str(), "Button A: X+%d, Y+%d", &x, &y))
            machines.push_back(machine_t{{x, y}});
        else if (std::sscanf(line.c_str(), "Button B: X+%d, Y+%d", &x, &y))
            machines.back().B = point_t{x, y};
        else if (std::sscanf(line.c_str(), "Prize: X=%d, Y=%d", &x, &y))
            machines.back().P = point_t{x, y};
    }
    //std::cout << machines << '\n';

    size_t cnt = 0;
    for (auto const& m : machines)
    {
        auto ra = m.P ^ m.B;
        auto la = m.A ^ m.B;
        auto rb = m.P ^ m.A;
        auto lb = -la;

        if (ra % la == 0 && rb % lb == 0)
        {
            // if (auto a = ra / la; a > 0 && a <= 100)
            // {
            //     auto b = rb / lb;
            //     if (b > 0 && b < 100) cnt += a * 3 + b;
            // }
            cnt += ra / la * 3 + rb / lb;
        }
    }

    return cnt;
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    int x{}, y{};
    std::vector<machine_t> machines;
    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;

        if (std::sscanf(line.c_str(), "Button A: X+%d, Y+%d", &x, &y))
            machines.push_back(machine_t{{x, y}});
        else if (std::sscanf(line.c_str(), "Button B: X+%d, Y+%d", &x, &y))
            machines.back().B = point_t{x, y};
        else if (std::sscanf(line.c_str(), "Prize: X=%d, Y=%d", &x, &y))
            machines.back().P = point_t{x, y};
    }

    size_t cnt = 0;
    for (auto& m : machines)
    {
        m.P += 10000000000000;

        auto ra = m.P ^ m.B;
        auto la = m.A ^ m.B;
        auto rb = m.P ^ m.A;
        auto lb = -la;

        if (ra % la == 0 && rb % lb == 0)
        {
            // if (auto a = ra / la; a > 100)
            // {
            //     auto b = rb / lb;
            //     if (b > 100) cnt += a * 3 + b;
            // }
            cnt += ra / la * 3 + rb / lb;
        }
    }

    return cnt;
}

int main(int argc, char* argv[])
{
    std::cout << "What is the fewest tokens you would have to spend to win all possible prizes?\n"
              << part_1(argv[1]) << std::endl;
    std::cout << "What is the fewest tokens you would have to spend to win all possible prizes?\n"
              << part_2(argv[1]) << std::endl;

    return 0;
}