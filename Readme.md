# Guerra de Aviões 3D

Trata-se de um jogo desenvolvido como trabalho final da disciplina de Computação Gráfica da Universidade Federal do Espírito Santo.

<img src="https://github.com/viniciush4/cg5/blob/master/videos/3pessoa.gif" />

<img src="https://github.com/viniciush4/cg5/blob/master/videos/base.gif" />

<img src="https://github.com/viniciush4/cg5/blob/master/videos/bombas.gif" />

<img src="https://github.com/viniciush4/cg5/blob/master/videos/cam_superior.gif" />

<img src="https://github.com/viniciush4/cg5/blob/master/videos/canhao.gif" />

<img src="https://github.com/viniciush4/cg5/blob/master/videos/decolagem.gif" />

<img src="https://github.com/viniciush4/cg5/blob/master/videos/derrota.gif" />

<img src="https://github.com/viniciush4/cg5/blob/master/videos/inimigo.gif" />

## Iniciando


As instruções a seguir te auxiliarão a obter uma cópia do projeto em execução na sua máquina local para fins de desenvolvimento e teste.

### Pré-requisitos

Você precisará ter instalada em sua máquina a biblioteca GLUT, além de um compilador C++. Para a biblioteca GLUT, veja [codebind.com](http://www.codebind.com/linux-tutorials/install-opengl-ubuntu-linux/ "codebind.com"):

```
$ sudo apt-get update
$ sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

Para o compilador de C++, veja [linuxconfig.org](https://linuxconfig.org/how-to-install-g-the-c-compiler-on-ubuntu-18-04-bionic-beaver-linux "linuxconfig.org"):

```
$ sudo apt install g++
$ sudo apt install build-essential
```

### Instalando

Baixe o conteúdo do repositório e rode o makefile na raiz do projeto (isso irá compilar e linkar os códigos, gerando um programa chamado *trabalhocg*):

```
$ make
```

Execute o programa passando o caminho relativo (em relação à raiz do projeto) de onde se encontra o arquivo de configurações *config.xml*:

```
$ ./trabalhocg ./Test_1/
```

## Jogando:

| Function name             | Description                            |
| -------------             | ------------------------------         |
| `1`                       | Câmera no cokpit do avião              |
| `2`                       | Câmera em cima do canhão               |
| `3`                       | Câmera atrás do jogador                |
| `5`                       | Câmera em cima da base inimiga         |
| `6`                       | Câmera atrás de um inimigo             |
| `9`                       | Habilitar desenho das câmeras 1 (cokpit do avião) e 2 (em cima do canhão) |
| `y`                       | Ativar modo conferência do minimapa    |
| `t`                       | Desativar modo conferência do minimapa |
| `e + MOUSE RIGHT BUTTON`  | Rotacionar Câmera 3 (atrás do jogador), câmera 5 (em cima da base inimiga) e câmera 6 (atrás de um inimigo)       |
| `MOUSE SCROLL BUTTON`     | Zoom na câmera 3 (atrás do jogador), câmera 5 (em cima da base inimiga)  e câmera 6 (atrás de um inimigo)       |
| `u`                 | Decolar       |
| `a,s,d,w`           | Mover       |
| `n`                 | Anoitecer       |
| `m`                 | Acende / Apaga farol       |
| `MOUE LEFT BUTTON`  | Atirar       |
| `MOUSE RIGHT BUTTON`| Bombardear       |
| `r`                 | Reiniciar       |
