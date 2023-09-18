#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <chrono>
#include <functional>
#include "benchmarks.h"

double f1(const std::vector<double>& position) {
    return std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + b * b; });
}

double f2(const std::vector<double>& position) {
    double sum_abs = std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + std::abs(b); });
    double prod_abs = std::accumulate(position.begin(), position.end(), 1.0, [](double a, double b) { return a * std::abs(b); });
    return sum_abs + prod_abs;
}

double f3(const std::vector<double>& position) {
    double sum = 0.0;
    for (size_t i = 0; i < position.size(); ++i) {
        sum += std::accumulate(position.begin(), position.begin() + i + 1, 0.0);
    }
    return sum;
}

double f4(const std::vector<double>& position) {
    return *std::max_element(position.begin(), position.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
}

double f5(const std::vector<double>& position) {
    double sum = 0.0;
    for (size_t i = 0; i < position.size() - 1; ++i) {
        sum += 100 * std::pow(position[i+1] - position[i]*position[i], 2) + std::pow(position[i] - 1, 2);
    }
    return sum;
}

double f6(const std::vector<double>& position) {
    return std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + std::pow(std::floor(b + 0.5), 2); });
}

double f7(const std::vector<double>& position) {
    double sum = 0.0;
    for (size_t i = 0; i < position.size(); ++i) {
        sum += (i + 1) * std::pow(position[i], 4);
    }
    return sum;
}

double f8(const std::vector<double>& position) {
    return std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a - b * std::sin(std::sqrt(std::abs(b))); });
}

double f9(const std::vector<double>& position) {
    return std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + b*b - 10 * std::cos(2 * M_PI * b) + 10; });
}

double f10(const std::vector<double>& position) {
    double d = position.size();
    double sum1 = std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + b*b; });
    double sum2 = std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + std::cos(2 * M_PI * b); });

    return -20 * std::exp(-0.2 * std::sqrt(sum1 / d)) - std::exp(sum2 / d) + 20 + std::exp(1.0);
}

double f11(const std::vector<double>& position) {
    double d = position.size();
    double sum = 0.0;
    double prod = 1.0;

    for (size_t i = 0; i < d; ++i) {
        sum += position[i] * position[i];
        prod *= std::cos(position[i] / std::sqrt(i + 1));
    }

    return sum / 4000.0 - prod + 1;
}

double f12(const std::vector<double>& position) {
    double d = position.size();
    std::vector<double> y(d);

    for (size_t i = 0; i < d; ++i) {
        y[i] = 1 + (position[i] - 1) / 4;
    }

    double term1 = std::pow(std::sin(M_PI * y[0]), 2);
    double term2 = 0.0;
    for (size_t i = 0; i < d - 1; ++i) {
        term2 += std::pow(y[i] - 1, 2) * (1 + 10 * std::pow(std::sin(M_PI * y[i + 1]), 2));
    }
    double term3 = std::pow(y[d - 1] - 1, 2) * (1 + std::sin(2 * M_PI * y[d - 1]));

    return M_PI / d * (10 * term1 + term2 + term3);
}
