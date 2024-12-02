#include <vector>
#include <iostream>

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
    os << "[ ";
    for (auto const& e : v)
        os << e << " ";
    return os << "]\n";
}