# Detecção de Anomalias em Sensores Industriais

Este projeto tem como objetivo detectar automaticamente anomalias em sinais de
sensores industriais, utilizando técnicas estatísticas simples implementadas
em C++.

## Objetivo
Identificar comportamentos anormais (picos, ruído excessivo e falhas iniciais)
em dados de sensores industriais, com foco em manutenção preditiva.
com foco em 
- Detecção de falhas iniciais
- Identificação de ruídos e interferências
- Monitoramento contínuo do processo
- Base para manutenção preditiva

## 🧠 Conceito aplicado

Diferente de abordagens ingênuas que recalculam média e desvio padrão a cada leitura (O(n²)) e contaminam a análise com o próprio valor avaliado, este projeto:

- Avalia a anomalia **antes** de inserir o valor na janela estatística
- Utiliza **buffer circular (`std::deque`)**
- Mantém **soma** e **soma dos quadrados** para cálculo eficiente de média e desvio
- Processa os dados em modo **streaming**, amostra por amostra

Esse é o mesmo princípio utilizado em sistemas reais de diagnóstico de sensores.

## 🚀 Principais características

- Detecção de outliers com **Z-Score**
- Janela deslizante eficiente
- Estatística incremental (sem recomputação total)
- Processamento em tempo real
- Leitura robusta de CSV
- Estrutura preparada para expansão (detecção de ruído, drift e classificação)

## Tecnologias Utilizadas
- C++
- Git e GitHub

Compile **a partir da raiz do projeto**:


g++ src/main.cpp src/outlier_filter.cpp -o detector -std=c++17
./detecto

## Status
🚧 Em desenvolvimento

