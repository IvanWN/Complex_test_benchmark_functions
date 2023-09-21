#include <vector>
#include <random>
#include <algorithm>

// Вспомогательные функции
double getRandomValue(double min, double max) {
    static std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

std::vector<double> calculateDifferenceSCA(const std::vector<double>& v1, const std::vector<double>& v2, double C) {
    std::vector<double> result(v1.size());
    for(size_t i = 0; i < v1.size(); i++) {
        result[i] = v1[i] - v2[i];
        result[i] *= C;
    }
    return result;
}

std::vector<double> calculateDifferenceRandomPositonSCA(const std::vector<double>& random_agent, const std::vector<double>& D, double A) {
    std::vector<double> result(random_agent.size());
    for(size_t i = 0; i < random_agent.size(); i++) {
        result[i] = random_agent[i] + A * D[i];
    }
    return result;
}

// Основная функция SCA
std::pair<std::vector<double>, double> sca(int num_agents, int max_iter, const std::vector<std::pair<double, double>>& search_space,
                                           double (*benchmark_function)(const std::vector<double>&)) {
    int num_features = search_space.size();
    std::vector<std::vector<double>> agents(num_agents, std::vector<double>(num_features));

    // Инициализация популяции
    for(int i = 0; i < num_agents; i++) {
        for(int j = 0; j < num_features; j++) {
            agents[i][j] = getRandomValue(search_space[j].first, search_space[j].second);
        }
    }

    std::vector<double> fitness(num_agents);

    for(int i = 0; i < num_agents; i++) {
        fitness[i] = benchmark_function(agents[i]);
    }

    for (int t = 0; t < max_iter; t++) {
        for (int i = 0; i < num_agents; i++) {
            double a_t = 2.0 - double(t) * (2.0 / double(max_iter));
            double r1 = getRandomValue(0, 1);
            double r2 = getRandomValue(0, 1);
            double A = 2 * a_t * r1 - a_t;
            double C = 2 * r2;

            int random_agent_index = getRandomIndex(num_agents);
            while (random_agent_index == i)
                random_agent_index = getRandomIndex(num_agents);

            std::vector<double> random_agent = agents[random_agent_index];
            std::vector<double> D = calculateDifferenceSCA(random_agent, agents[i], C);
            std::vector<double> new_position = calculateDifferenceRandomPositonSCA(random_agent, D, A);

            double new_fitness = benchmark_function(new_position);
            if (new_fitness < fitness[i]) {
                agents[i] = new_position;
                fitness[i] = new_fitness;
            }
        }
    }

    int best_index = std::distance(fitness.begin(), std::min_element(fitness.begin(), fitness.end()));
    return {agents[best_index], fitness[best_index]};
}
