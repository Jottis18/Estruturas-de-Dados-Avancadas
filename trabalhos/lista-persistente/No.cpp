#include "No.hpp"

No::No(long long chave, No* prox) : chave(chave), prox(prox), anterior(nullptr), mods(), num_mods(0) {}

// Valor de `prox` na versão dada: a última modificação feita até essa versão,
// ou o valor original se nenhuma se aplica.
No* No::prox_na_versao(int versao) const {
    No* valor = prox;
    for (int i = 0; i < num_mods && mods[i].versao <= versao; i++) {
        valor = mods[i].valor;
    }
    return valor;
}

No* No::prox_atual() const {
    return num_mods > 0 ? mods[num_mods - 1].valor : prox;
}

bool No::cheio() const {
    return num_mods == MAX_MODS;
}
