#include <iostream> // Exception-handling.
#include "Tree.hpp"
#include <array>
#include <corecrt_math_defines.h>
#define _USE_MATH_DEFINES

int main() {
    Tree<double> t;
    auto one = t.Insert(1, std::move(1));
    auto two = t.Insert(2, std::move(2));
    if (one != t.Predecessor(two)) throw;
    if (two != t.Successor(one)) throw;
    if (nullptr != t.Predecessor(one)) throw;
    if (nullptr != t.Successor(two)) throw;
    t.Delete(one);
    t.Delete(two);

    const int range = 200;
    std::array<double, range> vals;
    std::array<Node<double>*, range> ptrs;

    for (int i = 0; i < range; i++) {
        double v = i / static_cast<double>(range) * 2 * M_PI;
        int k = range / 2 * (1 + cos(3 * M_PI_4 + v));          // k : k = [ 0, ..., 200 ]
        std::cout << "v: " << v << "\tk: " << k << std::endl;
        ptrs[i] = t.Insert(k, std::move(v));
        vals[i] = ptrs[i]->value;
    }
    for (int i = 0; i < range; i++) {
        double v = i / static_cast<double>(range) * 2 * M_PI;
        int k = range / 2 * (1 + cos(3 * M_PI_4 + v));
        if (vals[i] != t.Delete(ptrs[i]).value) throw;
    }
}