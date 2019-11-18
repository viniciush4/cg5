all: tinyxml2 configuracao pista arena inimigo
	g++ main.cpp -o trabalhocg tinyxml2.o configuracao.o pista.o arena.o -lglut -lGLU -lGL -std=c++11
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

clean:
	@rm -f *.o trabalhocg