//
// Created by Harold on 2024/12/11.
//

#include <fstream>
#include <list>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "../utils/str_op.h"
#include "../utils/debug.h"

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::list<size_t> stones(std::istream_iterator<size_t>{ifs}, std::istream_iterator<size_t>{});

    for (auto i = 0; i < 25; i++)
        for (auto it = stones.begin(); it != stones.end(); it++)
        {
            if (*it == 0)
            {
                *it = 1;
                continue;
            }
            int dn = std::log10(*it) + 1;
            if (dn % 2 == 0)
            {
                size_t base = std::pow(10, dn / 2);
                stones.insert(it, *it / base);
                *it %= base;
                continue;
            }
            *it *= 2024;
        }

    return stones.size();
}

struct state_t
{
    size_t stone;
    int blinks;

    bool operator==(state_t const& other) const { return stone == other.stone && blinks == other.blinks; }
    struct hasher
    {
        size_t operator()(state_t const& s) const
        {
            return std::hash<size_t>()(s.stone) ^ (std::hash<int>()(s.blinks) << 1);
        }
    };
};
// state_t : acc
using map_t = std::unordered_map<state_t, size_t, state_t::hasher>;
size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::list<size_t> stones(std::istream_iterator<size_t>{ifs}, std::istream_iterator<size_t>{});
    map_t states;

    auto rec = [&states](size_t stone, int blinks, auto&& rec) {
        if (blinks == 0) return size_t{1};

        state_t cur{stone, blinks};
        if (states.find(cur) != states.end()) return states[cur];

        size_t cnt = 0;

        if (stone == 0)
            cnt = rec(1, blinks - 1, rec);
        else
        {
            int dn = std::log10(stone) + 1;
            if (dn % 2 == 0)
            {
                size_t base = std::pow(10, dn / 2);
                cnt = rec(stone / base, blinks - 1, rec) + rec(stone % base, blinks - 1, rec);
            }
            else
                cnt = rec(stone * 2024, blinks - 1, rec);
        }
        states[cur] = cnt;
        return cnt;
    };

    return std::reduce(stones.cbegin(), stones.cend(), size_t{},
                       [&rec](size_t acc, size_t s) { return acc + rec(s, 75, rec); });
}

int main(int argc, char* argv[])
{
    std::cout << "How many stones will you have after blinking 25 times?\n" << part_1(argv[1]) << std::endl;
    std::cout << "How many stones would you have after blinking a total of 75 times?\n" << part_2(argv[1]) << std::endl;

    return 0;
}