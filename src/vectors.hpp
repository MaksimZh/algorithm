#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>


using IntVec = std::vector<int>;


void printVector(IntVec& a, size_t width = 3) {
    for (auto x : a) {
        std::cout << std::setw(width) << x << " ";
    }
    std::cout << std::endl;
}


IntVec randomVector(size_t size, int min = 0, int max = 100) {
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_int_distribution<int> distribution(0, 100);
    IntVec a;
    a.reserve(size);
    for (int i = 0; i < size; ++i) {
        a.push_back(distribution(gen));
    }
    return a;
}
