#ifndef OUTLIER_FILTER_H
#define OUTLIER_FILTER_H

#include <deque>

class OutlierFilter {
public:
    OutlierFilter(int janela, double limite_z);

    bool isAnomaly(double valor);
    void addSample(double valor);

private:
    int tamanho_janela;
    double limite_z;

    std::deque<double> buffer;

    double soma = 0.0;
    double soma_quadrados = 0.0;

    double media() const;
    double desvio() const;
};

#endif

