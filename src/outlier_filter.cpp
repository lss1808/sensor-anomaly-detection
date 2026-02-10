#include "outlier_filter.h"
#include <cmath>
#include <numeric>

OutlierFilter::OutlierFilter(int tamanho, double limite)
    : tamanhoJanela(tamanho), limiteZ(limite) {}

bool OutlierFilter::isAnomaly(double valor) {
    janela.push_back(valor);

    if (janela.size() < (size_t)tamanhoJanela)
        return false;

    double media = std::accumulate(janela.begin(), janela.end(), 0.0) / janela.size();

    double soma = 0.0;
    for (double v : janela)
        soma += (v - media) * (v - media);

    double desvio = std::sqrt(soma / janela.size());

    if (janela.size() > (size_t)tamanhoJanela)
        janela.pop_front();

    double z = std::abs((valor - media) / desvio);
    return z > limiteZ;
}
