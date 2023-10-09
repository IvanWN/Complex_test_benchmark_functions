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

#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

#ifndef M_E
const double M_E = 2.71828182845904523536;
#endif

double f1(const std::vector<double>& position) {
    return std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + b * b; });
}

double f2(const std::vector<double>& position) {
    double sum_abs = std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + std::abs(b); });
    return sum_abs + sum_abs * sum_abs;
}

double f3(const std::vector<double>& position) {
    double result = 0.0;
    for (size_t i = 0; i < position.size(); ++i) {
        double inner_sum = 0.0;
        for (size_t j = 0; j <= i; ++j) {
            inner_sum += position[j];
        }
        result += inner_sum * inner_sum;
    }
    return result;
}

double f4(const std::vector<double>& position) {
    return *std::max_element(position.begin(), position.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
}

double f5(const std::vector<double>& position) {
    double result = 0.0;
    for (size_t i = 0; i < position.size() - 1; ++i) {
        double term1 = 100.0 * std::pow((position[i + 1] - position[i] * position[i]), 2);
        double term2 = std::pow((position[i] - 1.0), 2);
        result += term1 + term2;
    }
    return result;
}

double f6(const std::vector<double>& position) {
    return std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + std::pow(std::round(b + 0.5), 2); });
}

double f7(const std::vector<double>& position) {
    double result = 0.0;
    for (size_t i = 0; i < position.size(); ++i) {
        result += (i + 1) * std::pow(position[i], 4) + ((double)rand() / RAND_MAX);
    }
    return result;
}

double f8(const std::vector<double>& position) {
    return std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a - b * std::sin(std::sqrt(std::abs(b))); });
}

double f9(const std::vector<double>& position) {
    return std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + b * b - 10.0 * std::cos(2.0 * M_PI * b) + 10.0; });
}

double f10(const std::vector<double>& position) {
    double sum1 = std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + b * b; });
    double sum2 = std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + std::cos(2.0 * M_PI * b); });
    return -20.0 * std::exp(-0.2 * std::sqrt(sum1 / position.size())) - std::exp(sum2 / position.size()) + 20.0 + M_E;
}

double f11(const std::vector<double>& position) {
    double sum1 = std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + b * b; });
    double sum2 = std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + std::cos(std::sqrt(std::abs(b))); });
    return sum1 / 4000.0 - sum2 + 1.0;
}

double u(double x, double a, double k, double m) {
    if (x > a) return k * std::pow(x - a, m);
    else if (x < -a) return k * std::pow(-x - a, m);
    else return 0.0;
}

double f12(const std::vector<double>& position) {
    std::vector<double> y(position.size());
    std::transform(position.begin(), position.end(), y.begin(), [](double xi) { return 1.0 + (xi + 1.0) / 4.0; });

    double sum1 = std::accumulate(y.begin(), y.end() - 1, 0.0, [](double a, double b) { return a + std::pow(b - 1.0, 2.0) * (1.0 + 10.0 * std::sin(M_PI * b + 1.0) * std::sin(M_PI * b + 1.0)); });
    double term1 = 10.0 * std::sin(M_PI * y[0]);
    double term2 = std::pow(y.back() - 1.0, 2.0);
    double sum2 = std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + u(b, 10.0, 100.0, 4.0); });

    return M_PI / position.size() * (term1 + sum1 + term2) + sum2;
}

double f13(const std::vector<double>& position) {
    double sum1 = std::accumulate(position.begin(), position.end() - 1, 0.0, [](double a, double b) { return a + std::pow(b - 1.0, 2.0) * (1.0 + std::sin(3.0 * M_PI * b + 1.0) * std::sin(3.0 * M_PI * b + 1.0)); });
    double term1 = 0.1 * std::sin(3.0 * M_PI * position[0]) * std::sin(3.0 * M_PI * position[0]);
    double term2 = 0.1 * std::pow(position.back() - 1.0, 2.0) * (1.0 + std::sin(2.0 * M_PI * position.back()) * std::sin(2.0 * M_PI * position.back()));
    double sum2 = std::accumulate(position.begin(), position.end(), 0.0, [](double a, double b) { return a + u(b, 5.0, 100.0, 4.0); });

    return term1 + sum1 + term2 + sum2;
}

// Composite Function F14 (CF1)
double f14(const std::vector<double>& position) {
    const std::vector<double> biases = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const std::vector<double> lambdas = {5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0};

    double result = 0.0;
    for (size_t i = 0; i < 10; ++i) {
        result += biases[i] * f1(position) * lambdas[i];
    }
    return result;
}

// Composite Function F15 (CF2)
double f15(const std::vector<double>& position) {
    const std::vector<double> biases = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const std::vector<double> lambdas = {5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0};

    double result = 0.0;
    for (size_t i = 0; i < 10; ++i) {
        result += biases[i] * f2(position) * lambdas[i];
    }
    return result;
}

// Composite Function F16 (CF3)
double f16(const std::vector<double>& position) {
    const std::vector<double> biases = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    double result = 0.0;
    for (size_t i = 0; i < 10; ++i) {
        result += biases[i] * f3(position);
    }
    return result;
}

// Composite Function F17 (CF4)
double f17(const std::vector<double>& position) {
    const std::vector<double> biases = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const std::vector<double> lambdas = {5.0/32.0, 5.0/32.0, 1.0, 1.0, 5.0/0.5, 5.0/0.5, 5.0/100.0, 5.0/100.0, 5.0/100.0, 5.0/100.0};

    std::vector<std::function<double(const std::vector<double>&)>> functions = {f10, f10, f9, f9, f8, f8, f7, f7, f1, f1};

    double result = 0.0;
    for (size_t i = 0; i < 10; ++i) {
        result += biases[i] * functions[i](position) * lambdas[i];
    }
    return result;
}

double f18(const std::vector<double>& position) {
    const std::vector<double> biases = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const std::vector<double> lambdas = {1/5.0, 1/5.0, 5/0.5, 5/0.5, 5/100.0, 5/100.0, 5/32.0, 5/32.0, 5/100.0, 5/100.0};

    std::vector<std::function<double(const std::vector<double>&)>> functions = {f9, f9, f2, f2, f11, f11, f10, f10, f1, f1};  // Adjusted according to the provided functions.

    double result = 0.0;
    for (size_t i = 0; i < 10; ++i) {
        result += biases[i] * functions[i](position) * lambdas[i];
    }
    return result;
}

double f19(const std::vector<double>& position) {
    const std::vector<double> biases = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
    const std::vector<double> lambdas = {0.1 * 1/5.0, 0.2 * 1/5.0, 0.3 * 5/0.5, 0.4 * 5/0.5, 0.5 * 5/100.0, 0.6 * 5/100.0, 0.7 * 5/32.0, 0.8 * 5/32.0, 0.9 * 5/100.0, 1 * 5/100.0};

    std::vector<std::function<double(const std::vector<double>&)>> functions = {f9, f9, f2, f2, f11, f11, f10, f10, f1, f1};  // Adjusted according to the provided functions.

    double result = 0.0;
    for (size_t i = 0; i < 10; ++i) {
        result += biases[i] * functions[i](position) * lambdas[i];
    }
    return result;
}
