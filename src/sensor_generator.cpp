#include <iostream>
#include <fstream>
#include <random>

int main() {
    std::ofstream file("data/sensor_data.csv");

    if (!file.is_open()) {
        std::cerr << "Erro ao criar arquivo de dados.\n";
        return 1;
    }

    std::default_random_engine generator;
    std::normal_distribution<double> normal_dist(70.0, 2.0); // média 70, desvio 2

    for (int i = 0; i < 500; i++) {
        double value = normal_dist(generator);

        // Inserir anomalias propositalmente
        if (i == 150 || i == 320) {
            value = 120.0; // pico anormal
        }

        file << value << "\n";
    }

    file.close();
    std::cout << "Dados do sensor gerados com sucesso.\n";
    return 0;
}
