# Sistema de Alocação de Máquinas

## Descrição

Este projeto foi desenvolvido para a disciplina de Tópicos Especiais em Inteligência Artificial e tem como objetivo otimizar a distribuição de lotes de produção entre máquinas industriais, minimizando o *makespan* e equilibrando a carga de trabalho por meio de um Algoritmo Genético.

Foi implementado o algoritmo **Genético (Genetic Algorithm - GA)**, uma meta-heurística inspirada nos processos de evolução natural, capaz de explorar diferentes soluções ao longo de várias gerações até encontrar uma distribuição eficiente dos lotes entre as máquinas.

## Problema

Uma fábrica de calçados possui um setor responsável pela colagem de solas, realizado por máquinas industriais.

Durante um turno de produção:

- Existem entre **2 e 5 máquinas**
- São processados entre **10 e 30 lotes**
- Cada lote contém entre **10 e 50 sapatos**
- O tempo de colagem varia entre **1 e 25 segundos por sapato**

## Restrições

- Cada máquina processa um lote por vez
- Um lote deve ser completamente finalizado antes do próximo
- Cada lote é atribuído a apenas uma máquina

## Objetivo

Distribuir os lotes entre as máquinas de forma que:

- O trabalho fique balanceado
- O tempo total de produção (*makespan*) seja minimizado

---

## Modelagem

### Representação Computacional

#### Lote

- ID
- Quantidade de sapatos
- Tempo de colagem de cada sapato
- Tempo total de processamento

#### Indivíduo

Cada indivíduo representa uma possível solução para o problema.

O habitante é composto por um vetor onde:

- Cada posição representa um lote
- O valor armazenado representa a máquina responsável por aquele lote

Exemplo:

```text
[0, 2, 1, 0, 1]
```

Significa:

- Lote 0 → Máquina 0
- Lote 1 → Máquina 2
- Lote 2 → Máquina 1
- Lote 3 → Máquina 0
- Lote 4 → Máquina 1

## Função de Avaliação

O desempenho de cada indivíduo é medido pelo:

### Makespan

Maior tempo total de processamento entre todas as máquinas.

```text
minimizar max(tempo_maquina)
```

Quanto menor o makespan, melhor a solução.

## Algoritmo Genético

O algoritmo é composto pelas seguintes etapas:

### 1. Geração Inicial

É criada uma população inicial aleatória contendo diversas soluções candidatas.

### 2. Avaliação

Cada indivíduo tem seu fitness calculado através do makespan.

### 3. Seleção por Torneio

São sorteados grupos de indivíduos e o melhor de cada grupo é escolhido para reprodução.

### 4. Crossover

Dois indivíduos selecionados geram novos descendentes através do cruzamento de seus cromossomos.

Foi utilizado **crossover de um ponto**, onde parte dos genes é herdada de um pai e o restante do outro.

### 5. Mutação

Alguns genes possuem uma pequena probabilidade de serem alterados aleatoriamente.

Essa etapa aumenta a diversidade genética da população e evita convergência prematura.

### 6. Elitismo

O melhor indivíduo da geração anterior é preservado na próxima geração, garantindo que a qualidade da solução não seja perdida.

### 7. Critério de Parada

O processo é repetido durante um número fixo de gerações.

Ao final, o melhor indivíduo encontrado é retornado como solução.

## Parâmetros Utilizados

| Parâmetro | Valor |
|-----------|--------|
| Tamanho da população | 10 |
| Número de gerações | 100 |
| Tamanho do torneio | 3 |
| Taxa de crossover | 80% |
| Taxa de mutação | 5% |

---

## Tecnologias Utilizadas

- C++
- HTML
- CSS
- JavaScript
- JSON

---

## Visualização

A aplicação gera um arquivo JSON contendo:

- Dados do problema (máquinas e lotes)
- População de cada geração
- Eventos de seleção por torneio
- Operações de crossover
- Eventos de mutação
- Melhor indivíduo de cada geração

Essas informações são utilizadas pela interface web para demonstrar visualmente o funcionamento do algoritmo genético ao longo das gerações.

---

## Como Executar

### 1. Compilar o algoritmo

```bash
g++ main.cpp -o main
```

### 2. Executar

```bash
./main
```

Será gerado o arquivo:

```text
dados.json
```

contendo todos os dados utilizados pela visualização.

### 3. Iniciar servidor local

```bash
python -m http.server 8000
```

### 4. Acessar no navegador

```text
http://localhost:8000
```

---

## Equipe 04

- João Felype Morais Vieira
- Luís Guilherme Ferreira da Costa
- Maria Eduarda Araujo Sales
- Maria Luiza Pereira Sousa