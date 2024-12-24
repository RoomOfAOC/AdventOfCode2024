//
// Created by Harold on 2024/12/24.
//

#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "../utils/str_op.h"
#include "../utils/debug.h"

struct wire_t
{
    std::string name;
    bool value;
};
std::ostream& operator<<(std::ostream& os, wire_t const& w)
{
    return os << "wire_t{" << w.name << ", " << w.value << "}";
}

struct gate_t
{
    std::string name;
    std::string op0, op, op1;
};
std::ostream& operator<<(std::ostream& os, gate_t const& g)
{
    return os << "gate_t{" << g.name << ", " << g.op0 << ", " << g.op << ", " << g.op1 << "}";
}

unsigned long long part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;

    std::unordered_map<std::string, wire_t> wires;
    while (std::getline(ifs, line) && !line.empty())
    {
        auto name = line.substr(0, 3);
        wires[name] = wire_t{name, static_cast<bool>(parse_num<int>(line.substr(5)))};
    }
    //std::cout << wires << '\n';

    std::unordered_map<std::string, gate_t> gates;
    while (std::getline(ifs, line) && !line.empty())
    {
        auto op0 = line.substr(0, 3);
        auto s = line.find(' ', 4);
        auto op = line.substr(4, s - 4);
        auto op1 = line.substr(s + 1, 3);
        auto name = line.substr(s + 8);
        gates[name] = gate_t{name, op0, op, op1};
    }
    //std::cout << gates << '\n';

    auto eval_gate = [&wires](gate_t const& g) {
        if (wires.find(g.op0) != wires.end() && wires.find(g.op1) != wires.end())
        {
            if (g.op == "AND")
                wires[g.name] = wire_t{g.name, wires[g.op0].value && wires[g.op1].value};
            else if (g.op == "OR")
                wires[g.name] = wire_t{g.name, wires[g.op0].value || wires[g.op1].value};
            else if (g.op == "XOR")
                wires[g.name] = wire_t{g.name, static_cast<bool>(wires[g.op0].value ^ wires[g.op1].value)};
        }
    };

    while (true)
    {
        bool ok = true;
        for (auto const& [name, g] : gates)
            if (wires.find(name) == wires.end())
            {
                ok = false;
                eval_gate(g);
            }
        if (ok) break;
    }

    std::vector<wire_t> zs;
    for (auto const& [name, w] : wires)
        if (name[0] == 'z') zs.push_back(w);
    std::sort(zs.begin(), zs.end(), [](auto const& w1, auto const& w2) {
        return parse_num<int>(w1.name.substr(1)) > parse_num<int>(w2.name.substr(1));
    });
    std::ostringstream oss;
    std::for_each(zs.cbegin(), zs.cend(), [&oss](auto const& w) { oss << w.value; });

    return std::stoull(oss.str(), nullptr, 2);
}

std::string part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;

    std::unordered_map<std::string, wire_t> wires;
    while (std::getline(ifs, line) && !line.empty())
    {
        auto name = line.substr(0, 3);
        wires[name] = wire_t{name, static_cast<bool>(parse_num<int>(line.substr(5)))};
    }

    std::unordered_map<std::string, gate_t> gates;
    while (std::getline(ifs, line) && !line.empty())
    {
        auto op0 = line.substr(0, 3);
        auto s = line.find(' ', 4);
        auto op = line.substr(4, s - 4);
        auto op1 = line.substr(s + 1, 3);
        auto name = line.substr(s + 8);
        gates[name] = gate_t{name, op0, op, op1};
    }
    //std::cout << gates << '\n';

    auto find = [&gates](auto const& op0, auto const& op1, auto const& op) -> std::string {
        for (auto const& [_, g] : gates)
            if (((g.op0 == op0 && g.op1 == op1) || (g.op0 == op1 && g.op1 == op0)) && g.op == op) return g.name;
        return {};
    };

    std::vector<std::pair<std::string, std::string>> swapped_pairs;
    swapped_pairs.reserve(4);
    // half adder
    // X1 XOR Y1 => M1
    // X1 AND Y1 => N1
    // C0 AND M1 => R1
    // C0 XOR M1 -> Z1
    // R1 OR N1 -> C1
    std::string m1, n1, r1, z1, c0, c1;
    for (auto i = 0; i < 45; i++)
    {
        auto n = i > 9 ? std::to_string(i) : ("0" + std::to_string(i));
        auto xn = "x" + n;
        auto yn = "y" + n;

        m1 = find(xn, yn, "XOR");
        n1 = find(xn, yn, "AND");
        //std::cout << "m1: " << m1 << ", n1: " << n1 << '\n';

        if (!c0.empty())
        {
            r1 = find(c0, m1, "AND");
            if (r1.empty())
            {
                std::swap(m1, n1);
                swapped_pairs.push_back(std::make_pair(m1, n1));
                r1 = find(c0, m1, "AND");
            }
            //std::cout << "r1: " << r1 << '\n';

            z1 = find(c0, m1, "XOR");
            //std::cout << "z1: " << z1 << '\n';

            if (m1[0] == 'z')
            {
                std::swap(m1, z1);
                swapped_pairs.push_back(std::make_pair(m1, z1));
            }
            if (n1[0] == 'z')
            {
                std::swap(n1, z1);
                swapped_pairs.push_back(std::make_pair(n1, z1));
            }
            if (r1[0] == 'z')
            {
                std::swap(r1, z1);
                swapped_pairs.push_back(std::make_pair(r1, z1));
            }

            c1 = find(r1, n1, "OR");
            //std::cout << "c1: " << c1 << '\n';
        }

        if (c1[0] == 'z' && c1 != "z45")
        {
            std::swap(c1, z1);
            swapped_pairs.push_back(std::make_pair(c1, z1));
        }

        if (!c0.empty())
            c0 = c1;
        else
            c0 = n1;

        //std::cout << "c0: " << c0 << '\n';
    }
    //std::cout << swapped_pairs << '\n';

    std::vector<std::string> swapped;
    swapped.reserve(8);
    std::for_each(swapped_pairs.cbegin(), swapped_pairs.cend(), [&swapped](auto const& p) {
        swapped.push_back(p.first);
        swapped.push_back(p.second);
    });
    std::sort(swapped.begin(), swapped.end());

    return std::reduce(swapped.cbegin(), swapped.cend(), std::string{},
                       [](auto const& s, auto const& e) { return s.empty() ? e : s + "," + e; });
}

int main(int argc, char* argv[])
{
    std::cout << "What decimal number does it output on the wires starting with z?\n" << part_1(argv[1]) << std::endl;
    std::cout
        << "what do you get if you sort the names of the eight wires involved in a swap and then join those names with commas?\n"
        << part_2(argv[1]) << std::endl;

    return 0;
}