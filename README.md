🛰️ Detecção de Anomalias em Sensores Industriais (C++ + Qt)

Sistema completo para detecção automática de anomalias em dados de sensores industriais, com:

Processamento estatístico em tempo real (streaming)

Interface gráfica (GUI) em Qt

Geração automática de relatório Excel

Estrutura preparada para expansão (drift, ruído, classificação, ML futuramente)

O projeto simula o mesmo princípio utilizado em sistemas reais de diagnóstico de sensores em ambientes industriais e manutenção preditiva.

🎯 Objetivo do Projeto

Este projeto demonstra, de forma prática e profissional:

Como detectar falhas iniciais em sensores

Como tratar ruídos e interferências

Como fazer monitoramento contínuo em streaming

Como gerar relatórios automáticos

Como integrar processamento estatístico + interface gráfica

🧠 Conceito Estatístico Aplicado (Diferencial do Projeto)

Diferente de abordagens ingênuas que:

❌ Recalculam média/desvio incluindo o próprio valor avaliado
❌ Têm complexidade O(n²)
❌ Contaminam a estatística com o outlier

Este sistema:

✅ Avalia a anomalia antes de inserir na janela
✅ Usa janela deslizante eficiente O(1)
✅ Processa dados em streaming, amostra por amostra
✅ Aplica o método estatístico Z-Score real de sistemas industriais

🖥️ Interface Gráfica (Qt GUI)

A GUI permite:

Visualizar os valores do sensor em gráfico

Destacar visualmente as anomalias

Executar o detector de forma interativa

Tornar o projeto visual e profissional

📁 Estrutura Atual do Projeto
sensor-anomaly-detection/
│
├── data/
│   └── sensor_data.csv
│
├── output/
│   └── anomaly_report.xlsx (gerado automaticamente)
│
├── src/
│   ├── main.cpp
│   ├── gui_main.cpp
│   ├── outlier_filter.cpp
│   ├── outlier_filter.h
│   └── sensor_generator.cpp
│
├── detector_gui.pro
├── sensor-anomaly-detection.pro
└── README.md

🛠 Tecnologias Utilizadas

C++17

Qt 6 (Widgets)

MSYS2 UCRT64

MinGW

libxlsxwriter (geração do Excel)

Git e GitHub

⚙️ Ambiente Necessário (MSYS2 UCRT64)

⚠️ MUITO IMPORTANTE: usar UCRT64, não MINGW64

Abra o terminal:

MSYS2 UCRT64


Instale as dependências:

pacman -S mingw-w64-ucrt-x86_64-qt6 \
         mingw-w64-ucrt-x86_64-libxlsxwriter \
         mingw-w64-ucrt-x86_64-toolchain \
         mingw-w64-ucrt-x86_64-make

▶️ Como Compilar o Projeto (forma correta)

Na raiz do projeto:

qmake detector_gui.pro
mingw32-make


Isso irá gerar:

release/detector_gui.exe

▶️ Como Executar
./release/detector_gui.exe


A interface gráfica abrirá.

🧪 O que acontece quando o programa roda

Lê data/sensor_data.csv

Processa os dados em streaming

Detecta anomalias com Z-Score

Exibe o gráfico na GUI

Gera automaticamente:

output/anomaly_report.xlsx

📊 Relatório Excel Gerado

O Excel contém:

Índice da amostra

Valor do sensor

Indicação de anomalia

Linhas anômalas destacadas em vermelho

Abra com Microsoft Excel para visualizar corretamente.

🧮 Algoritmo Utilizado

Z-Score em janela deslizante:

𝑧
=
𝑥
−
𝜇
𝜎
z=
σ
x−μ
	​


Regra:

|z| > 3  →  Anomalia

🚀 Como Gerar Novos Dados de Sensor

O projeto possui um gerador de sensores:

g++ src/sensor_generator.cpp -o sensor_generator
./sensor_generator


Isso atualiza o arquivo:

data/sensor_data.csv

🧩 Possibilidades de Expansão

O projeto foi estruturado para permitir facilmente:

Detecção de drift

Classificação de tipo de falha

Integração com banco de dados

Integração com IA / Machine Learning

Dashboard industrial

✅ Status Atual
Recurso	Status
Detector estatístico	✅
Janela deslizante eficiente	✅
GUI Qt funcional	✅
Geração automática de Excel	✅
Gerador de dados de sensor	✅
Estrutura profissional de projeto	✅
👨‍💻 Autor

Projeto desenvolvido para fins acadêmicos e demonstração prática de técnicas reais de diagnóstico de sensores industriais.