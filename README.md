🎮 Jogo da Velha N x N em C

Um jogo da velha desenvolvido em linguagem C, com suporte a tabuleiros de 3x3 e 4x4, para dois jogadores. O projeto utiliza alocação dinâmica de memória, modularização de funções, manipulação de ponteiros, validação de entradas e persistência de pontuações em arquivos .txt.

📌 Sobre o projeto

O objetivo do projeto é implementar uma versão expandida do tradicional Jogo da Velha, permitindo partidas em diferentes tamanhos de tabuleiro.

Dois jogadores participam alternadamente, utilizando as marcas X e O. Vence aquele que conseguir preencher completamente uma linha, uma coluna ou uma das duas diagonais do tabuleiro.

Além da dinâmica do jogo, o programa mantém um placar acumulado entre execuções, armazenando os resultados em um arquivo de texto identificado pelos nomes dos jogadores.

✨ Funcionalidades
Dois jogadores: permite informar o nome dos participantes.
Tabuleiros dinâmicos: escolha entre os formatos 3x3 e 4x4.
Identificação dos jogadores: jogador 1 utiliza X e jogador 2 utiliza O.
Alternância de turnos: os jogadores realizam suas jogadas alternadamente.
Condição de vitória: completa uma linha, coluna ou diagonal inteira.
Detecção de empate: identifica quando todas as casas estão preenchidas sem um vencedor.
Validação de entradas: impede nomes vazios, números inválidos, posições fora do tabuleiro e jogadas em casas ocupadas.
Alocação dinâmica: utiliza matriz de ponteiros (char **) para armazenar o tabuleiro.
Gerenciamento de memória: libera a memória alocada ao final de cada partida.
Placar persistente: salva e recupera as vitórias dos jogadores em arquivos de texto.
Revanche: permite iniciar novas partidas sem reiniciar o programa.
📏 Regras do jogo
Os jogadores informam seus nomes, com limite de 20 caracteres.
O jogador 1 utiliza a marca X e começa a partida.
O jogador 2 utiliza a marca O.
Antes de cada partida, é possível escolher o tamanho do tabuleiro:
3x3: versão clássica do jogo da velha.
4x4: versão expandida do jogo.
Cada jogador informa a linha e a coluna desejadas para realizar sua jogada.
Não é permitido jogar em posições ocupadas ou fora dos limites do tabuleiro.
Vence quem preencher completamente uma linha, coluna, diagonal principal ou diagonal secundária.
Caso todas as casas sejam preenchidas sem que nenhum jogador vença, a partida termina empatada.
As vitórias são contabilizadas no placar acumulado.
🛠️ Tecnologias e conceitos utilizados
Tecnologia / Conceito	Aplicação no projeto
Linguagem C	Desenvolvimento da aplicação
Structs	Organização dos dados dos jogadores
Ponteiros	Compartilhamento e manipulação de dados entre funções
Ponteiros de ponteiros	Representação da matriz do tabuleiro
Alocação dinâmica	Utilização de malloc() e free()
Manipulação de strings	Leitura e validação dos nomes
Manipulação de arquivos	Persistência do placar com fopen(), fprintf() e fgets()
Modularização	Separação das responsabilidades em funções
Validação de entrada	Utilização de fgets(), strtol() e isspace()
📋 Requisitos

Para compilar e executar o projeto, é necessário:

Compilador C compatível com o padrão utilizado pelo código.
GCC (GNU Compiler Collection).
Terminal ou prompt de comando.
Sistema operacional Windows ou Linux.
🚀 Como executar
1. Clone o repositório
bash
git clone https://github.com/SEU-USUARIO/SEU-REPOSITORIO.git

2. Acesse a pasta do projeto
bash
cd SEU-REPOSITORIO

3. Compile o código

Utilizando GCC:

Windows:

bash
gcc main.c -o tictactoe.exe


Linux:

bash
gcc main.c -o tictactoe


Opcionalmente, para habilitar avisos adicionais do compilador:

bash
gcc -Wall -Wextra -std=c11 main.c -o tictactoe

