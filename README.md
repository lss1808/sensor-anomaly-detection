# Detecção de Anomalias em Sensores Industriais (C++)

Sistema para detecção automática de anomalias em dados de sensores industriais,
com foco em manutenção preditiva e geração de relatório visual em Excel.

O projeto simula o mesmo princípio utilizado em sistemas reais de diagnóstico
de sensores em processos industriais.

---

## 🎯 Foco do Projeto

- Detecção de falhas iniciais
- Identificação de ruídos e interferências
- Monitoramento contínuo do processo
- Base para manutenção preditiva

---

## 🧠 Conceito Aplicado

Diferente de abordagens ingênuas que recalculam média e desvio padrão a cada leitura (O(n²)) e contaminam a análise com o próprio valor avaliado, este projeto:

- Avalia a anomalia **antes** de inserir o valor na janela estatística
- Utiliza **janela deslizante eficiente**
- Processa os dados em modo **streaming**, amostra por amostra
- Aplica o método estatístico **Z-Score**

Esse é o mesmo princípio utilizado em sistemas reais de diagnóstico de sensores.

---

## 🚀 Principais Características

- Detecção de outliers com **Z-Score**
- Janela deslizante eficiente
- Processamento em tempo real
- Leitura robusta de CSV
- Geração automática de relatório Excel
- Destaque visual das anomalias
- Estrutura preparada para expansão (ruído, drift, classificação)

---

## 📁 Estrutura do Projeto

```
sensor-anomaly-detection/
│
├── data/
│   └── sensor_data.csv
│
├── src/
│   ├── main.cpp
│   ├── outlier_filter.cpp
│   └── outlier_filter.h
│
├── output/        # Gerado automaticamente
└── README.md
```

---

## 🛠 Tecnologias Utilizadas

- C++
- MSYS2 / MinGW64
- libxlsxwriter (geração do Excel)
- Git e GitHub

---

## ⚙️ Requisitos

Instalar no MSYS2 MINGW64:

```bash
pacman -S mingw-w64-x86_64-libxlsxwriter
```

---

## ▶️ Como Compilar (na raiz do projeto)

```bash
g++ src/main.cpp src/outlier_filter.cpp -o detector -std=c++17 -lxlsxwriter
```

---

## ▶️ Como Executar

```bash
./detector
```

O programa irá:

1. Ler `data/sensor_data.csv`
2. Detectar anomalias
3. Criar a pasta `output`
4. Gerar automaticamente:

```
output/anomaly_report.xlsx
```

---

## 📊 Relatório Excel Gerado

O Excel contém:

- Índice do ponto
- Valor do sensor
- Indicação de anomalia
- Linhas anômalas destacadas em vermelho

> Caso o Windows abra com outro programa, use **Abrir com → Excel**.

---

## 🧮 Algoritmo Utilizado

Z-Score aplicado em janela deslizante:

\[
z = \frac{x - \mu}{\sigma}
\]

Valores com |z| > 3 são considerados anomalias.

---

## 🚀 Status

✅ Funcionando  
✅ Relatório Excel automático  
✅ Pronto para expansão
