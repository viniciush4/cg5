# Nome do compilador
CC = g++ -std=c++11

# Opções de compilação
CPPFLAGS = -Wall -g

# Ligação com as bibliotecas
LDFLAGS = -lm -lglut -lGLU -lGL

# wildcard é utilizado para listar os arquivos-fonte no diretório atual
FONTES = $(wildcard *.cpp)

# gera a lista dos arquivos-objeto usando a lista de arquivos-fonte
OBJETOS = $(FONTES:.cpp=.o)

# nome do arquivo executável
EXECUTAVEL = trabalhocg

all: $(EXECUTAVEL)

$(EXECUTAVEL): $(OBJETOS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $^
clean:
	rm -rf *.o *~ trabalhocg

rmproper: clean
	rm -rf $(EXECUTAVEL)

run: $(EXECUTAVEL)
	./$(EXECUTAVEL)


