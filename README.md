# 🎮 Jogo da Velha N x N em C

Um jogo da velha desenvolvido em **linguagem C**, com suporte a tabuleiros de **3x3 e 4x4**, para **dois jogadores**. O projeto utiliza alocação dinâmica de memória, modularização de funções, manipulação de ponteiros (e ponteiros de ponteiros), validação robusta de entradas e **persistência do placar em arquivo `.txt`**, incluindo o total de partidas e o número de empates.

---

## 📌 Sobre o projeto

O objetivo do projeto é implementar uma versão expandida do tradicional Jogo da Velha, permitindo partidas em diferentes tamanhos de tabuleiro.

Dois jogadores participam alternadamente, utilizando as marcas **X** e **O**. Vence aquele que conseguir preencher completamente uma **linha**, uma **coluna** ou uma das **diagonais** do tabuleiro.

Além da dinâmica do jogo, o programa mantém um placar acumulado entre execuções, armazenando **vitórias**, **total de partidas** e **empates** em um arquivo de texto identificado pelos nomes dos jogadores.

---

## ✨ Funcionalidades

- **Dois jogadores** — informam os nomes logo no início da execução (jogador 1 = **X**, jogador 2 = **O**);
- **Tabuleiros dinâmicos** — escolha entre **3x3** (clássico) e **4x4** (expandido);
- **Alocação dinâmica** — o tabuleiro é uma matriz de ponteiros (`char **`), alocada com `malloc()` e liberada com `free()`;
- **Comunicação por ponteiros** — as funções recebem `Jogador *`, `char **` e `char ***`;
- **Validação robusta de entrada** — impede nomes vazios, nomes com mais de 10 caracteres, números inválidos, valores fora do intervalo, posições fora do tabuleiro e casas ocupadas;
- **Detecção de vitória e empate** — reconhece linha, coluna, diagonal principal e secundária completas, além do empate (velha);
- **Placar persistente** — vitórias, total de partidas e empates são gravados e recarregados entre execuções;
- **Revanche** — permite jogar quantas partidas quiser sem reiniciar o programa.

---

## 📏 Regras do jogo

1. Os jogadores informam seus nomes, com **limite de 10 caracteres** (os nomes não podem ser iguais);
2. O **jogador 1** utiliza a marca **X** e começa a partida;
3. O **jogador 2** utiliza a marca **O**;
4. Antes de cada partida, é escolhido o tamanho do tabuleiro:
   - **3x3** — versão clássica do jogo da velha;
   - **4x4** — versão expandida do jogo;
5. Cada jogador informa a **linha** e a **coluna** desejadas (valores de 1 até o tamanho do tabuleiro);
6. Não é permitido jogar em casas ocupadas ou fora dos limites — a jogada é recusada e o jogador repete;
7. **Vence** quem preencher completamente uma linha, coluna, diagonal principal ou secundária;
8. Se todas as casas forem preenchidas sem vencedor, a partida **empatou** (velha).

---

## 🛠️ Tecnologias e conceitos utilizados

| Tecnologia / Conceito | Aplicação no projeto |
|---|---|
| Linguagem C | Desenvolvimento da aplicação |
| `struct` | Organização dos dados dos jogadores (`Jogador`) e estatísticas (`Estatisticas`) |
| Ponteiros | Compartilhamento e manipulação de dados entre funções |
| Ponteiros de ponteiros | Matriz do tabuleiro (`char **`) e liberação via `char ***` |
| Alocação dinâmica | `malloc()` e `free()` para o tabuleiro |
| Manipulação de strings | Leitura e validação dos nomes |
| Manipulação de arquivos | Persistência do placar com `fopen()`, `fprintf()` e `fgets()` |
| Modularização | Separação das responsabilidades em funções |
| Validação de entrada | `fgets()`, `strtol()` e `isspace()` |

---

## 📋 Requisitos

- Compilador C compatível com C99 ou superior;
- GCC (GNU Compiler Collection);
- Terminal ou prompt de comando;
- Windows ou Linux.

---

## 🚀 Como executar

### 1. Acesse a pasta do projeto

```bash
cd TicTacToe
```

### 2. Compile o código

**Windows:**

```bash
gcc main.c -o tictactoe.exe
```

**Linux:**

```bash
gcc main.c -o tictactoe
```

Opcionalmente, para habilitar avisos adicionais do compilador:

```bash
gcc -Wall -Wextra main.c -o tictactoe.exe
```

Usuários do **VS Code**: basta abrir a pasta do projeto e usar `Ctrl+Shift+B` — o `.vscode/tasks.json` compila automaticamente.

### 3. Execute o programa

**Windows:**

```powershell
.\tictactoe.exe
```

**Linux:**

