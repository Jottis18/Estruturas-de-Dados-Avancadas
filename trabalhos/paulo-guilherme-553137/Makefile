# make build                 compila o programa
# make run INPUT=arquivo.in  executa o programa com a entrada dada
# make teste                 compara a saída com todos os testes de ../../testes
# make clean                 apaga o executável
#
# Todas as receitas são silenciosas (@) para que `make run` escreva no
# terminal apenas a saída do programa.

# Não escrever "Entering directory" quando chamado com `make -C`.
MAKEFLAGS += --no-print-directory

INPUT = ../../testes/teste05.in

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic

PROGRAMA = programa
FONTES = main.cpp No.cpp ListaPersistente.cpp
CABECALHOS = No.hpp ListaPersistente.hpp
TESTES = ../../testes
COMPILAR = $(CXX) $(CXXFLAGS) -o $(PROGRAMA) $(FONTES)

.PHONY: build run teste clean

# Sempre recompila, para nunca aproveitar um executável compilado em outro
# sistema que tenha vindo junto com os arquivos.
build:
	@$(COMPILAR)

$(PROGRAMA): $(FONTES) $(CABECALHOS)
	@$(COMPILAR)

run: $(PROGRAMA)
	@./$(PROGRAMA) "$(INPUT)"

# Depende de build (e não do executável) para nunca testar uma compilação velha.
teste: build
	@if [ ! -d $(TESTES) ]; then \
	    echo "make teste precisa da pasta de testes do repositório em $(TESTES)"; \
	    exit 1; \
	fi; \
	falhas=0; \
	for entrada in $(TESTES)/*.in; do \
	    esperado="$${entrada%.in}.out"; \
	    if ./$(PROGRAMA) "$$entrada" | cmp -s - "$$esperado"; then \
	        echo "OK      $$entrada"; \
	    else \
	        echo "FALHOU  $$entrada"; \
	        falhas=$$((falhas + 1)); \
	    fi; \
	done; \
	test $$falhas -eq 0

clean:
	@rm -f $(PROGRAMA)
