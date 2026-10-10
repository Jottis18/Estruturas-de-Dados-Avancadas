#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include "ListaPersistente.hpp"

// O TRABALHO.md pede "vírgula e espaço" no IMP, mas as saídas esperadas em testes/
// usam apenas espaço. Seguimos os testes; troque aqui se o professor indicar o contrário.
const char* const SEPARADOR = " ";

std::string sem_espacos_nas_pontas(const std::string& linha) {
    const char* espacos = " \t\r\n";
    size_t inicio = linha.find_first_not_of(espacos);
    if (inicio == std::string::npos) return "";
    size_t fim = linha.find_last_not_of(espacos);
    return linha.substr(inicio, fim - inicio + 1);
}

void processar(std::istream& entrada, std::ostream& saida) {
    ListaPersistente lista;
    std::string linha;
    while (std::getline(entrada, linha)) {
        linha = sem_espacos_nas_pontas(linha);
        std::istringstream campos(linha);
        std::string operacao;
        long long chave, versao;
        if (!(campos >> operacao)) continue;

        if (operacao == "INC" && campos >> chave) {
            lista.inserir(chave);
        } else if (operacao == "REM" && campos >> chave) {
            lista.remover(chave);
        } else if (operacao == "SUC" && campos >> chave >> versao) {
            saida << linha << '\n';
            std::optional<long long> resultado = lista.sucessor(chave, versao);
            if (resultado) {
                saida << *resultado << '\n';
            } else {
                saida << "inf\n";
            }
        } else if (operacao == "IMP" && campos >> versao) {
            saida << linha << '\n';
            const char* separador = "";
            for (long long elemento : lista.elementos(versao)) {
                saida << separador << elemento;
                separador = SEPARADOR;
            }
            saida << '\n';
        }
    }
}

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    if (argc < 2) {
        processar(std::cin, std::cout);
        return 0;
    }
    std::ifstream arquivo(argv[1]);
    if (!arquivo) {
        std::cerr << "Erro: nao foi possivel abrir o arquivo '" << argv[1] << "'\n";
        return 1;
    }
    processar(arquivo, std::cout);
    return 0;
}
