#include <iostream>
#include <fstream>
#include <sstream>
#include "outlier_filter.h"
#include <xlsxwriter.h>

int main() {
    std::ifstream arquivo("data/sensor_data.csv");

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo de entrada.\n";
        return 1;
    }

    lxw_workbook  *workbook  = workbook_new("output/anomaly_report.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *red_format = workbook_add_format(workbook);
    format_set_bg_color(red_format, LXW_COLOR_RED);

    worksheet_write_string(worksheet, 0, 0, "indice", NULL);
    worksheet_write_string(worksheet, 0, 1, "valor", NULL);
    worksheet_write_string(worksheet, 0, 2, "anomalia", NULL);

    const int TAM_JANELA = 20;
    const double LIMITE_Z = 3.0;

    OutlierFilter filtro(TAM_JANELA, LIMITE_Z);

    std::string linha;
    int indice = 0;
    int row = 1;

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        double valor;

        while (ss >> valor) {
            bool anomalia = filtro.isAnomaly(valor);

            lxw_format *fmt = anomalia ? red_format : NULL;

            worksheet_write_number(worksheet, row, 0, indice, fmt);
            worksheet_write_number(worksheet, row, 1, valor, fmt);
            worksheet_write_number(worksheet, row, 2, anomalia ? 1 : 0, fmt);

            filtro.addSample(valor);
            indice++;
            row++;

            if (ss.peek() == ',') ss.ignore();
        }
    }

    workbook_close(workbook);
    std::cout << "Relatório Excel gerado em output/anomaly_report.xlsx\n";
}

