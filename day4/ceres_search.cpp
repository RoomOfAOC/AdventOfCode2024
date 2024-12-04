//
// Created by Harold on 2024/12/4.
//

#include <fstream>
#include <string>
#include <array>
#include <iostream>

#include "../utils/mat.h"

using map_t = Mat<char>;

unsigned count_xmas(map_t const& m, int i, int j)
{
    static std::array<std::array<Point, 3>, 8> dirs{
        std::array<Point, 3>{Point{-1, 0}, {-2, 0}, {-3, 0}},    std::array<Point, 3>{Point{1, 0}, {2, 0}, {3, 0}},
        std::array<Point, 3>{Point{0, 1}, {0, 2}, {0, 3}},       std::array<Point, 3>{Point{0, -1}, {0, -2}, {0, -3}},
        std::array<Point, 3>{Point{-1, -1}, {-2, -2}, {-3, -3}}, std::array<Point, 3>{Point{-1, 1}, {-2, 2}, {-3, 3}},
        std::array<Point, 3>{Point{1, -1}, {2, -2}, {3, -3}},    std::array<Point, 3>{Point{1, 1}, {2, 2}, {3, 3}},
    };
    unsigned cnt = 0;
    Point pos(i, j);
    std::string xmas;
    xmas.reserve(3);
    for (auto const& d : dirs)
    {
        xmas.clear();
        for (auto p : d)
        {
            auto pos_n = pos + p;
            if (!m.valid_pos(pos_n))
                break;
            else
                xmas += m(pos_n);
        }
        cnt += (xmas == "MAS");
    }
    return cnt;
}

unsigned count_x_mas(map_t const& m, int i, int j)
{
    static std::array<std::array<Point, 2>, 2> dirs{
        std::array<Point, 2>{Point{-1, -1}, {1, 1}},
        std::array<Point, 2>{Point{-1, 1}, {1, -1}},
    };
    unsigned cnt = 0;
    Point pos(i, j);
    std::string x_mas;
    x_mas.reserve(2);
    for (auto const& d : dirs)
    {
        x_mas.clear();
        for (auto p : d)
        {
            auto pos_n = pos + p;
            if (!m.valid_pos(pos_n))
                break;
            else
                x_mas += m(pos_n);
        }
        cnt += (x_mas == "MS" || x_mas == "SM");
    }
    return (cnt == 2);
}

long part_1(char const* fn)
{
    long res = 0;
    std::ifstream ifs(fn);
    map_t m;
    read_all(ifs, m);
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
            if (m(i, j) == 'X') res += count_xmas(m, i, j);
    return res;
}

long part_2(char const* fn)
{
    long res = 0;
    std::ifstream ifs(fn);
    map_t m;
    read_all(ifs, m);
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
            if (m(i, j) == 'A') res += count_x_mas(m, i, j);
    return res;
}

int main(int argc, char* argv[])
{
    std::cout << "How many times does XMAS appear?\n" << part_1(argv[1]) << std::endl;
    std::cout << "How many times does an X-MAS appear?\n" << part_2(argv[1]) << std::endl;

    return 0;
}