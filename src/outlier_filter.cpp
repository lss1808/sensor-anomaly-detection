#include "outlier_filter.h"
#include <cmath>
#include <numeric>

OutlierFilter::OutlierFilter(int janela, double limite)
    : tamanhoJanela(janela), limiteZ(limite) {}

bool OutlierFilter::isAnomaly(const std::vector<double>& janela, double valor) {
    double media = std::accumulate(janela.begin(), janela.end(), 0.0) / janela.size();

    double soma = 0.0;
    for (double v : janela)
        soma += (v - media) * (v - media);

    double desvio = std::sqrt(soma / janela.size());
    if (desvio == 0) return false;

    double z = std::abs((valor - media) / desvio);
    return z > limiteZ;
}
