#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QDir>

#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "outlier_filter.h"

// ---------------- WIDGET DO GRÁFICO ----------------
class GraficoWidget : public QWidget {
public:
    std::vector<double> dados;
    std::vector<int> anomalias;

    void setDados(const std::vector<double>& d, const std::vector<int>& a) {
        dados = d;
        anomalias = a;
        qDebug() << "Gráfico recebeu" << dados.size() << "pontos e" << anomalias.size() << "anomalias";
        update();
    }

protected:
    void paintEvent(QPaintEvent*) override {
        if (dados.empty()) {
            qDebug() << "Gráfico vazio - nenhum dado para desenhar";
            return;
        }

        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(rect(), Qt::white);

        int margem = 50;
        int w = width() - 2 * margem;
        int h = height() - 2 * margem;

        double min = *std::min_element(dados.begin(), dados.end());
        double max = *std::max_element(dados.begin(), dados.end());
        
        qDebug() << "Valores - Min:" << min << "Max:" << max;
        
        double range = max - min;
        min -= range * 0.1;
        max += range * 0.1;

        auto yMap = [&](double v) {
            return margem + h - ((v - min) / (max - min)) * h;
        };

        // Eixos
        p.setPen(QPen(Qt::gray, 2));
        p.drawLine(margem, margem, margem, margem + h);
        p.drawLine(margem, margem + h, margem + w, margem + h);

        // Labels
        p.setFont(QFont("Arial", 10));
        p.setPen(Qt::black);
        p.drawText(5, margem + 5, QString::number(max, 'f', 1));
        p.drawText(5, margem + h, QString::number(min, 'f', 1));
        p.drawText(margem - 10, height() - 5, "0");
        p.drawText(margem + w - 40, height() - 5, QString::number(dados.size()));

        // Linha do sinal
        p.setPen(QPen(Qt::blue, 2));
        for (size_t i = 1; i < dados.size(); ++i) {
            int x1 = margem + ((i - 1) * w) / (dados.size() - 1);
            int x2 = margem + (i * w) / (dados.size() - 1);
            p.drawLine(x1, yMap(dados[i - 1]), x2, yMap(dados[i]));
        }

        // Anomalias
        p.setPen(QPen(Qt::red, 2));
        p.setBrush(Qt::red);
        for (int idx : anomalias) {
            if (idx >= 0 && idx < (int)dados.size()) {
                int x = margem + (idx * w) / (dados.size() - 1);
                int y = yMap(dados[idx]);
                p.drawEllipse(QPoint(x, y), 8, 8);
                
                // Label da anomalia
                p.drawText(x + 10, y - 5, QString("Idx:%1\nVal:%2")
                    .arg(idx).arg(dados[idx], 0, 'f', 1));
            }
        }

        // Título
        p.setFont(QFont("Arial", 12, QFont::Bold));
        p.drawText(margem, 25, QString("Dados: %1 | Anomalias: %2")
            .arg(dados.size()).arg(anomalias.size()));
    }
};

