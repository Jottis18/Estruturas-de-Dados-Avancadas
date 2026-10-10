#ifndef NO_HPP
#define NO_HPP

struct No;

// Uma modificação do campo `prox` de um nó, válida a partir de `versao`.
struct Mod {
    int versao;
    No* valor;
};

// Nó da lista encadeada ordenada parcialmente persistente (método de cópia de nós).
//
// Cada nó tem um único ponteiro de entrada (p = 1): o do seu predecessor, ou a raiz
// da versão quando é o primeiro da lista. Por isso guarda 2p = 2 modificações.
struct No {
    static constexpr int MAX_MODS = 2;

    long long chave;     // valor armazenado; nunca muda
    No* prox;            // valor original de `prox`, válido desde a versão de criação do nó
    No* anterior;        // ponteiro de retorno para o predecessor na versão mais recente
    Mod mods[MAX_MODS];  // modificações de `prox`, em ordem crescente de versão
    int num_mods;

    No(long long chave, No* prox);

    No* prox_na_versao(int versao) const;
    No* prox_atual() const;
    bool cheio() const;
};

#endif
