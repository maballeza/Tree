#include <iostream> // Exception-handling.
#include "Tree.hpp"
#include <array>
#include <corecrt_math_defines.h>
#define _USE_MATH_DEFINES

int main() {
    Tree<double, double> t;

    const int range = 200;
    std::array<Tree<double, double>::Node*, range> ptrs;

    // A tree mapping sinusoidally changing values from a 
    // range (e.g. 0 to 200) with its corresponding radial value.
    for (int i = 0; i < range; ++i) {
        double k = static_cast<double>(i) / range * 2 * M_PI;
        double val = static_cast<double>(range / 2) * (1 + cos(3 * M_PI_4 + k));
        t.Insert(k, std::move(val));
    }
    for (int i = 0; i < range; ++i) {
        double k = static_cast<double>(i) / range * 2 * M_PI;
        double val = static_cast<double>(range / 2) * (1 + cos(3 * M_PI_4 + k));
        ptrs[i] = t[k];
    }

    for (int i = 0; i < range; ++i) {
        double k = static_cast<double>(i) / range * 2 * M_PI;
        double val = static_cast<double>(range / 2) * (1 + cos(3 * M_PI_4 + k));
        t.Delete(&ptrs[i]);
        if (ptrs[i] != nullptr) throw;
    }

    auto v1 = t.Walk();
    Tree<double, double> t2{ std::move(t) };
    auto v2 = t2.Walk();

    auto p = v1.begin();
    for (auto& v : v2) {
        if (p->first != v.first) throw;
        if (p->second != v.second) throw;
        p++;
    }
 }