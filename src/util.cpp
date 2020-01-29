#include "util.hpp"


float random_float(const float min, const float max) {
    static thread_local std::mt19937 generator;
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}
