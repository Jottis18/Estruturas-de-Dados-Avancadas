#include "ListaPersistente.hpp"

ListaPersistente::ListaPersistente() : raizes{nullptr} {}

int ListaPersistente::versao_atual() const {
    return static_cast<int>(raizes.size()) - 1;
}

// A nova versão começa igual à anterior; as modificações seguintes são gravadas nela.
void ListaPersistente::nova_versao() {
    raizes.push_back(raizes.back());
}

No* ListaPersistente::novo_no(long long chave, No* prox) {
    nos.emplace_back(chave, prox);
    return &nos.back();
}

// Faz `no->prox = valor` na versão atual. `no == nullptr` representa a raiz da versão.
//
// Se o nó ainda tem espaço, a mudança vira uma modificação. Se está cheio, é criada
// uma cópia com o novo valor e o predecessor (achado pelo ponteiro de retorno) passa
// a apontar para a cópia, o que pode se propagar até a raiz.
void ListaPersistente::definir_prox(No* no, No* valor) {
    int versao = versao_atual();
    while (true) {
        if (no == nullptr) {
            raizes[versao] = valor;
            if (valor != nullptr) valor->anterior = nullptr;
            return;
        }
        if (!no->cheio()) {
            no->mods[no->num_mods++] = {versao, valor};
            if (valor != nullptr) valor->anterior = no;
            return;
        }
        No* copia = novo_no(no->chave, valor);
        if (valor != nullptr) valor->anterior = copia;
        valor = copia;
        no = no->anterior;
    }
}

void ListaPersistente::inserir(long long chave) {
    nova_versao();
    No* anterior = nullptr;
    No* atual = raizes.back();
    while (atual != nullptr && atual->chave < chave) {
        anterior = atual;
        atual = atual->prox_atual();
    }
    No* novo = novo_no(chave, atual);
    if (atual != nullptr) atual->anterior = novo;
    definir_prox(anterior, novo);
}

void ListaPersistente::remover(long long chave) {
    nova_versao();
    No* anterior = nullptr;
    No* atual = raizes.back();
    while (atual != nullptr && atual->chave < chave) {
        anterior = atual;
        atual = atual->prox_atual();
    }
    if (atual == nullptr || atual->chave != chave) return;
    definir_prox(anterior, atual->prox_atual());
}

// Versões que ainda não existem são tratadas como a versão mais recente.
int ListaPersistente::versao_existente(long long versao) const {
    if (versao < 0 || versao > versao_atual()) return versao_atual();
    return static_cast<int>(versao);
}

std::optional<long long> ListaPersistente::sucessor(long long chave, long long versao) const {
    int v = versao_existente(versao);
    for (No* no = raizes[v]; no != nullptr; no = no->prox_na_versao(v)) {
        if (no->chave > chave) return no->chave;
    }
    return std::nullopt;
}

std::vector<long long> ListaPersistente::elementos(long long versao) const {
    int v = versao_existente(versao);
    std::vector<long long> resultado;
    for (No* no = raizes[v]; no != nullptr; no = no->prox_na_versao(v)) {
        resultado.push_back(no->chave);
    }
    return resultado;
}
