//
// Created by Harold on 2024/12/7.
//

#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

#include "../utils/str_op.h"
#include "../utils/debug.h"

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<unsigned> nums;
    auto rec = [&nums](size_t target, size_t acc, size_t index, auto&& rec) {
        if (index == nums.size()) return acc == target;
        if (acc > target) return false;
        return rec(target, acc + nums[index], index + 1, rec) || rec(target, acc * nums[index], index + 1, rec);
    };
    size_t sum = 0;
    while (std::getline(ifs, line))
    {
        auto p = line.find(':');
        auto t = parse_num<size_t>(line.substr(0, p));
        nums.clear();
        for (auto const& n : split(line.substr(p + 1), " "))
            nums.push_back(parse_num<unsigned>(n));
        if (rec(t, nums[0], 1, rec))
        {
            //std::cout << t << ", " << nums << '\n';
            sum += t;
        }
    }
    return sum;
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<unsigned> nums;
    auto rec = [&nums](size_t target, size_t acc, size_t index, auto&& rec) {
        if (index == nums.size()) return acc == target;
        if (acc > target) return false;
        return rec(target, acc + nums[index], index + 1, rec) || rec(target, acc * nums[index], index + 1, rec) ||
               rec(target, parse_num<size_t>(std::to_string(acc) + std::to_string(nums[index])), index + 1, rec);
    };
    size_t sum = 0;
    while (std::getline(ifs, line))
    {
        auto p = line.find(':');
        auto t = parse_num<size_t>(line.substr(0, p));
        nums.clear();
        for (auto const& n : split(line.substr(p + 1), " "))
            nums.push_back(parse_num<unsigned>(n));
        if (rec(t, nums[0], 1, rec)) sum += t;
    }
    return sum;
}

int main(int argc, char* argv[])
{
    std::cout << "What is their total calibration result?\n" << part_1(argv[1]) << std::endl;
    std::cout << "What is their total calibration result?\n" << part_2(argv[1]) << std::endl;

    return 0;
}