```bash
./tictactoe
```

> ℹ️ **Windows + acentos**: o programa ajusta automaticamente o console para UTF-8 ao iniciar (`chcp 65001`), então nomes e o rótulo "Vitórias" aparecem corretamente.

---

## 💾 Persistência do placar

O placar é armazenado em um arquivo de texto cujo nome é construído a partir dos nomes dos dois jogadores.

**Exemplo de arquivo gerado:** `Joao_X_Maria.txt`

**Conteúdo do arquivo:**

```text
Joao | Vitórias: 3
Maria | Vitórias: 2
Total de partidas: 5 | Empates: 1
```

O funcionamento da persistência segue três etapas:

1. **Carregamento** — ao iniciar o programa, o sistema verifica se já existe um arquivo de placar para os dois jogadores (vitórias, partidas e empates são recuperados);
2. **Atualização** — o total de partidas é incrementado a cada partida; vitórias são somadas ao vencedor e empates à sua própria estatística; o placar é salvo logo após cada partida;
3. **Persistência** — ao encerrar o programa, o placar final é gravado novamente, preservando os resultados para futuras execuções.

Caso seja uma primeira partida entre os dois jogadores, o placar começa zerado.

---

## 🧩 Organização das funções

O código é modular, com cada função responsável por uma única responsabilidade:

| Função | Responsabilidade |
|---|---|
| `limparEntrada()` | Descarta caracteres excedentes da entrada padrão |
| `lerInteiro()` | Lê e valida valores inteiros dentro de um intervalo |
| `lerNome()` | Lê e valida os nomes dos jogadores (até 10 caracteres) |
| `alocarTabuleiro()` | Aloca dinamicamente a matriz do tabuleiro |
| `liberarTabuleiro()` | Libera a memória utilizada pelo tabuleiro (via `char ***`) |
| `imprimirTabuleiro()` | Exibe o tabuleiro formatado no terminal |
| `casaDisponivel()` | Verifica se uma posição está disponível |
| `registrarJogada()` | Registra a marca do jogador no tabuleiro |
| `verificarVitoria()` | Verifica se existe uma combinação vencedora |
| `escolherTamanhoTabuleiro()` | Permite escolher entre os tabuleiros 3x3 e 4x4 |
| `construirCaminhoDoPlacar()` | Constrói o nome do arquivo de pontuação |
| `extrairNumero()` | Recupera um valor numérico de uma linha do arquivo |
| `imprimirPlacar()` | Exibe o placar dos jogadores e as estatísticas |
| `salvarPlacar()` | Salva os resultados em arquivo |
| `carregarPlacar()` | Recupera os resultados de partidas anteriores |
| `jogarPartida()` | Controla o fluxo completo de uma partida |
| `main()` | Inicializa o programa e controla o fluxo principal |

---

## 🖥️ Exemplo de utilização

Ao iniciar o programa, é apresentada a tela inicial:

```text
=================================
        JOGO DA VELHA N x N
   3x3 | 4x4  -  X  vs  O
=================================

Nome do jogador 1 (franco 'X'): Joao
Nome do jogador 2 (franco 'O'): Maria

========== PLACAR ==========
Joao | Vitórias: 0
Maria | Vitórias: 0
Total de partidas: 0 | Empates: 0
============================

Escolha o tabuleiro:
  1) 3x3
  2) 4x4
Opcao:
```

Após escolher o tabuleiro, os jogadores realizam suas jogadas informando as coordenadas de linha e coluna.

---

## 🎯 Objetivos de aprendizagem

Este projeto foi desenvolvido com o propósito de praticar e consolidar os conceitos fundamentais da linguagem C, especialmente:

- Manipulação de ponteiros e ponteiros de ponteiros;
- Alocação e liberação dinâmica de memória;
- Criação e utilização de estruturas (`struct`);
- Modularização e organização do código;
- Validação robusta de entradas do usuário;
- Manipulação de arquivos para armazenamento persistente de dados;
- Estruturas condicionais e de repetição;
- Algoritmos para verificação de condições de vitória.

---

## 📌 Considerações finais

O Jogo da Velha N x N combina lógica de programação com conceitos importantes da linguagem C, explorando desde operações básicas com matrizes até gerenciamento de memória dinâmica e persistência de dados em arquivos.

Além de proporcionar uma experiência interativa para dois jogadores, o projeto demonstra a aplicação prática de boas práticas, como modularização, validação de entradas e organização das responsabilidades do código.

> ⚠️ **Aviso:** em computadores com política de Controle de Aplicativo do Windows (WDAC/Intune) exigindo binários assinados, o executável gerado localmente pode ser bloqueado pelo sistema.

---

Desenvolvido em linguagem C. 🧡
