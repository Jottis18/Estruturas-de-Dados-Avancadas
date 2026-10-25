# Estruturas de Dados Avançadas - Trabalho 01: Persistência Parcial

## Equipe

Paulo Guilherme de Almeida Silva (matrícula 553137)

## Descrição

Lista encadeada ordenada (versão da graduação) com persistência parcial, feita pelo método da cópia de nós visto em sala.
Cada nó guarda até 2 modificações do seu ponteiro `prox`, cada uma marcada com a versão em que foi feita, e um ponteiro de retorno para o seu antecessor na versão mais recente.
Quando um nó já tem 2 modificações e precisa de outra, ele é copiado e a cópia é ligada ao antecessor, o que pode se propagar até o início da lista.

- **Linguagem de programação utilizada:** C++17, compilado com `g++ -std=c++17`. Testado com g++ 14.2 e GNU Make 4.4.1 no Debian 13 (Linux) e com Apple clang 17 no macOS.
- **Compilar:** `make build` gera o executável `programa`.
- **Executar:** `make run INPUT=arquivo.in`. O `run` compila antes, se necessário, e escreve no terminal apenas a saída do programa.
- **Testar:** `make teste` recompila o programa, roda com cada `.in` da pasta `testes/` do repositório e compara com o `.out` correspondente.
- **Limpar:** `make clean` apaga o executável.

## Arquivos

| Arquivo | Conteúdo |
|---|---|
| `No.hpp`, `No.cpp` | constante `MAX_MODS` e estruturas `Modificacao` e `No` |
| `ListaPersistente.hpp`, `ListaPersistente.cpp` | classe `ListaPersistente` |
| `main.cpp` | leitura da entrada e execução dos comandos |
| `Makefile` | regras `build`, `run`, `teste` e `clean` |

## Estruturas e funções

### Constante `MAX_MODS`

- Definida em `No.hpp`.
- Número máximo de modificações por nó: 2p, onde p = 1 é o maior número de ponteiros que apontam para um mesmo nó (numa lista simplesmente encadeada, só o antecessor aponta para ele).

### `struct Modificacao`

- Definida em `No.hpp`.
- Uma modificação do ponteiro `prox` de um nó.
- O campo `versao` é a versão a partir da qual a modificação vale.
- O campo `prox` é o novo valor do ponteiro.
- Não guarda qual campo foi modificado porque `prox` é o único campo de um nó que muda.

### `struct No`

- Definida em `No.hpp` e implementada em `No.cpp`.
- Nó da lista persistente.
- O campo `chave` é o valor inteiro guardado no nó (`long long`, 64 bits). Nunca muda.
- O campo `prox` é o ponteiro para o próximo nó no momento em que o nó foi criado.
- O campo `retorno` é o ponteiro de retorno: o antecessor do nó na versão mais recente, ou nulo se o nó é o primeiro. Só vale para a versão mais recente e é atualizado no lugar, sem histórico.
- O campo `mods[MAX_MODS]` guarda as modificações de `prox`, em ordem crescente de versão.
- O campo `nmods` é o número de posições ocupadas em `mods`.
- O construtor `No(chave, prox, retorno)` cria o nó sem modificações.
- A função `proxNaVersao(versao)` devolve o valor de `prox` na versão pedida: a última modificação com versão menor ou igual a `versao` ou, se não houver, o valor original. Olha no máximo 2 modificações, então é O(1).
- A função `proxAtual()` devolve o valor de `prox` na versão mais recente: a última modificação ou, se não houver, o valor original.
- A função `cheio()` indica se todas as posições de `mods` estão ocupadas.
- A função `registrarMod(versao, novoProx)` acrescenta uma modificação ao final de `mods`. Só é chamada em nós que não estão cheios.

### `class ListaPersistente`

- Definida em `ListaPersistente.hpp` e implementada em `ListaPersistente.cpp`.
- Guarda a lista ordenada e todas as suas versões. A versão 0 é a lista vazia e cada inclusão ou remoção cria uma versão nova.
- O campo `nos` é um `std::deque<No>` com todos os nós já criados. Ele é o dono da memória: o deque não move os elementos quando cresce, então os ponteiros entre nós continuam válidos, e tudo é liberado no fim do programa.
- O campo `raizes` é o vetor de identificadores de versão: `raizes[v]` é o primeiro nó da lista na versão `v` (nulo se a lista está vazia nessa versão).
- A cópia da classe é proibida (construtor de cópia e atribuição removidos), porque a cópia ficaria apontando para os nós da lista original.
- Funções públicas:
  - O construtor `ListaPersistente()` cria a versão 0, vazia.
  - A função `inserir(chave)` implementa o `INC`: cria uma versão nova e inclui `chave` antes do primeiro elemento maior ou igual a ela. Chaves repetidas são incluídas de novo.
  - A função `remover(chave)` implementa o `REM`: cria uma versão nova e retira a primeira ocorrência de `chave`. Se a chave não existe, a versão nova é igual à anterior.
  - A função `sucessor(chave, versao)` implementa o `SUC`: percorre a versão pedida e devolve a primeira chave estritamente maior que `chave`, ou `std::nullopt` se não houver (impresso como `inf`).
  - A função `imprimir(versao, saida)` implementa o `IMP`: escreve as chaves da versão pedida separadas por `", "`, seguidas de uma quebra de linha (só a quebra de linha, se a versão está vazia).
  - A função `versaoMaisRecente()` devolve o número da versão mais recente.
