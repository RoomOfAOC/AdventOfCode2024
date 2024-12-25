//
// Created by Harold on 2024/12/25.
//

#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <iostream>

#include "../utils/debug.h"

unsigned long long part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;

    std::vector<std::array<int, 5>> locks, keys;
    std::vector<std::string> block;
    block.reserve(7);
    auto parse_block = [&locks, &keys](std::vector<std::string> const& block) {
        std::array<int, 5> lk{};
        for (auto i = 1; i < 6; i++)
            for (auto j = 0; j < 5; j++)
                lk[j] += (block[i][j] == '#');
        if (block[0] == "#####")
            locks.push_back(lk);
        else
            keys.push_back(lk);
    };
    while (std::getline(ifs, line))
    {
        if (line.empty())
        {
            parse_block(block);
            block.clear();
            continue;
        }
        block.push_back(line);
    }
    if (!block.empty()) parse_block(block);

    //std::cout << locks << "\n\n" << keys << '\n';

    size_t cnt = 0;
    for (auto const& l : locks)
        for (auto const& k : keys)
        {
            bool all_fit = true;
            for (auto i = 0; i < 5; i++)
                if (l[i] + k[i] > 5)
                {
                    all_fit = false;
                    break;
                }
            if (all_fit) cnt++;
        }

    return cnt;
}

int main(int argc, char* argv[])
{
    std::cout << "How many unique lock/key pairs fit together without overlapping in any column?\n"
              << part_1(argv[1]) << std::endl;

    return 0;
}