#ifndef UTIL_H
#define UTIL_H
#include <limits>
#include <random>

inline float random_float(const float min = 0.0F, const float max = 1.0F) {
    static thread_local std::mt19937 generator;
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}
inline int random_int(const int min, const int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(random_float(min, max+1));
}
constexpr float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
#endif
