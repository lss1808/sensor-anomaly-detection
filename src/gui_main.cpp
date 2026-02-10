#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QPainter>
#include <vector>
#include <cstdlib>
#include "outlier_filter.h"

class GraficoWidget : public QWidget {
    std::vector<double> dados;
    std::vector<int> anomalias;

public:
    GraficoWidget(const std::vector<double>& d, const std::vector<int>& a)
        : dados(d), anomalias(a) {
        setMinimumSize(800, 600);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.fillRect(rect(), Qt::white);

        if (dados.empty()) return;

        int w = width();
        int h = height();

        double maxVal = *std::max_element(dados.begin(), dados.end());
        double minVal = *std::min_element(dados.begin(), dados.end());

        // Desenha linha do sinal
        p.setPen(Qt::blue);
        for (size_t i = 1; i < dados.size(); ++i) {
            int x1 = (i-1) * w / dados.size();
            int y1 = h - ((dados[i-1] - minVal) / (maxVal - minVal)) * h;

            int x2 = i * w / dados.size();
            int y2 = h - ((dados[i] - minVal) / (maxVal - minVal)) * h;

            p.drawLine(x1, y1, x2, y2);
        }

        // Desenha anomalias
        p.setBrush(Qt::red);
        for (int idx : anomalias) {
            int x = idx * w / dados.size();
            int y = h - ((dados[idx] - minVal) / (maxVal - minVal)) * h;
            p.drawEllipse(QPoint(x, y), 5, 5);
        }
    }
};

class Janela : public QWidget {
    std::vector<double> dados;
    std::vector<int> indices;

public:
    Janela() {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QPushButton *btnAnalisar = new QPushButton("Analisar Dados");
        QPushButton *btnTabela = new QPushButton("Mostrar Tabela");
        QPushButton *btnGrafico = new QPushButton("Mostrar Grafico");

        layout->addWidget(btnAnalisar);
        layout->addWidget(btnTabela);
        layout->addWidget(btnGrafico);

        connect(btnAnalisar, &QPushButton::clicked, this, &Janela::analisar);
        connect(btnTabela, &QPushButton::clicked, this, &Janela::tabela);
        connect(btnGrafico, &QPushButton::clicked, this, &Janela::grafico);

        for (int i = 0; i < 200; ++i) {
            double v = rand() % 100;
            if (i % 40 == 0) v += 150;
            dados.push_back(v);
        }
    }

    void analisar() {
        indices.clear();
        OutlierFilter filtro(30, 2.5);
        for (int i = 0; i < (int)dados.size(); ++i) {
            if (filtro.isAnomaly(dados[i]))
                indices.push_back(i);
        }
    }

    void tabela() {
        QTableWidget *t = new QTableWidget(indices.size(), 2);
        t->setHorizontalHeaderLabels({"Indice", "Valor"});
        for (int i = 0; i < (int)indices.size(); ++i) {
            t->setItem(i,0,new QTableWidgetItem(QString::number(indices[i])));
            t->setItem(i,1,new QTableWidgetItem(QString::number(dados[indices[i]])));
        }
        t->show();
    }

    void grafico() {
        GraficoWidget *g = new GraficoWidget(dados, indices);
        g->show();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Janela j;
    j.show();
    return app.exec();
}
