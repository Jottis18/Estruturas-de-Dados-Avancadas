# Estruturas de Dados Avançadas - Trabalho de Persistência Parcial

## Equipe

Paulo Guilherme de Almeida Silva (matrícula 553137)

## Descrição

Lista encadeada ordenada com persistência parcial (graduação), implementada pelo
método de cópia de nós (*node copying*).

- **Linguagem de programação utilizada:** C++17, compilado com `g++` (testado com g++ 13.3).
- **Como usar:**
  - `make build` compila o programa `programa`.
  - `make run INPUT=arquivo.in` roda o programa com o arquivo de entrada dado.
  - `make test` compara a saída do programa com cada `.out` da pasta `testes/` do repositório.

### Ideia geral

- Cada nó tem apenas um ponteiro de entrada (do predecessor, ou da raiz da versão quando é o
  primeiro da lista), então `p = 1` e cada nó guarda `2p = 2` modificações do campo `prox`.
- A chave de um nó nunca muda: inserir cria um nó novo e remover altera o `prox` do predecessor.
- Ao alterar o `prox` de um nó, a mudança é gravada como modificação se houver espaço. Se o nó
  estiver cheio, cria-se uma cópia dele com o valor novo, e o predecessor (achado pelo ponteiro de
  retorno `anterior`) passa a apontar para a cópia. Isso pode se propagar até a raiz da versão.
- Ler o `prox` de um nó numa versão custa O(1), pois há no máximo 2 modificações para olhar.
  Assim, cada operação tem custo adicional multiplicativo O(1) amortizado e o espaço total é O(v).
- O vetor `raizes` guarda o primeiro nó da lista em cada versão.
- Todas as modificações feitas por uma inserção ou remoção recebem o número da versão que ela cria.
  Como cada nó é modificado no máximo uma vez por operação, não é preciso um contador de versões
  interno separado do número de versão do arquivo de entrada.

### Decisões sobre casos não definidos no enunciado

- **Separador do `IMP`:** o `TRABALHO.md` pede vírgula e espaço (`1, 5, 13`), mas as saídas
  esperadas em `testes/` usam só espaço (`1 5 13`). Seguimos os testes. O separador é a
  constante `SEPARADOR` em `main.cpp`.
- **`SUC` em versão inexistente:** usa a versão mais recente, como o enunciado define para o `IMP`.

### Estruturas e funções

- **`Mod`** (definida em `No.hpp`)
  - Uma modificação do campo `prox` de um nó.
  - O campo `versao` é a versão a partir da qual a modificação vale.
  - O campo `valor` é o novo valor de `prox`.

- **`No`** (definida em `No.hpp` e implementada em `No.cpp`)
  - Nó da lista encadeada ordenada parcialmente persistente.
  - O campo `chave` é o valor armazenado no nó; nunca muda.
  - O campo `prox` é o valor original do ponteiro para o próximo nó, válido desde a versão em
    que o nó foi criado.
  - O campo `anterior` é o ponteiro de retorno para o predecessor na versão mais recente. Ele não
    é persistente e só é usado para propagar cópias.
  - O campo `mods[]` guarda até `MAX_MODS = 2` modificações de `prox`, em ordem crescente de
    versão. O campo `num_mods` diz quantas estão em uso.
  - O construtor `No(chave, prox)` cria um nó sem modificações.
  - A função `prox_na_versao(versao)` devolve o valor de `prox` na versão dada: a última
    modificação com versão menor ou igual a ela, ou o valor original.
  - A função `prox_atual()` devolve o valor mais recente de `prox`.
  - A função `cheio()` diz se não há mais espaço para modificações.

- **`ListaPersistente`** (definida em `ListaPersistente.hpp` e implementada em `ListaPersistente.cpp`)
  - A lista persistente, com todas as suas versões.
  - O campo `nos` é um `std::deque<No>` com todos os nós já criados. O deque mantém os
    endereços dos nós estáveis e libera a memória ao final.
  - O campo `raizes[]` guarda, para cada versão `v`, o primeiro nó da lista nessa versão.
    A versão 0 é a lista vazia.
  - A função `inserir(chave)` implementa o `INC`: cria uma nova versão e insere um nó novo antes
    do primeiro nó com chave maior ou igual.
  - A função `remover(chave)` implementa o `REM`: cria uma nova versão e, se houver um nó com
    a chave, faz o predecessor apontar para o sucessor dele.
  - A função `sucessor(chave, versao)` implementa o `SUC`: percorre a lista na versão e devolve
    a primeira chave estritamente maior, ou nada (impresso como `inf`).
  - A função `elementos(versao)` implementa o `IMP`: devolve as chaves da versão em ordem crescente.
  - A função `versao_atual()` devolve o número da versão mais recente.
  - A função `nova_versao()` cria uma versão igual à mais recente, onde as modificações seguintes
    são gravadas.
  - A função `novo_no(chave, prox)` cria um nó em `nos` e devolve um ponteiro para ele.
  - A função `definir_prox(no, valor)` faz `no->prox = valor` na versão atual (`no == nullptr`
    representa a raiz), gravando uma modificação ou copiando o nó e propagando a cópia ao
    predecessor.
  - A função `versao_existente(versao)` troca versões que não existem pela mais recente.

- **Programa principal** (em `main.cpp`)
  - A constante `SEPARADOR` é o texto impresso entre as chaves no `IMP`.
  - A função `sem_espacos_nas_pontas(linha)` remove espaços e `\r` do início e do fim da linha.
  - A função `processar(entrada, saida)` lê cada linha da entrada, executa a operação na lista e
    imprime o resultado de `SUC` e `IMP`.
  - A função `main` abre o arquivo passado como argumento (ou lê da entrada padrão se não houver
    argumento) e chama `processar`.
