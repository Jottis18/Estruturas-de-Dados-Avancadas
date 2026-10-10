INPUT = entrada.in
TESTES = ../../testes

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
FONTES = main.cpp No.cpp ListaPersistente.cpp
CABECALHOS = No.hpp ListaPersistente.hpp

# Os comandos usam `@` para que a saída padrão de `make run` contenha apenas
# a saída do programa.

.PHONY: build run test clean

# `make build` compila o programa executável `programa`.
build: programa

programa: $(FONTES) $(CABECALHOS)
	@$(CXX) $(CXXFLAGS) -o programa $(FONTES)

# `make run INPUT=arquivo.in` roda o programa com o arquivo de entrada dado.
run: programa
	@./programa $(INPUT)

# `make test` compara a saída do programa com cada saída esperada em $(TESTES).
test: programa
	@falhas=0; \
	for entrada in $(TESTES)/*.in; do \
		if ./programa "$$entrada" | diff -q - "$${entrada%.in}.out" > /dev/null; then \
			echo "OK     $$entrada"; \
		else \
			echo "FALHOU $$entrada"; falhas=$$((falhas + 1)); \
		fi; \
	done; \
	exit $$falhas

clean:
	@rm -f programa
