#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <xlsxwriter.h>
#include "outlier_filter.h"

int main() {
    std::filesystem::create_directory("output");

    std::string caminho_csv = std::filesystem::current_path().string()
                            + "/data/sensor_data.csv";

    std::ifstream arquivo(caminho_csv);
    std::cout << "Lendo arquivo em: " << caminho_csv << "\n";

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo.\n";
        return 1;
    }

    std::vector<double> dados;
    double valor;

    while (arquivo >> valor) {
        dados.push_back(valor);
    }

    const int TAM_JANELA = 20;
    const double LIMITE_Z = 3.0;
    OutlierFilter filtro(TAM_JANELA, LIMITE_Z);

    lxw_workbook  *workbook  = workbook_new("output/anomaly_report.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format_normal = workbook_add_format(workbook);
    lxw_format *format_anomalia = workbook_add_format(workbook);

    format_set_bg_color(format_anomalia, LXW_COLOR_RED);

    worksheet_write_string(worksheet, 0, 0, "Indice", NULL);
    worksheet_write_string(worksheet, 0, 1, "Valor", NULL);
    worksheet_write_string(worksheet, 0, 2, "Anomalia", NULL);

    std::vector<double> janela;

    for (size_t i = 0; i < dados.size(); i++) {
        janela.push_back(dados[i]);

        bool anomalia = false;
        if (janela.size() >= TAM_JANELA) {
            anomalia = filtro.isAnomaly(janela, dados[i]);
            janela.erase(janela.begin());
        }

        lxw_format *fmt = anomalia ? format_anomalia : format_normal;

        worksheet_write_number(worksheet, i + 1, 0, i, fmt);
        worksheet_write_number(worksheet, i + 1, 1, dados[i], fmt);
        worksheet_write_string(worksheet, i + 1, 2,
                               anomalia ? "SIM" : "NAO", fmt);
    }

    workbook_close(workbook);

    std::cout << "Excel gerado em: output/anomaly_report.xlsx\n";
    return 0;
}
