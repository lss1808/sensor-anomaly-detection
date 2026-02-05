#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Calcula a média
double calcular_media(const std::vector<double>& dados) {
    double soma = 0.0;
    for (double v : dados)
        soma += v;
    return soma / dados.size();
}

// Calcula o desvio padrão
double calcular_desvio(const std::vector<double>& dados, double media) {
    double soma = 0.0;
    for (double v : dados)
        soma += (v - media) * (v - media);
    return std::sqrt(soma / dados.size());
}

int main() {
    std::ifstream arquivo("data/sensor_data.csv");

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo de dados.\n";
        return 1;
    }

    std::vector<double> janela;
    double valor;

    const int TAM_JANELA = 20;
    const double LIMITE_Z = 3.0;

    int indice = 0;

    while (arquivo >> valor) {
        janela.push_back(valor);

        if (janela.size() >= TAM_JANELA) {
            double media = calcular_media(janela);
            double desvio = calcular_desvio(janela, media);

            double z = (valor - media) / desvio;

            if (std::abs(z) > LIMITE_Z) {
                std::cout << "Anomalia detectada no ponto "
                          << indice
                          << " | Valor: " << valor
                          << " | Z-score: " << z << "\n";
            }

            janela.erase(janela.begin());
        }

        indice++;
    }

    arquivo.close();
    return 0;
}