4. Execute o programa

Windows:

powershell
.\tictactoe.exe


Linux:

bash
./tictactoe

💾 Persistência do placar

O placar é armazenado em um arquivo de texto cujo nome é construído a partir dos nomes dos dois jogadores.

Exemplo de arquivo gerado:

text
Joao_X_Maria.txt


Conteúdo do arquivo:

text
Joao | Vitórias: 3
Maria | Vitórias: 2


O funcionamento da persistência segue três etapas:

Carregamento: ao iniciar o programa, o sistema verifica se já existe um arquivo de placar para os dois jogadores.
Atualização: quando um jogador vence, sua quantidade de vitórias é incrementada e o placar é salvo.
Persistência: ao encerrar o programa, o placar final é gravado novamente, preservando os resultados para futuras execuções.

Caso seja uma primeira partida entre os jogadores, o placar começa com zero vitórias para ambos.

🧩 Organização das funções

O código foi desenvolvido de maneira modular, dividindo as responsabilidades em diferentes funções.

Função	Responsabilidade
limparEntrada()	Descarta caracteres excedentes da entrada padrão.
lerInteiro()	Lê e valida valores inteiros dentro de um intervalo.
lerNome()	Lê e valida os nomes dos jogadores.
alocarTabuleiro()	Aloca dinamicamente a matriz do tabuleiro.
liberarTabuleiro()	Libera a memória utilizada pelo tabuleiro.
imprimirTabuleiro()	Exibe o tabuleiro formatado no terminal.
casaDisponivel()	Verifica se uma posição está disponível.
registrarJogada()	Registra a marca do jogador no tabuleiro.
verificarVitoria()	Verifica se existe uma combinação vencedora.
escolherTamanhoTabuleiro()	Permite escolher entre os tabuleiros 3x3 e 4x4.
construirCaminhoDoPlacar()	Constrói o nome do arquivo de pontuação.
extrairVitorias()	Recupera a quantidade de vitórias de uma linha do arquivo.
imprimirPlacar()	Exibe o placar dos jogadores.
salvarPlacar()	Salva os resultados em arquivo.
carregarPlacar()	Recupera os resultados de partidas anteriores.
jogarPartida()	Controla o fluxo completo de uma partida.
main()	Inicializa o programa e controla o fluxo principal.
🖥️ Exemplo de utilização

Ao iniciar o programa, será apresentada a tela inicial:

text
=================================
        JOGO DA VELHA N x N
   3x3 | 4x4  -  X  vs  O
=================================

Nome do jogador 1 (franco 'X'): Joao
Nome do jogador 2 (franco 'O'): Maria

========== PLACAR ==========
Joao | Vitórias: 0
Maria | Vitórias: 0
=============================

Escolha o tabuleiro:
  1) 3x3
  2) 4x4
Opcao:


Após escolher o tabuleiro, os jogadores começam a realizar suas jogadas, informando as coordenadas de linha e coluna.

🎯 Objetivos de aprendizagem

Este projeto foi desenvolvido com o propósito de praticar e consolidar conceitos fundamentais da linguagem C, especialmente:

Manipulação de ponteiros e ponteiros de ponteiros.
Alocação e liberação dinâmica de memória.
Criação e utilização de estruturas (struct).
Modularização e organização do código.
Manipulação e validação de entradas do usuário.
Manipulação de arquivos para armazenamento persistente de dados.
Implementação de estruturas condicionais e de repetição.
Desenvolvimento de algoritmos para verificação de condições de vitória.
📌 Considerações finais

O Jogo da Velha N x N é um projeto que combina lógica de programação com conceitos importantes da linguagem C. Sua implementação permite explorar desde operações básicas com matrizes até gerenciamento de memória dinâmica e persistência de dados em arquivos.

Além de proporcionar uma experiência interativa para dois jogadores, o projeto demonstra a aplicação prática de boas práticas de programação, como modularização, validação de entradas e organização das responsabilidades do código.

Desenvolvido em linguagem C.
