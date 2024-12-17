//
// Created by Harold on 2024/12/17.
//

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <iostream>
#include <iomanip>

#include "../utils/str_op.h"
#include "../utils/debug.h"

std::string part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::getline(ifs, line);
    auto A = parse_num<int>(line.substr(12));
    std::getline(ifs, line);
    auto B = parse_num<int>(line.substr(12));
    std::getline(ifs, line);
    auto C = parse_num<int>(line.substr(12));
    std::getline(ifs, line);
    std::getline(ifs, line);
    std::vector<int> program;
    for (auto const& c : split(line.substr(9), ","))
        program.push_back(parse_num<int>(c));

    //std::cout << A << ", " << B << ", " << C << '\n' << program << '\n';

    std::vector<int> output;
    auto instruction_ptr = 0;
    while (instruction_ptr < program.size())
    {
        auto opcode = program[instruction_ptr];
        auto operand = program[instruction_ptr + 1];

        auto operand_value = operand;
        switch (operand)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            break;
        case 4:
            operand_value = A;
            break;
        case 5:
            operand_value = B;
            break;
        case 6:
            operand_value = C;
            break;
        case 7:
        default:
            throw "Combo operand 7 is reserved and will not appear in valid programs";
        }

        switch (opcode)
        {
        case 0:
            A >>= operand_value;
            break;
        case 1:
            B ^= operand;
            break;
        case 2:
            B = operand_value % 8;
            break;
        case 3:
            if (A != 0)
            {
                instruction_ptr = operand;
                continue;
            }
            break;
        case 4:
            B ^= C;
            break;
        case 5:
            output.push_back(operand_value % 8);
            break;
        case 6:
            B = A >> operand_value;
            break;
        case 7:
            C = A >> operand_value;
            break;
        }

        instruction_ptr += 2;
    }

    //std::cout << A << ", " << B << ", " << C << '\n';

    return std::reduce(output.cbegin(), output.cend(), std::string{},
                       [](std::string acc, int v) { return (acc.empty() ? "" : (acc + ",")) + std::to_string(v); });
}

/*
2,4,1,1,7,5,1,5,0,3,4,3,5,5,3,0
2,4 => B = A % 8
1,1 => B ^= 1
7,5 => C = A >> B
1,5 => B ^= 5
0,3 => A >>= 3
4,3 => B ^= C
5,5 => output B % 8
3,0 => if A != 0 -> ptr = 0
*/
long long part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::getline(ifs, line);
    std::getline(ifs, line);
    std::getline(ifs, line);
    std::getline(ifs, line);
    std::getline(ifs, line);
    std::vector<int> program;
    for (auto const& c : split(line.substr(9), ","))
        program.push_back(parse_num<int>(c));

    long long A = 0;
    long long B = 0;
    long long C = 0;
    long long cnt = 0;
    long long a = 0;
    std::vector<int> output;
    size_t matching_length = 0;
RERUN:
    //a = cnt++;
    //a = (cnt++ << (3 * 7)) + 03267275;
    a = (cnt++ << (3 * 11)) + 016273267275;
    A = a;
    output.clear();
    auto instruction_ptr = 0;
    while (instruction_ptr < program.size())
    {
        long long opcode = program[instruction_ptr];
        long long operand = program[instruction_ptr + 1];

        auto operand_value = operand;
        switch (operand)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            break;
        case 4:
            operand_value = A;
            break;
        case 5:
            operand_value = B;
            break;
        case 6:
            operand_value = C;
            break;
        case 7:
        default:
            goto RERUN;
        }

        switch (opcode)
        {
        case 0:
            A >>= operand_value;
            break;
        case 1:
            B ^= operand;
            break;
        case 2:
            B = operand_value % 8;
            break;
        case 3:
            if (A != 0)
            {
                instruction_ptr = operand;
                continue;
            }
            break;
        case 4:
            B ^= C;
            break;
        case 5:
            {
                output.push_back(operand_value % 8);
                if (output.size() > program.size()) goto RERUN;
                if (program[output.size() - 1] != output.back())
                {
                    if (output.size() > matching_length)
                    {
                        matching_length = output.size();
                        std::cout << a << ", " << std::setbase(8) << a << ", " << std::setbase(10) << matching_length
                                  << '\n';
                    }
                    goto RERUN;
                }
                break;
            }
        case 6:
            B = A >> operand_value;
            break;
        case 7:
            C = A >> operand_value;
            break;
        }

        instruction_ptr += 2;
    }
    if (instruction_ptr >= program.size())
        if (output != program) goto RERUN;

    return a;
}

int main(int argc, char* argv[])
{
    std::cout << "what do you get if you use commas to join the values it output into a single string?\n"
              << part_1(argv[1]) << std::endl;
    std::cout
        << "What is the lowest positive initial value for register A that causes the program to output a copy of itself?\n"
        << part_2(argv[1]) << std::endl;

    return 0;
}