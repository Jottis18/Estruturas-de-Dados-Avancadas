#include "ListaPersistente.hpp"

ListaPersistente::ListaPersistente() {
    raizes.push_back(nullptr);  // versão 0: lista vazia
}

void ListaPersistente::inserir(long long chave) {
    novaVersao();
    auto [anterior, atual] = buscar(chave);
    No* novo = criarNo(chave, atual, anterior);
    if (atual != nullptr)
        atual->retorno = novo;
    ligar(anterior, novo);
}

void ListaPersistente::remover(long long chave) {
    novaVersao();
    auto [anterior, atual] = buscar(chave);
    if (atual == nullptr || atual->chave != chave)
        return;  // nada a remover; a versão nova fica igual à anterior
    ligar(anterior, atual->proxAtual());
}

std::optional<long long> ListaPersistente::sucessor(long long chave, long long versao) const {
    const int v = resolverVersao(versao);
    for (No* no = raizes[v]; no != nullptr; no = no->proxNaVersao(v))
        if (no->chave > chave)
            return no->chave;
    return std::nullopt;
}

void ListaPersistente::imprimir(long long versao, std::ostream& saida) const {
    const int v = resolverVersao(versao);
    const char* separador = "";
    for (No* no = raizes[v]; no != nullptr; no = no->proxNaVersao(v)) {
        saida << separador << no->chave;
        separador = ", ";
    }
    saida << '\n';
}

int ListaPersistente::versaoMaisRecente() const {
    return static_cast<int>(raizes.size()) - 1;
}

int ListaPersistente::resolverVersao(long long versao) const {
    if (versao < 0 || versao > versaoMaisRecente())
        return versaoMaisRecente();
    return static_cast<int>(versao);
}

No* ListaPersistente::criarNo(long long chave, No* prox, No* retorno) {
    nos.emplace_back(chave, prox, retorno);
    return &nos.back();
}

void ListaPersistente::novaVersao() {
    raizes.push_back(raizes.back());
}

std::pair<No*, No*> ListaPersistente::buscar(long long chave) const {
    No* anterior = nullptr;
    No* atual = raizes.back();
    while (atual != nullptr && atual->chave < chave) {
        anterior = atual;
        atual = atual->proxAtual();
    }
    return {anterior, atual};
}

void ListaPersistente::ligar(No* anterior, No* novo) {
    const int v = versaoMaisRecente();

    // Um nó cheio não aceita mais modificações: ele é substituído por uma
    // cópia que já nasce apontando para `novo`, e quem precisa ser ligado
    // agora é a cópia, no antecessor do nó copiado. Isso pode se repetir
    // até achar um nó com espaço ou chegar ao início da lista. O nó copiado
    // continua intacto, servindo às versões antigas.
    while (anterior != nullptr && anterior->cheio()) {
        No* copia = criarNo(anterior->chave, novo, anterior->retorno);
        if (novo != nullptr)
            novo->retorno = copia;
        novo = copia;
        anterior = anterior->retorno;
    }

    if (novo != nullptr)
        novo->retorno = anterior;
    if (anterior == nullptr)
        raizes[v] = novo;
    else
        anterior->registrarMod(v, novo);
}