// ---------------- JANELA PRINCIPAL ----------------
class Janela : public QWidget {
    Q_OBJECT

public:
    Janela() {
        QVBoxLayout *layout = new QVBoxLayout(this);

        infoLabel = new QLabel("👉 Clique em 'Analisar Dados' para começar");
        infoLabel->setStyleSheet("padding: 15px; background-color: #e0e0e0; "
                                 "border-radius: 5px; font-size: 14px;");

        QPushButton *btnAnalisar = new QPushButton("🔍 Analisar Dados");
        QPushButton *btnTabela   = new QPushButton("📊 Mostrar Tabela");
        QPushButton *btnGrafico  = new QPushButton("📈 Mostrar Gráfico");

        btnAnalisar->setStyleSheet("background-color: #4CAF50; color: white; "
                                   "padding: 12px; font-weight: bold; font-size: 14px;");
        btnTabela->setStyleSheet("background-color: #2196F3; color: white; "
                                "padding: 12px; font-size: 14px;");
        btnGrafico->setStyleSheet("background-color: #FF9800; color: white; "
                                 "padding: 12px; font-size: 14px;");

        tabela = new QTableWidget();
        tabela->setVisible(false);
        
        grafico = new GraficoWidget();
        grafico->setMinimumHeight(400);

        layout->addWidget(infoLabel);
        layout->addWidget(btnAnalisar);
        layout->addWidget(btnTabela);
        layout->addWidget(btnGrafico);
        layout->addWidget(tabela);
        layout->addWidget(grafico);

        connect(btnAnalisar, &QPushButton::clicked, this, &Janela::analisarDados);
        connect(btnTabela, &QPushButton::clicked, this, &Janela::mostrarTabela);
        connect(btnGrafico, &QPushButton::clicked, this, &Janela::mostrarGrafico);
        
        qDebug() << "=== APLICATIVO INICIADO ===";
    }

private:
    std::vector<double> dados;
    std::vector<int> indicesAnomalias;
    QTableWidget *tabela;
    GraficoWidget *grafico;
    QLabel *infoLabel;

    std::vector<double> lerCSV() {
        std::vector<double> valores;
        
        // Lista de caminhos possíveis
        std::vector<std::string> caminhos = {
            "data/sensor_data.csv",
            "../data/sensor_data.csv",
            "../../data/sensor_data.csv",
            "./sensor_data.csv"
        };
        
        std::ifstream arq;
        std::string caminhoUsado;
        
        for (const auto& caminho : caminhos) {
            qDebug() << "Tentando abrir:" << QString::fromStdString(caminho);
            arq.open(caminho);
            if (arq.is_open()) {
                caminhoUsado = caminho;
                qDebug() << "✓ ARQUIVO ABERTO:" << QString::fromStdString(caminho);
                break;
            }
        }

        if (!arq.is_open()) {
            qDebug() << "❌ ERRO: Não conseguiu abrir nenhum caminho!";
            QMessageBox::critical(nullptr, "ERRO - Arquivo não encontrado",
                "Não conseguiu abrir sensor_data.csv em nenhum dos caminhos:\n" +
                QString::fromStdString(caminhos[0]) + "\n" +
                QString::fromStdString(caminhos[1]) + "\n" +
                QString::fromStdString(caminhos[2]) + "\n" +
                QString::fromStdString(caminhos[3]));
            return valores;
        }

        std::string linha;
        int linhaNum = 0;
        
        while (std::getline(arq, linha)) {
            linhaNum++;
            if (!linha.empty()) {
                try {
                    double v = std::stod(linha);
                    valores.push_back(v);
                } catch (const std::exception& e) {
                    qDebug() << "Erro na linha" << linhaNum << ":" << QString::fromStdString(linha);
                }
            }
        }

        qDebug() << "✓ Lidos" << valores.size() << "valores do arquivo";
        
        if (valores.size() > 0) {
            qDebug() << "Primeiros 5 valores:" << valores[0] << valores[1] 
                     << valores[2] << valores[3] << valores[4];
        }

        return valores;
    }

private slots:
    void analisarDados() {
        qDebug() << "\n=== INICIANDO ANÁLISE ===";
        
        dados = lerCSV();
        
        if (dados.empty()) {
            QMessageBox::critical(this, "ERRO", 
                "Nenhum dado foi lido!\n\n"
                "Verifique se o arquivo data/sensor_data.csv existe.");
            qDebug() << "❌ ANÁLISE ABORTADA - Sem dados";
            return;
        }

        qDebug() << "Total de dados lidos:" << dados.size();
        
        indicesAnomalias.clear();

        const int TAM_JANELA = 30;
        const double LIMITE_Z = 2.5;
        OutlierFilter filtro(TAM_JANELA, LIMITE_Z);

        qDebug() << "Configuração: Janela=" << TAM_JANELA << "Z-score=" << LIMITE_Z;

        std::vector<double> janela;
        int anomaliasCount = 0;

        for (size_t i = 0; i < dados.size(); ++i) {
            janela.push_back(dados[i]);

            if (janela.size() > (size_t)TAM_JANELA) {
                janela.erase(janela.begin());
            }

            if (janela.size() == (size_t)TAM_JANELA) {
                bool anomalia = filtro.isAnomaly(janela, dados[i]);
                
                if (anomalia) {
                    indicesAnomalias.push_back(i);
                    anomaliasCount++;
                    qDebug() << "⚠ Anomalia #" << anomaliasCount 
                             << "no índice" << i << "valor:" << dados[i];
                }
            }
        }

        qDebug() << "✓ ANÁLISE COMPLETA - Anomalias encontradas:" << indicesAnomalias.size();

        infoLabel->setText(QString("✓ Análise Concluída!\n"
                                   "Total de leituras: %1 | Anomalias detectadas: %2")
            .arg(dados.size()).arg(indicesAnomalias.size()));
        
        infoLabel->setStyleSheet("padding: 15px; background-color: #4CAF50; "
                                "color: white; border-radius: 5px; "
                                "font-weight: bold; font-size: 14px;");

        QMessageBox::information(this, "Análise Completa",
            QString("Dados processados: %1\nAnomalias encontradas: %2\n\n"
                    "Clique em 'Mostrar Gráfico' para visualizar!")
                .arg(dados.size()).arg(indicesAnomalias.size()));

        mostrarGrafico();
    }

