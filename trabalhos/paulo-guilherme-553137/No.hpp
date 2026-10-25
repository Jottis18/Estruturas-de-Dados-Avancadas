#ifndef NO_HPP
#define NO_HPP

// Quantas modificações cabem em um nó antes de ele precisar ser copiado.
// O método da cópia de nós usa 2p modificações por nó, onde p é o maior
// número de ponteiros que apontam para um mesmo nó. Numa lista simplesmente
// encadeada só o antecessor aponta para um nó, então p = 1.
constexpr int MAX_MODS = 2;

struct No;

// Modificação do campo `prox` de um nó: a partir da versão `versao`, o campo
// passa a valer `prox`. Como `prox` é o único campo que muda (a chave de um
// nó nunca é alterada), não é preciso guardar qual campo foi modificado.
struct Modificacao {
    int versao;
    No* prox;
};

// Nó da lista ordenada parcialmente persistente.
struct No {
    long long chave;             // valor guardado no nó; nunca muda
    No* prox;                    // valor de `prox` quando o nó foi criado
    No* retorno;                 // antecessor na versão mais recente (nulo se for o primeiro)
    Modificacao mods[MAX_MODS];  // modificações de `prox`, em ordem crescente de versão
    int nmods;                   // quantas posições de `mods` estão ocupadas

    No(long long chave, No* prox, No* retorno);

    // Valor de `prox` na versão `versao`.
    No* proxNaVersao(int versao) const;

    // Valor de `prox` na versão mais recente.
    No* proxAtual() const;

    // Indica se todas as posições de `mods` estão ocupadas.
    bool cheio() const;

    // Registra que, a partir da versão `versao`, `prox` vale `novoProx`.
    // Só pode ser chamada se o nó não estiver cheio.
    void registrarMod(int versao, No* novoProx);
};

#endif
