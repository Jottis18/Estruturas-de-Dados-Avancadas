#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "ListaPersistente.hpp"

namespace {

// Converte `texto` em inteiro. Falha se o texto não for um número inteiro
// ou se o número não couber em um long long.
bool lerInteiro(const std::string& texto, long long& valor) {
    char* fim = nullptr;
    errno = 0;
    valor = std::strtoll(texto.c_str(), &fim, 10);
    return !texto.empty() && *fim == '\0' && errno != ERANGE;
}

// Executa o comando de uma linha da entrada, já separada em campos, e
// escreve em `saida` o que o comando imprime. Devolve false se a linha não
// for um comando válido.
bool executar(const std::vector<std::string>& campos, ListaPersistente& lista,
              std::ostream& saida) {
    const std::string& comando = campos[0];
    long long n = 0;
    long long m = 0;

    if (comando == "INC" && campos.size() == 2 && lerInteiro(campos[1], n)) {
        lista.inserir(n);
    } else if (comando == "REM" && campos.size() == 2 && lerInteiro(campos[1], n)) {
        lista.remover(n);
    } else if (comando == "SUC" && campos.size() == 3 && lerInteiro(campos[1], n) &&
               lerInteiro(campos[2], m)) {
        saida << "SUC " << campos[1] << ' ' << campos[2] << '\n';
        if (std::optional<long long> resposta = lista.sucessor(n, m))
            saida << *resposta << '\n';
        else
            saida << "inf\n";
    } else if (comando == "IMP" && campos.size() == 2 && lerInteiro(campos[1], m)) {
        saida << "IMP " << campos[1] << '\n';
        lista.imprimir(m, saida);
    } else {
        return false;
    }
    return true;
}

}  // namespace

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);

    // Lê do arquivo passado como argumento ou, sem argumento, da entrada padrão.
    std::ifstream arquivo;
    if (argc > 1) {
        arquivo.open(argv[1]);
        if (!arquivo) {
            std::cerr << "Erro: não foi possível abrir o arquivo '" << argv[1] << "'\n";
            return 1;
        }
    }
    std::istream& entrada = argc > 1 ? arquivo : std::cin;

    ListaPersistente lista;
    std::string linha;
    for (int numero = 1; std::getline(entrada, linha); numero++) {
        std::istringstream leitor(linha);
        std::vector<std::string> campos;
        for (std::string campo; leitor >> campo;)
            campos.push_back(campo);
        if (campos.empty())
            continue;  // linha em branco
        if (!executar(campos, lista, std::cout))
            std::cerr << "Aviso: linha " << numero << " ignorada (comando inválido): " << linha
                      << '\n';
    }
    return 0;
}
