#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <ctime>
#include "benchmarks.h"

// Creating a random number generator based on the current time
std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

// Function for updating the agent's position based on time and search space
std::vector<double> updatePosition(const std::vector<double>& agent, double t, const std::vector<std::pair<double, double>>& search_space) {
    std::vector<double> new_position(agent.size());
    for(size_t i = 0; i < agent.size(); i++) {
        double amplitude = (search_space[i].second - search_space[i].first) / 2.0;
        new_position[i] = agent[i] + amplitude * std::sin(2 * M_PI * t);
        new_position[i] = std::clamp(new_position[i], search_space[i].first, search_space[i].second);
    }
    return new_position;
}

// The main function for optimization based on the algorithm
std::pair<std::vector<double>, double> aoa(int num_agents, int max_iter, const std::vector<std::pair<double, double>>& search_space,
                                           double (*benchmark_function)(const std::vector<double>&)) {
    int num_features = search_space.size();
    std::vector<std::vector<double>> agents(num_agents, std::vector<double>(num_features));

    // Initialization of the population
    for(int i = 0; i < num_agents; i++) {
        for(int j = 0; j < num_features; j++) {
            agents[i][j] = getRandomValue(search_space[j].first, search_space[j].second);
        }
    }

    std::vector<double> fitness(num_agents, std::numeric_limits<double>::infinity());

    // Using the benchmark_function function for initialized agents
    std::for_each(agents.begin(), agents.end(), [&](const std::vector<double>& agent) {
        int i = &agent - &agents[0];
        fitness[i] = benchmark_function(agent);
    });

    // Main optimization cycle
    for (int t = 0; t < max_iter; t++) {
        double time_ratio = static_cast<double>(t) / max_iter;

        for (int i = 0; i < num_agents; i++) {
            std::vector<double> new_position = updatePosition(agents[i], time_ratio, search_space);
            double new_fitness = benchmark_function(new_position);
            if (new_fitness < fitness[i]) {
                agents[i] = std::move(new_position);
                fitness[i] = new_fitness;
            }
        }
    }
    
    // Search for the best result among all agents
    auto min_element_iter = std::min_element(fitness.begin(), fitness.end());
    int best_index = std::distance(fitness.begin(), min_element_iter);
    return {agents[best_index], *min_element_iter};
}
