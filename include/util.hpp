#ifndef UTIL_H
#define UTIL_H

#include <random>

inline float random_float(const float min = 0.0f, const float max = 1.0f) {
    static thread_local std::mt19937 generator;
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

#endif