    void mostrarTabela() {
        qDebug() << "\n=== MOSTRANDO TABELA ===";
        
        if (dados.empty()) {
            QMessageBox::warning(this, "Aviso", 
                "Execute a análise primeiro!\n\n"
                "Clique no botão 'Analisar Dados'");
            return;
        }

        qDebug() << "Montando tabela com" << dados.size() << "linhas";

        tabela->setVisible(true);
        tabela->clear();
        tabela->setRowCount(dados.size());
        tabela->setColumnCount(3);
        tabela->setHorizontalHeaderLabels({"Índice", "Valor do Sensor", "Status"});

        for (size_t i = 0; i < dados.size(); ++i) {
            bool anom = std::find(indicesAnomalias.begin(),
                                  indicesAnomalias.end(), i) != indicesAnomalias.end();

            tabela->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
            tabela->setItem(i, 1, new QTableWidgetItem(QString::number(dados[i], 'f', 4)));
            tabela->setItem(i, 2, new QTableWidgetItem(anom ? "⚠ ANOMALIA" : "✓ Normal"));

            if (anom) {
                for (int c = 0; c < 3; c++) {
                    tabela->item(i, c)->setBackground(QColor(255, 200, 200));
                    tabela->item(i, c)->setForeground(QColor(200, 0, 0));
                    QFont font = tabela->item(i, c)->font();
                    font.setBold(true);
                    tabela->item(i, c)->setFont(font);
                }
            }
        }

        tabela->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tabela->setAlternatingRowColors(true);
        
        qDebug() << "✓ Tabela montada com sucesso";
    }

    void mostrarGrafico() {
        qDebug() << "\n=== MOSTRANDO GRÁFICO ===";
        
        if (dados.empty()) {
            QMessageBox::warning(this, "Aviso", 
                "Execute a análise primeiro!\n\n"
                "Clique no botão 'Analisar Dados'");
            return;
        }

        grafico->setDados(dados, indicesAnomalias);
        qDebug() << "✓ Gráfico atualizado";
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    qDebug() << "Diretório de trabalho:" << QDir::currentPath();
    
    Janela j;
    j.setWindowTitle("🔬 Detector de Anomalias em Sensores v3.0 [DEBUG]");
    j.resize(1100, 800);
    j.show();
    
    return app.exec();
}

#include "gui_main.moc"