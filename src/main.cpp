#include <iostream>
#include <fstream>
#include <sstream>
#include "outlier_filter.h"

int main() {
    std::ifstream arquivo("data/sensor_data.csv");
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo.\n";
        return 1;
    }

    const int TAM_JANELA = 20;
    const double LIMITE_Z = 3.0;

    OutlierFilter filtro(TAM_JANELA, LIMITE_Z);

    std::string linha;
    int indice = 0;

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        double valor;

        while (ss >> valor) {
            if (filtro.isAnomaly(valor)) {
                std::cout << "Anomalia detectada no ponto "
                          << indice << " | Valor: " << valor << "\n";
            }

            filtro.addSample(valor);
            indice++;

            if (ss.peek() == ',') ss.ignore();
        }
    }
}

