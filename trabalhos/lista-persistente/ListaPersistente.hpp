#ifndef LISTA_PERSISTENTE_HPP
#define LISTA_PERSISTENTE_HPP

#include <deque>
#include <optional>
#include <vector>

#include "No.hpp"

// Lista encadeada ordenada com persistência parcial.
//
// A versão 0 é a lista vazia e cada inserção ou remoção cria a versão seguinte.
// Todas as modificações de campo feitas por uma operação recebem o número da versão
// que ela cria; como cada nó é modificado no máximo uma vez por operação, não é
// preciso um contador de versões interno separado.
class ListaPersistente {
public:
    ListaPersistente();

    void inserir(long long chave);
    void remover(long long chave);
    std::optional<long long> sucessor(long long chave, long long versao) const;
    std::vector<long long> elementos(long long versao) const;
    int versao_atual() const;

private:
    std::deque<No> nos;        // todos os nós já criados; o deque mantém os endereços estáveis
    std::vector<No*> raizes;   // raizes[v] = primeiro nó da lista na versão v

    void nova_versao();
    No* novo_no(long long chave, No* prox);
    void definir_prox(No* no, No* valor);
    int versao_existente(long long versao) const;
};

#endif
