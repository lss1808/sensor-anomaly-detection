#include "outlier_filter.h"
#include <cmath>

OutlierFilter::OutlierFilter(int janela, double limite_z)
    : tamanho_janela(janela), limite_z(limite_z) {}

double OutlierFilter::media(const std::vector<double>& dados) {
    double soma = 0.0;
    for (double v : dados) soma += v;
    return soma / dados.size();
}

double OutlierFilter::desvio(const std::vector<double>& dados, double m) {
    double soma = 0.0;
    for (double v : dados)
        soma += (v - m) * (v - m);
    return std::sqrt(soma / dados.size());
}

bool OutlierFilter::isAnomaly(const std::vector<double>& janela, double valor) {
    if (janela.size() < tamanho_janela) return false;

    double m = media(janela);
    double d = desvio(janela, m);

    if (d == 0.0) return false;

    double z = (valor - m) / d;
    return std::abs(z) > limite_z;
}
