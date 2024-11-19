# Bin-Packing
O objetivo do bin-packing é minimizar o número de bins necessários para acomodar um conjunto de itens com pesos variados, respeitando a capacidade máxima de cada bin.

## Formulação Inteira Mista
### Conjuntos e Parâmetros
- $$N$$: Número de itens.
- $$C$$: Capacidade de cada bin.
- $$P_j$$: Tamanho (peso) do item $$\( j \), \( \forall j \in \{1, 2, \dots, N\} \)$$.

### Variáveis de Decisão
- $$x_i$$: Variável binária que indica se o bin $$i$$ é utilizado.
- $$y_{i,j}$$: Variável binária que indica se o item $$j$$ é atribuído ao bin $$i$$.

### Formulação Matemática

**Função Objetivo:** $$\\min \sum_{i=1}^N x_i\$$

**Sujeito a:**

1. **Restrição de Capacidade:**
   $$\sum_{j=1}^N P_j y_{i,j} \leq C x_, \quad \forall i \in \{1, 2, \dots, N\}\$$

2. **Restrição de Atribuição:**
   $$\sum_{i=1}^N y_{i,j} = 1 \quad \forall j \in \{1, 2, \dots, N\}\$$

3. **Restrição Binária:**
   $$x_i \in \{0, 1\} \quad \forall i \in \{1, 2, \dots, N\} \quad\quad$$
   $$y_{i,j} \in \{0, 1\} \quad \forall i, j \in \{1, 2, \dots, N\}\$$

## Iterated Local Search (ILS) para o Problema de Bin Packing
### Abordagem
A solução utiliza uma combinação de heurísticas de inicialização, busca local e perturbação para explorar o espaço de soluções.  

1. **Solução Inicial**: É gerada uma solução viável usando a heurística *Best Fit*.  

2. **Busca Local**: Otimiza a solução inicial com base em trocas e redistribuições de itens entre os bins, garantindo viabilidade e redução do número de bins usados.  

3. **Perturbação**: Perturba a solução atual para escapar de ótimos locais, promovendo maior diversidade na busca.  

4. **Iteração**: Repete as etapas de busca local e perturbação, refinando a solução ao longo de várias iterações.  

### Uso
Para executar o programa:  ```./ils <filename> <num_iterations> <seed>```

### Bibliografia
Baseado nas ideias apresentadas no artigo "Local Search for the Bin Packing Problem" de Fred Glover, Adriana C. F. Alvim, Celso C. Ribeiro.
