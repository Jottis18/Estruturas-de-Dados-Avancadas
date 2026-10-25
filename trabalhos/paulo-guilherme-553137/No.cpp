#include "No.hpp"

No::No(long long chave, No* prox, No* retorno)
    : chave(chave), prox(prox), retorno(retorno), mods(), nmods(0) {}

No* No::proxNaVersao(int versao) const {
    // As modificações estão em ordem crescente de versão: vale a última
    // feita até `versao` ou, se não houver nenhuma, o valor original.
    No* resultado = prox;
    for (int i = 0; i < nmods && mods[i].versao <= versao; i++)
        resultado = mods[i].prox;
    return resultado;
}

No* No::proxAtual() const {
    return nmods == 0 ? prox : mods[nmods - 1].prox;
}

bool No::cheio() const {
    return nmods == MAX_MODS;
}

void No::registrarMod(int versao, No* novoProx) {
    mods[nmods++] = Modificacao{versao, novoProx};
}
