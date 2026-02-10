#pragma once
#include <vector>

class OutlierFilter {
public:
    OutlierFilter(int janela, double limite);
    bool isAnomaly(const std::vector<double>& janela, double valor);

private:
    int tamanhoJanela;
    double limiteZ;
};
