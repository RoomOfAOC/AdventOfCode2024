//
// Created by Harold on 2024/12/12.
//

#include <fstream>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "../utils/mat.h"
#include "../utils/debug.h"

using map_t = Mat<char>;
using vec_t = std::vector<Point>;
using set_t = std::unordered_set<Point, Point::PointHasher>;
struct region_t
{
    char plant;
    set_t blocks;
};
std::ostream& operator<<(std::ostream& os, region_t const& r)
{
    return os << "{" << r.plant << ", " << r.blocks << "}";
}

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    map_t m;
    read_all(ifs, m);

    set_t visited;

    auto get_region = [&m, &visited](Point cur) {
        region_t region{m(cur), {cur}};
        vec_t q;
        q.push_back(cur);
        vec_t neighbors;
        neighbors.reserve(4);
        while (!q.empty())
        {
            auto cur = q.back();
            q.pop_back();
            if (visited.find(cur) != visited.end()) continue;
            visited.insert(cur);
            neighbors.clear();
            m.find_4_neighbors(cur, neighbors);
            for (auto n : neighbors)
                if (m(n) == m(cur))
                {
                    q.push_back(n);
                    region.blocks.insert(n);
                }
        }
        return region;
    };

    std::vector<region_t> regions;
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
        {
            Point cur(i, j);
            if (visited.find(cur) == visited.end()) regions.push_back(get_region(cur));
        }
    //std::cout << regions << '\n';

    auto get_price = [&m](region_t const& r) {
        vec_t neighbors;
        neighbors.reserve(4);
        size_t perimeter = 0;
        for (auto const& p : r.blocks)
        {
            neighbors.clear();
            m.find_4_neighbors(p, neighbors);
            perimeter +=
                4 - neighbors.size() +
                std::count_if(neighbors.begin(), neighbors.end(), [&m, &p](Point const& n) { return m(n) != m(p); });
        }
        return r.blocks.size() * perimeter;
    };

    return std::reduce(regions.cbegin(), regions.cend(), size_t{0},
                       [&](auto acc, auto const& r) { return acc + get_price(r); });
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    map_t m;
    read_all(ifs, m);

    set_t visited;

    auto get_region = [&m, &visited](Point cur) {
        region_t region{m(cur), {cur}};
        vec_t q;
        q.push_back(cur);
        vec_t neighbors;
        neighbors.reserve(4);
        while (!q.empty())
        {
            auto cur = q.back();
            q.pop_back();
            if (visited.find(cur) != visited.end()) continue;
            visited.insert(cur);
            neighbors.clear();
            m.find_4_neighbors(cur, neighbors);
            for (auto n : neighbors)
                if (m(n) == m(cur))
                {
                    q.push_back(n);
                    region.blocks.insert(n);
                }
        }
        return region;
    };

    std::vector<region_t> regions;
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
        {
            Point cur(i, j);
            if (visited.find(cur) == visited.end()) regions.push_back(get_region(cur));
        }

    // TODO: very naive... try some graph algo?
    auto get_price = [&m](region_t const& r) {
        Point left_up(m.rows, m.cols), right_bottom(0, 0);
        for (auto const& p : r.blocks)
        {
            if (p.x < left_up.x) left_up.x = p.x;
            if (p.y < left_up.y) left_up.y = p.y;
            if (p.x > right_bottom.x) right_bottom.x = p.x;
            if (p.y > right_bottom.y) right_bottom.y = p.y;
        }
        auto nrows = right_bottom.x - left_up.x + 1 + 2;
        auto ncols = right_bottom.y - left_up.y + 1 + 2;
        map_t mm(nrows, ncols, '.');
        for (auto const& p : r.blocks)
            mm(p.x - left_up.x + 1, p.y - left_up.y + 1) = 'O';
        //std::cout << mm << '\n';

        size_t side = 0;
        std::string seq;
        seq.reserve(std::max(nrows, ncols));

        // up down
        for (auto i = 1; i < nrows - 1; i++)
        {
            seq.clear();
            for (auto j = 1; j < ncols - 1; j++)
                seq += (mm(i, j) == 'O' && mm(i, j) != mm(i - 1, j)) ? 'O' : ' ';
            //std::cout << seq << '\n';
            seq.erase(std::unique(seq.begin(), seq.end()), seq.end());
            //std::cout << seq << '\n';
            side += std::count(seq.begin(), seq.end(), 'O');

            seq.clear();
            for (auto j = 1; j < ncols - 1; j++)
                seq += (mm(i, j) == 'O' && mm(i, j) != mm(i + 1, j)) ? 'O' : ' ';
            //std::cout << seq << '\n';
            seq.erase(std::unique(seq.begin(), seq.end()), seq.end());
            //std::cout << seq << '\n';
            side += std::count(seq.begin(), seq.end(), 'O');
        }
        //std::cout << side << '\n';

        // // left right
        // for (auto i = 1; i < ncols - 1; i++)
        // {
        //     seq.clear();
        //     for (auto j = 1; j < nrows - 1; j++)
        //         seq += (mm(j, i) == 'O' && mm(j, i) != mm(j, i - 1)) ? 'O' : ' ';
        //     //std::cout << seq << '\n';
        //     seq.erase(std::unique(seq.begin(), seq.end()), seq.end());
        //     //std::cout << seq << '\n';
        //     side += std::count(seq.begin(), seq.end(), 'O');

        //     seq.clear();
        //     for (auto j = 1; j < nrows - 1; j++)
        //         seq += (mm(j, i) == 'O' && mm(j, i) != mm(j, i + 1)) ? 'O' : ' ';
        //     //std::cout << seq << '\n';
        //     seq.erase(std::unique(seq.begin(), seq.end()), seq.end());
        //     //std::cout << seq << '\n';
        //     side += std::count(seq.begin(), seq.end(), 'O');
        // }
        // //std::cout << side << '\n';
        // return r.blocks.size() * side;

        return r.blocks.size() * side * 2;
    };

    return std::reduce(regions.cbegin(), regions.cend(), size_t{0},
                       [&](auto acc, auto const& r) { return acc + get_price(r); });
}

int main(int argc, char* argv[])
{
    std::cout << "What is the total price of fencing all regions on your map?\n" << part_1(argv[1]) << std::endl;
    std::cout << "What is the new total price of fencing all regions on your map?\n" << part_2(argv[1]) << std::endl;

    return 0;
}