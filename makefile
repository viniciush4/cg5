all: tinyxml2 configuracao pista arena inimigo base jogador
	g++ main.cpp -o trabalhocg tinyxml2.o configuracao.o pista.o arena.o inimigo.o base.o jogador.o -lglut -lGLU -lGL -std=c++11
	@echo "Done."

tinyxml2: tinyxml2.cpp
	g++ -c tinyxml2.cpp -std=c++11

configuracao: configuracao.cpp
	g++ -c configuracao.cpp -std=c++11

pista: pista.cpp
	g++ -c pista.cpp -std=c++11

arena: arena.cpp
	g++ -c arena.cpp -std=c++11

inimigo: inimigo.cpp
	g++ -c inimigo.cpp -std=c++11

base: base.cpp
	g++ -c base.cpp -std=c++11

jogador: jogador.cpp
	g++ -c jogador.cpp -std=c++11

clean:
	@rm -f *.o trabalhocg