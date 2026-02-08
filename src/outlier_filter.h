#ifndef OUTLIER_FILTER_H
#define OUTLIER_FILTER_H

#include <vector>

class OutlierFilter {
public:
    OutlierFilter(int janela, double limite_z);
    bool isAnomaly(const std::vector<double>& janela, double valor);

private:
    int tamanho_janela;
    double limite_z;

    double media(const std::vector<double>& dados);
    double desvio(const std::vector<double>& dados, double m);
};

#endif
