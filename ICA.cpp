#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <ctime>
#include <chrono>
#include <functional>
#include "benchmarks.h"

// Main ICA function
std::pair<std::vector<double>, double> ica(int num_agents, int max_iter, const std::vector<std::pair<double, double>>& search_space,
                                           int num_empires, double (*benchmark_function)(const std::vector<double>&)) {
    int num_dimensions = search_space.size();
    std::vector<std::vector<double>> agents(num_agents, std::vector<double>(num_dimensions));
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (int i = 0; i < num_agents; ++i) {
        for (int j = 0; j < num_dimensions; ++j) {
            agents[i][j] = search_space[j].first + (search_space[j].second - search_space[j].first) * static_cast<double>(std::rand()) / RAND_MAX;
        }
    }

    std::vector<double> fitness(num_agents);
    for (int i = 0; i < num_agents; ++i) {
        fitness[i] = benchmark_function(agents[i]);
    }

    std::vector<int> sorted_indices(num_agents);
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&fitness](int i1, int i2) { return fitness[i1] < fitness[i2]; });

    std::vector<std::vector<double>> empires(num_empires);
    std::vector<std::vector<double>> colonies(num_agents - num_empires);
    std::vector<double> empire_fitness(num_empires);
    std::vector<double> colony_fitness(num_agents - num_empires);

    for (int i = 0; i < num_empires; ++i) {
        empires[i] = agents[sorted_indices[i]];
        empire_fitness[i] = fitness[sorted_indices[i]];
    }
    for (int i = 0; i < colonies.size(); ++i) {
        colonies[i] = agents[sorted_indices[i + num_empires]];
        colony_fitness[i] = fitness[sorted_indices[i + num_empires]];
    }

    // Инициализация адаптивных скоростей обучения и коэффициентов ассимиляции
    double learning_rate_init = 0.5;
    double learning_rate_final = 0.01;
    double assimilation_coeff_init = 0.5;
    double assimilation_coeff_final = 0.1;

    for (int t = 0; t < max_iter; ++t) {
        double assimilation_coeff = assimilation_coeff_init - (assimilation_coeff_init - assimilation_coeff_final) * static_cast<double>(t) / max_iter;
        double learning_rate = learning_rate_init - (learning_rate_init - learning_rate_final) * static_cast<double>(t) / max_iter;

        // Осуществляем скрещивание между империями
        for (int i = 0; i < num_empires; ++i) {
            if (static_cast<double>(std::rand()) / RAND_MAX < 0.5) {
                int other = std::rand() % num_empires;
                std::vector<double> child(num_dimensions);
                for (int j = 0; j < num_dimensions; ++j) {
                    child[j] = 0.5 * (empires[i][j] + empires[other][j]);
                }
                double child_fitness = benchmark_function(child);
                if (child_fitness < empire_fitness[i]) {
                    empires[i] = child;
                    empire_fitness[i] = child_fitness;
                }
            }
        }

        // Обновление позиций колоний на основе их соответствующих империй
        for (int i = 0; i < colonies.size(); ++i) {
            for (int j = 0; j < num_dimensions; ++j) {
                colonies[i][j] -= learning_rate * assimilation_coeff * (colonies[i][j] - empires[i % num_empires][j]);
            }
        }

        // Осуществляем революцию, внося случайные возмущения
        for (int i = 0; i < colonies.size(); ++i) {
            for (int j = 0; j < num_dimensions; ++j) {
                colonies[i][j] += 0.2 * static_cast<double>(rand()) / RAND_MAX;
            }
        }

        // Обновление приспособленности всех агентов
        for (int i = 0; i < num_empires; ++i) {
            empire_fitness[i] = benchmark_function(empires[i]);
        }
        for (int i = 0; i < colonies.size(); ++i) {
            colony_fitness[i] = benchmark_function(colonies[i]);
        }

        // Поиск лучшей империи
        int best_index = std::distance(empire_fitness.begin(), std::min_element(empire_fitness.begin(), empire_fitness.end()));
        auto best_agent = empires[best_index];
        double best_fitness = empire_fitness[best_index];

        if (*std::min_element(empire_fitness.begin(), empire_fitness.end()) < 1e-9) {
            break;
        }
    }

    int best_index = std::distance(empire_fitness.begin(), std::min_element(empire_fitness.begin(), empire_fitness.end()));
    return {empires[best_index], empire_fitness[best_index]};
}
