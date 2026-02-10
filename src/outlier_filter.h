#ifndef OUTLIER_FILTER_H
#define OUTLIER_FILTER_H

#include <deque>
#include <vector>

class OutlierFilter {
private:
    std::deque<double> janela;
    int tamanhoJanela;
    double limiteZ;

public:
    OutlierFilter(int tamanho = 30, double limite = 2.5);
    bool isAnomaly(double valor);
};

#endif
