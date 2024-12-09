//
// Created by Harold on 2024/12/9.
//

#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "../utils/debug.h"

struct FS
{
    int id{0};
    int file{0};
    int space{0};
    std::vector<int> seq;
};
std::ostream& operator<<(std::ostream& os, FS const& fs)
{
    return os << "<id: " << fs.id << ", file: " << fs.file << ", space: " << fs.space << fs.seq << ">";
}

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    int id = 0;
    std::vector<FS> blocks;
    for (auto it = std::istream_iterator<char>{ifs}; it != std::istream_iterator<char>{}; it++)
    {
        if (id % 2 == 0)
            blocks.push_back(FS{id / 2, *it - '0', 0});
        else
            blocks.back().space = *it - '0';
        id++;
    }
    //std::cout << blocks << '\n';
    for (auto it = blocks.rbegin(), it1 = blocks.rend(); it != it1;)
    {
        if (it1.base()->space >= it->file)
        {
            it1.base()->space -= it->file;
            for (auto i = it->file; i != 0; i--)
                it1.base()->seq.push_back(it->id);
            it->file = 0;
            it++;
        }
        else
        {
            for (auto i = 0; i != it1.base()->space; i++)
                it1.base()->seq.push_back(it->id);
            it->file -= it1.base()->space;
            it1.base()->space = 0;
            it1--;
        }
    }
    //std::cout << blocks << '\n';
    std::vector<int> seq;
    for (auto it = blocks.cbegin(); it != blocks.cend(); it++)
    {
        for (auto i = 0; i < it->file; i++)
            seq.push_back(it->id);
        if (!it->seq.empty()) seq.insert(seq.end(), it->seq.cbegin(), it->seq.cend());
    }
    //std::cout << seq << '\n';
    size_t res = 0;
    for (size_t i = 0; i < seq.size(); i++)
        res += i * seq[i];

    return res;
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    int id = 0;
    std::vector<FS> blocks;
    for (auto it = std::istream_iterator<char>{ifs}; it != std::istream_iterator<char>{}; it++)
    {
        if (id % 2 == 0)
            blocks.push_back(FS{id / 2, *it - '0', 0});
        else
            blocks.back().space = *it - '0';
        id++;
    }
    //std::cout << blocks << '\n';
    for (auto it = blocks.rbegin(); it != blocks.rend(); it++)
    {
        for (auto it1 = blocks.begin(); it1 != it.base(); it1++)
            if (it1->space >= it->file)
            {
                it1->space -= it->file;
                for (auto i = it->file; i != 0; i--)
                {
                    it1->seq.push_back(it->id);

                    //it->seq.insert(it->seq.begin(), 0);
                    it->seq.push_back(0);
                }
                std::rotate(it->seq.rbegin(), it->seq.rbegin() + it->file, it->seq.rend());

                it->space += it->file;
                it->file = 0;
                break;
            }
    }
    //std::cout << blocks << '\n';
    std::vector<int> seq;
    for (auto it = blocks.cbegin(); it != blocks.cend(); it++)
    {
        for (auto i = 0; i < it->file; i++)
            seq.push_back(it->id);
        if (!it->seq.empty()) seq.insert(seq.end(), it->seq.cbegin(), it->seq.cend());
        for (auto i = 0; i < it->space - std::count(it->seq.cbegin(), it->seq.cend(), 0); i++)
            seq.push_back(0);
    }
    //std::cout << seq << '\n';
    size_t res = 0;
    for (size_t i = 0; i < seq.size(); i++)
        res += i * seq[i];

    return res;
}

int main(int argc, char* argv[])
{
    std::cout << "What is the resulting filesystem checksum?\n" << part_1(argv[1]) << std::endl;
    std::cout << "What is the resulting filesystem checksum?\n" << part_2(argv[1]) << std::endl;

    return 0;
}