- Funções privadas:
  - A função `resolverVersao(versao)` devolve `versao` se ela existe e, caso contrário (negativa ou maior que a mais recente), a versão mais recente.
  - A função `criarNo(chave, prox, retorno)` aloca um nó em `nos` e devolve um ponteiro para ele.
  - A função `novaVersao()` acrescenta a próxima versão a `raizes`, começando com a mesma raiz da versão anterior.
  - A função `buscar(chave)` percorre a versão mais recente e devolve o par `{anterior, atual}`: `atual` é o primeiro nó com chave maior ou igual a `chave` e `anterior` é o nó antes dele (cada um pode ser nulo).
  - A função `ligar(anterior, novo)` faz `anterior` apontar para `novo` na versão mais recente (ou torna `novo` o primeiro nó, se `anterior` é nulo) e mantém os ponteiros de retorno. Se `anterior` está cheio, cria uma cópia dele que já aponta para `novo` e repete o processo com o antecessor do nó copiado; é a cópia em cascata do método.

### Funções de `main.cpp`

- A função `lerInteiro(texto, valor)` converte um campo da linha em `long long`. Falha se o campo não é um número inteiro ou não cabe em 64 bits.
- A função `executar(campos, lista, saida)` executa um comando (`INC`, `REM`, `SUC` ou `IMP`) já separado em campos e escreve a saída dele. Devolve `false` se a linha não é um comando válido.
- A função `main(argc, argv)` abre o arquivo passado como argumento (ou lê da entrada padrão, se não houver argumento), lê a entrada linha a linha e executa cada comando.

## Como a persistência funciona

- **Leitura:** o valor de `prox` na versão `v` é o da última modificação com versão menor ou igual a `v` ou, se não houver, o valor original. Para percorrer a versão `v`, começa-se em `raizes[v]` e segue-se `proxNaVersao(v)`.
- **Escrita:** só acontece na versão mais recente. Se o nó tem espaço, acrescenta-se uma modificação marcada com a versão nova. Se está cheio, cria-se uma cópia dele com o valor novo em `prox` e nenhuma modificação. Em seguida, o antecessor do nó (achado pelo ponteiro de retorno) passa a apontar para a cópia, o que é uma nova escrita e pode gerar outra cópia. Se o nó copiado era o primeiro, a cópia vira `raizes[v]`. O nó antigo fica intacto e continua servindo às versões anteriores.
- **Versões:** cada `INC` ou `REM` cria exatamente uma versão, e todas as modificações feitas por ele são marcadas com o número dela. Nenhum nó é modificado duas vezes na mesma operação, então não foi preciso usar versões parciais: o número de versão guardado nas modificações é o mesmo usado no arquivo de entrada.

## Complexidade

Sejam `n` o tamanho da lista e `v` o número de versões.

- **Leitura de um campo em qualquer versão:** O(1), pois cada nó tem no máximo 2 modificações.
- **Escrita de um campo:** O(1) amortizado. Cada `INC` ou `REM` chama `ligar` no máximo uma vez, e `ligar` cria no máximo uma modificação, então há no máximo `v` modificações no total. Cada cópia substitui um nó cheio (com 2 modificações), e um nó substituído não está mais na versão mais recente, então nunca é copiado de novo. Logo, o total de cópias é no máximo `v/2`. Uma operação isolada pode copiar a lista inteira, mas a média por operação é O(1). É o mesmo resultado do argumento de potencial visto em sala, com o potencial proporcional ao número de modificações nos nós da versão mais recente.
- **`INC` e `REM`:** O(n) para achar a posição, como numa lista encadeada comum, mais O(1) amortizado da persistência.
- **`SUC` e `IMP`:** O(n) na versão pedida, com custo O(1) por nó visitado.
- **Espaço:** cada `INC` cria um nó e há no máximo `v/2` cópias, então existem no máximo 1,5·`v` nós, cada um de tamanho constante, mais o vetor `raizes` com `v` posições. O total é O(v).

## Decisões em casos que o enunciado não especifica

- `SUC` com versão inexistente usa a versão mais recente, como o `IMP`.
- Com chaves repetidas, `INC` insere antes das chaves iguais e `REM` remove a primeira ocorrência. Como as ocorrências são iguais, isso não muda a saída.
- A linha ecoada por `SUC` e `IMP` é refeita a partir dos campos lidos, então espaços extras e o `\r` de arquivos do Windows não aparecem na saída.
- Linhas em branco são puladas. Linhas inválidas são ignoradas com um aviso na saída de erro (`stderr`), sem afetar a saída padrão.
