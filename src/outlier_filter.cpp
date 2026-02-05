#include "outlier_filter.h"
#include <cmath>

OutlierFilter::OutlierFilter(int janela, double limite_z)
    : tamanho_janela(janela), limite_z(limite_z) {}

double OutlierFilter::media() const {
    return soma / buffer.size();
}

double OutlierFilter::desvio() const {
    double m = media();
    double variancia = (soma_quadrados / buffer.size()) - (m * m);
    return std::sqrt(variancia);
}

bool OutlierFilter::isAnomaly(double valor) {
    if (buffer.size() < tamanho_janela) return false;

    double m = media();
    double d = desvio();

    if (d == 0.0) return false;

    double z = (valor - m) / d;
    return std::abs(z) > limite_z;
}

void OutlierFilter::addSample(double valor) {
    buffer.push_back(valor);
    soma += valor;
    soma_quadrados += valor * valor;

    if (buffer.size() > tamanho_janela) {
        double removido = buffer.front();
        buffer.pop_front();
        soma -= removido;
        soma_quadrados -= removido * removido;
    }
}

