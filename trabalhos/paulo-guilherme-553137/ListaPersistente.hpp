#ifndef LISTA_PERSISTENTE_HPP
#define LISTA_PERSISTENTE_HPP

#include <deque>
#include <optional>
#include <ostream>
#include <utility>
#include <vector>

#include "No.hpp"

// Lista encadeada ordenada com persistência parcial: todas as versões podem
// ser consultadas, mas só a mais recente pode ser alterada. A versão 0 é a
// lista vazia e cada inclusão ou remoção cria uma versão nova.
class ListaPersistente {
public:
    ListaPersistente();

    // Os nós apontam uns para os outros dentro de `nos`; copiar a lista
    // deixaria a cópia apontando para os nós da original.
    ListaPersistente(const ListaPersistente&) = delete;
    ListaPersistente& operator=(const ListaPersistente&) = delete;

    // Cria uma versão nova com `chave` incluída (mesmo que já exista).
    void inserir(long long chave);

    // Cria uma versão nova sem uma ocorrência de `chave`. Se a chave não
    // estiver na lista, a versão nova é igual à anterior.
    void remover(long long chave);

    // Menor chave estritamente maior que `chave` na versão `versao`, ou
    // nada se não houver. Versão inexistente vira a mais recente.
    std::optional<long long> sucessor(long long chave, long long versao) const;

    // Escreve as chaves da versão `versao` em ordem crescente, separadas por
    // ", ", seguidas de uma quebra de linha. Versão inexistente vira a mais
    // recente.
    void imprimir(long long versao, std::ostream& saida) const;

    // Número da versão mais recente.
    int versaoMaisRecente() const;

private:
    std::deque<No> nos;       // todos os nós já criados; o deque é o dono da memória
    std::vector<No*> raizes;  // raizes[v] = primeiro nó da lista na versão v

    // Converte o número de versão da entrada em uma versão existente.
    int resolverVersao(long long versao) const;

    // Aloca um nó novo em `nos`, sem modificações.
    No* criarNo(long long chave, No* prox, No* retorno);

    // Abre a próxima versão, começando igual à mais recente.
    void novaVersao();

    // Na versão mais recente, devolve {anterior, atual}: `atual` é o primeiro
    // nó com chave >= `chave` (nulo se não houver) e `anterior` é o nó antes
    // dele (nulo se `atual` for o primeiro).
    std::pair<No*, No*> buscar(long long chave) const;

    // Faz `anterior` apontar para `novo` na versão mais recente; se
    // `anterior` é nulo, `novo` passa a ser o primeiro nó.
    void ligar(No* anterior, No* novo);
};

#endif
