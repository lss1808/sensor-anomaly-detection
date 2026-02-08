#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>

#include "outlier_filter.h"

#include <fstream>
#include <vector>

std::vector<double> carregarCSV(const QString& caminho) {
    std::ifstream file(caminho.toStdString());
    std::vector<double> dados;
    double valor;

    while (file >> valor) {
        dados.push_back(valor);
    }

    return dados;
}

class Janela : public QWidget {
    Q_OBJECT

public:
    Janela() {
        setWindowTitle("Detector de Anomalias - Sensores Industriais");
        resize(700, 500);

        QVBoxLayout* layout = new QVBoxLayout(this);

        QPushButton* btnImportar = new QPushButton("Importar CSV");
        QPushButton* btnAnalisar = new QPushButton("Analisar Dados");
        QPushButton* btnExportar = new QPushButton("Exportar Excel");

        tabela = new QTableWidget();
        tabela->setColumnCount(3);
        tabela->setHorizontalHeaderLabels({"Ponto", "Valor", "Anomalia"});
        tabela->horizontalHeader()->setStretchLastSection(true);

        layout->addWidget(btnImportar);
        layout->addWidget(btnAnalisar);
        layout->addWidget(btnExportar);
        layout->addWidget(tabela);

        connect(btnImportar, &QPushButton::clicked, this, &Janela::importarCSV);
        connect(btnAnalisar, &QPushButton::clicked, this, &Janela::analisarDados);
        connect(btnExportar, &QPushButton::clicked, this, &Janela::exportarExcel);
    }

private:
    QString caminhoCSV;
    std::vector<double> dados;
    QTableWidget* tabela;

    void importarCSV() {
        caminhoCSV = QFileDialog::getOpenFileName(this, "Abrir CSV", "", "CSV (*.csv)");
        if (!caminhoCSV.isEmpty()) {
            dados = carregarCSV(caminhoCSV);
            QMessageBox::information(this, "Sucesso", "CSV carregado com sucesso!");
        }
    }

    void analisarDados() {
        if (dados.empty()) return;

        OutlierFilter filtro(30, 2.5);

        tabela->setRowCount(dados.size());

        for (size_t i = 0; i < dados.size(); i++) {
            bool anomalia = filtro.isAnomaly(dados[i]);

            tabela->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
            tabela->setItem(i, 1, new QTableWidgetItem(QString::number(dados[i])));
            tabela->setItem(i, 2, new QTableWidgetItem(anomalia ? "SIM" : "NÃO"));

            if (anomalia) {
                for (int c = 0; c < 3; c++) {
                    tabela->item(i, c)->setBackground(Qt::red);
                }
            }
        }
    }

    void exportarExcel() {
        QMessageBox::information(this, "Info", "Use o detector normal para gerar o Excel.");
    }
};

#include "gui_main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Janela janela;
    janela.show();
    return app.exec();
}
