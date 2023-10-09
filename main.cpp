#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <cmath>
#include <fstream>
#include <iomanip>

// Including files
#include "SSA.cpp"
#include "ICA.cpp"
#include "TLBO.cpp"
#include "SCA.cpp"
#include "AOA.cpp"
#include "WOA.cpp"
#include "benchmarks.h"

using namespace std;

struct BenchmarkConfig {
    double (*benchmark_func)(const std::vector<double>&);
    std::vector<std::pair<double, double>> search_space;
};

template <typename AlgorithmFunc>
vector<vector<double>> run_and_evaluate(const string& algorithm_name, AlgorithmFunc algorithm, int num_runs, const std::vector<BenchmarkConfig>& configs) {
    std::vector<vector<double>> all_results(configs.size(), vector<double>(3, 0.0));

    double (*mean_function)(const std::vector<double>&) = [](const std::vector<double>& values) {
        return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    };
    double (*stddev_function)(const std::vector<double>&, double) = [](const std::vector<double>& values, double mean_val) {
        double variance = std::accumulate(values.begin(), values.end(), 0.0, [mean_val](double acc, double val) {
            return acc + (val - mean_val) * (val - mean_val);
        }) / values.size();
        return std::sqrt(variance);
    };

    for (size_t config_idx = 0; config_idx < configs.size(); ++config_idx) {
        auto& benchmark_function = configs[config_idx].benchmark_func;
        auto& search_space = configs[config_idx].search_space;

        std::vector<double> fitness_results;
        std::vector<double> run_times;

        for (int i = 0; i < num_runs; ++i) {
            auto start_time = std::chrono::high_resolution_clock::now();
            auto [best_solution, fitness] = algorithm(benchmark_function, search_space);
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> run_time = end_time - start_time;

            fitness_results.push_back(fitness);
            run_times.push_back(run_time.count());
        }
        all_results[config_idx][0] = mean_function(fitness_results);
        all_results[config_idx][1] = stddev_function(fitness_results, all_results[config_idx][0]);
        all_results[config_idx][2] = mean_function(run_times);
    }
    return all_results;
}

int main() {
    const int num_runs = 30;

    std::vector<BenchmarkConfig> configs = {
            {f1, std::vector<std::pair<double, double>>(20, {-100.0, 100.0})},
            {f2, std::vector<std::pair<double, double>>(20, {-10.0, 10.0})},
            {f3, std::vector<std::pair<double, double>>(20, {-100.0, 100.0})},
            {f4, std::vector<std::pair<double, double>>(20, {-100.0, 100.0})},
            {f5, std::vector<std::pair<double, double>>(20, {-30,30})},
            {f6, std::vector<std::pair<double, double>>(20, {-10.0, 10.0})},
            {f7, std::vector<std::pair<double, double>>(20, {-1.28,1.28})},
            {f8, std::vector<std::pair<double, double>>(20, {-500,500})},
            {f9, std::vector<std::pair<double, double>>(20, {-5.12,5.12})},
            {f10, std::vector<std::pair<double, double>>(20, {-32,32})},
            {f11, std::vector<std::pair<double, double>>(20, {-600,600})},
            {f12, std::vector<std::pair<double, double>>(20, {-50,50})},
            {f13, std::vector<std::pair<double, double>>(20, {-50,50})},
            {f14, std::vector<std::pair<double, double>>(10, {-5,5})},
            {f15, std::vector<std::pair<double, double>>(10, {-5,5})},
            {f16, std::vector<std::pair<double, double>>(10, {-5,5})},
            {f17, std::vector<std::pair<double, double>>(10, {-5,5})},
            {f18, std::vector<std::pair<double, double>>(10, {-5,5})},
            {f19, std::vector<std::pair<double, double>>(10, {-5,5})},
    };

    std::vector<std::string> algorithm_names = {"WOA", "AOA", "SSA", "ICA", "TLBO", "SCA"};
    std::vector<std::vector<vector<double>>> all_algorithm_results;

    all_algorithm_results.push_back(run_and_evaluate("WOA", [](auto benchmark_function, auto search_space) {
        return woa(100, 3000, search_space, benchmark_function);
    }, num_runs, configs));

    all_algorithm_results.push_back(run_and_evaluate("AOA", [](auto benchmark_function, auto search_space) {
        return aoa(100, 3000, search_space, benchmark_function);
    }, num_runs, configs));

    all_algorithm_results.push_back(run_and_evaluate("SSA", [](auto benchmark_function, auto search_space) {
        return ssa(100, 3000, search_space, benchmark_function);
    }, num_runs, configs));

    all_algorithm_results.push_back(run_and_evaluate("ICA", [](auto benchmark_function, auto search_space) {
        return ica(100, 3000, search_space, 10, benchmark_function);
    }, num_runs, configs));

    all_algorithm_results.push_back(run_and_evaluate("TLBO", [](auto benchmark_function, auto search_space) {
        return tlbo(100, 3000, search_space, benchmark_function);
    }, num_runs, configs));

    all_algorithm_results.push_back(run_and_evaluate("SCA", [](auto benchmark_function, auto search_space) {
        return sca(100, 3000, search_space, benchmark_function);
    }, num_runs, configs));


    std::ofstream outfile("results.txt");

    if (!outfile.is_open()) {
        std::cerr << "Error opening results.txt for writing" << std::endl;
        return 1;
    }

    outfile << std::fixed << std::setprecision(2);
    for (const auto& name : algorithm_names)
        outfile << std::setw(15) << name;
    outfile << '\n';

    for (size_t config_idx = 0; config_idx < configs.size(); ++config_idx) {
        outfile << "f" << config_idx + 1 << ": ";
        for (const auto& alg_results : all_algorithm_results)
            outfile << std::setw(5) << alg_results[config_idx][0] << " " << std::setw(5) << alg_results[config_idx][1] << " " << std::setw(5) << alg_results[config_idx][2] << " |";
        outfile << '\n';
    }
    outfile.close();

    return 0;
}
