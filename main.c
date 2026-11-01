/*
 * ============================================================================
 *  JOGO DA VELHA N x N  (tabuleiros 3x3 e 4x4)
 * ============================================================================
 *  Caracteristicas:
 *    - Pede o nome dos 2 jogadores (1 e 2), com no maximo 20 caracteres;
 *    - Marcacao das casas com 'X' (jogador 1) e 'O' (jogador 2);
 *    - Regra: vence quem completar uma linha, coluna ou diagonal inteira
 *      (no 3x3 isso corresponde ao jogo da velha classico);
 *    - Tabuleiro alocado dinamicamente (matriz via ponteiro de ponteiro);
 *    - Codigo modularizado, com ponteiros trocando informacoes entre funcoes;
 *    - Validacao robusta de TODA a entrada do usuario;
 *    - Placar salvo em arquivo "nome1_X_nome2.txt", no formato de tabela:
 *
 *          nome do jogador 1 | Vitorias: xxx
 *          nome do jogador 2 | Vitorias: xxx
 *
 *  Compilacao:  gcc main.c -o tictactoe.exe
 *  Execucao:    .\tictactoe.exe
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define MAX_NOME    20      /* tamanho maximo do nome do jogador          */
#define TAM_LINHA  256      /* buffer auxiliar para leitura de linhas     */

#define ROTULO_VITORIAS "Vitórias:"   /* rotulo usado no arquivo do placar  */

/* ---------------------------------------------------------------------------
 *  ESTRUTURAS
 * ------------------------------------------------------------------------- */
typedef struct {
    char nome[MAX_NOME + 1];   /* ate 20 caracteres + '\0'                  */
    char marca;                /* 'X' ou 'O'                                */
    int  vitorias;             /* placar acumulado com esses 2 jogadores    */
} Jogador;

/* ---------------------------------------------------------------------------
 *  FUNCOES DE ENTRADA (validacao robusta)
 * ------------------------------------------------------------------------- */

/* Descarta o restante de uma linha longa demais que ficou no stdin. */
static void limparEntrada(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* apenas consome */
    }
}

/* Le um numero inteiro dentro do intervalo [minimo..maximo], re-pedindo
 * quantas vezes for necessario. Toda leitura usa fgets + strtol . */
static int lerInteiro(const char *rotulo, int minimo, int maximo)
{
    char linha[TAM_LINHA];
    char *fim;
    long valor;

    for (;;) {
        printf("%s", rotulo);
        fflush(stdout);

        if (fgets(linha, sizeof linha, stdin) == NULL) {
            if (feof(stdin)) {              /* entrada fechou (ex.: pipe) */
                fprintf(stderr, "\nEntrada encerrada de forma inesperada.\n");
                exit(EXIT_FAILURE);
            }
            clearerr(stdin);
            printf("Falha na leitura. Tente novamente.\n");
            continue;
        }

        /* Linha maior que o buffer: descarta o resto que ficou no stdin . */
        if (strchr(linha, '\n') == NULL) {
            limparEntrada();
            printf("Entrada muito longa. Tente novamente.\n");
            continue;
        }

        valor = strtol(linha, &fim, 10);    /* converte com seguranca     */

        if (fim == linha) {                 /* nao havia nenhum digito    */
            printf("Entrada invalida: digite um numero inteiro.\n");
            continue;
        }

        /* Ignora espacos apos o numero e exige que nada mais reste . */
        while (isspace((unsigned char)*fim) && *fim != '\n') {
            fim++;
        }
        if (*fim != '\n' && *fim != '\0') {
            printf("Entrada invalida: digite apenas o numero.\n");
            continue;
        }

        if (valor < (long)minimo || valor > (long)maximo) {
            printf("Valor fora do intervalo [%d, %d]. Tente novamente.\n",
                   minimo, maximo);
            continue;
        }
        return (int)valor;
    }
}

/* Le o nome de um jogador (ate MAX_NOME caracteres), validando tudo . */
static void lerNome(const char *rotulo, Jogador *jogador)
{
    char linha[TAM_LINHA];
    char *inicio, *fim;
    size_t comprimento;

    for (;;) {
        printf("%s", rotulo);
        fflush(stdout);

        if (fgets(linha, sizeof linha, stdin) == NULL) {
            if (feof(stdin)) {
                fprintf(stderr, "\nEntrada encerrada de forma inesperada.\n");
                exit(EXIT_FAILURE);
            }
            clearerr(stdin);
            printf("Falha na leitura. Tente novamente.\n");
            continue;
        }

        /* Linha maior que o buffer . */
        if (strchr(linha, '\n') == NULL) {
            limparEntrada();
            printf("Nome invalido: use no maximo %d caracteres.\n", MAX_NOME);
            continue;
        }
        linha[strcspn(linha, "\n")] = '\0';   /* remove a quebra de linha   */

        /* Remove espacos do inicio e do fim. */
        inicio = linha;
        while (isspace((unsigned char)*inicio)) {
            inicio++;
        }
        fim = linha + strlen(linha);
        while (fim > inicio && isspace((unsigned char)fim[-1])) {
            fim--;
        }
        *fim = '\0';

        comprimento = strlen(inicio);

        if (comprimento == 0) {
            printf("O nome nao pode ficar vazio. Tente novamente.\n");
            continue;
        }
        if (comprimento > MAX_NOME) {
            printf("Nome invalido: (%s) tem %d caracteres; use no maximo %d.\n",
                   inicio, (int)comprimento, MAX_NOME);
            continue;
        }
        if (strchr(inicio, '|') != NULL) {
            /* '|' e separador do arquivo de placar; nao pode vir do usuario. */
            printf("Nome invalido: o caractere '|' nao e permitido.\n");
            continue;
        }

        strcpy(jogador->nome, inicio);      /* cabe, pois foi validado .  */
        return;
    }
}

/* ---------------------------------------------------------------------------
 *  FUNCOES DO TABULEIRO (alocacao dinamica: matriz = ponteiro de ponteiro)
 * ------------------------------------------------------------------------- */

/* Aloca uma matriz tamanho x tamanho de chars e inicializa com ' '.
 * Retorna char ** (ponteiro para as linhas da matriz). */
static char **alocarTabuleiro(int tamanho)
{
    char **tabuleiro = (char **)malloc((size_t)tamanho * sizeof(char *));

    if (tabuleiro == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente para o tabuleiro.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < tamanho; i++) {
        tabuleiro[i] = (char *)malloc((size_t)tamanho * sizeof(char));
        if (tabuleiro[i] == NULL) {
            /* libera o que ja foi alocado antes de encerrar */
            for (int k = 0; k < i; k++) {
                free(tabuleiro[k]);
            }
            free(tabuleiro);
            fprintf(stderr, "Erro: memoria insuficiente para o tabuleiro.\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < tamanho; j++) {
            tabuleiro[i][j] = ' ';          /* casa vazia                 */
        }
    }
    return tabuleiro;
}

/* Libera toda a matriz dinamicamente. Recebe char *** (ponteiro para o
 * ponteiro do tabuleiro) para poder zerar o ponteiro do chamador tambem . */
static void liberarTabuleiro(char ***tabuleiro, int tamanho)
{
    if (tabuleiro == NULL || *tabuleiro == NULL) {
        return;
    }
    for (int i = 0; i < tamanho; i++) {
        free((*tabuleiro)[i]);
    }
    free(*tabuleiro);
    *tabuleiro = NULL;                       /* evita ponteiro pendente    */
}

/* Mostra o tabuleiro formatado, com indices de linha e coluna (1..n). */
static void imprimirTabuleiro(char **tabuleiro, int tamanho)
{
    printf("\n    ");
    for (int j = 1; j <= tamanho; j++) {
        printf(" %d   ", j);
    }
    printf("\n");
    for (int i = 0; i < tamanho; i++) {
        printf("   +");
        for (int j = 0; j < tamanho; j++) {
            printf("---+");
        }
        printf("\n %d |", i + 1);
        for (int j = 0; j < tamanho; j++) {
            printf(" %c |", tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("   +");
    for (int j = 0; j < tamanho; j++) {
        printf("---+");
    }
    printf("\n");
}

/* Verifica se a casa (linha, coluna) informada (base 1) existe e esta vazia.
 * Recebe o tabuleiro por ponteiro de ponteiro, sem copiar nada. */
static int casaDisponivel(char **tabuleiro, int tamanho, int linha, int coluna)
{
    int i = linha - 1, j = coluna - 1;
    if (i < 0 || i >= tamanho || j < 0 || j >= tamanho) {
        return 0;                            /* fora do tabuleiro          */
    }
    return tabuleiro[i][j] == ' ';           /* vazia?                     */
}

/* Grava a marca do jogador na casa indicada (base 1) . */
static void registrarJogada(char **tabuleiro, int linha, int coluna, char marca)
{
    tabuleiro[linha - 1][coluna - 1] = marca;
}

/* Retorna 1 se a marca venceu (linha, coluna ou diagonal completa). */
static int verificarVitoria(char **tabuleiro, int tamanho, char marca)
{
    int i, j;

    /* linhas completas */
    for (i = 0; i < tamanho; i++) {
        for (j = 0; j < tamanho && tabuleiro[i][j] == marca; j++) {
            /* percorre */
        }
        if (j == tamanho) {
            return 1;
        }
    }

    /* colunas completas */
    for (j = 0; j < tamanho; j++) {
        for (i = 0; i < tamanho && tabuleiro[i][j] == marca; i++) {
            /* percorre */
        }
        if (i == tamanho) {
            return 1;
        }
    }

    /* diagonal principal */
    for (i = 0; i < tamanho && tabuleiro[i][i] == marca; i++) {
        /* percorre */
    }
    if (i == tamanho) {
        return 1;
    }

    /* diagonal secundaria */
    for (i = 0; i < tamanho && tabuleiro[i][tamanho - 1 - i] == marca; i++) {
        /* percorre */
    }
    if (i == tamanho) {
        return 1;
    }

    return 0;
}

/* Pergunta o tamanho do tabuleiro (3x3 ou 4x4) e retorna o tamanho. */
static int escolherTamanhoTabuleiro(void)
{
    int opcao = lerInteiro(
        "\nEscolha o tabuleiro:\n"
        "  1) 3x3\n"
        "  2) 4x4\n"
        "Opcao: ", 1, 2);

    return opcao + 2;   /* 1 -> 3, 2 -> 4 */
}

/* ---------------------------------------------------------------------------
 *  PLACAR EM ARQUIVO
 * ------------------------------------------------------------------------- */

/* Monta o nome do arquivo: "<jogador1>_X_<jogador2>.txt".
 * Escreve no buffer 'destino' passado por ponteiro pela funcao chamadora. */
static void construirCaminhoDoPlacar(const Jogador *j1, const Jogador *j2,
                                     char *destino, size_t capacidade)
{
    snprintf(destino, capacidade, "%s_X_%s.txt", j1->nome, j2->nome);
}

/* Extrai o numero de vitorias de uma linha no formato "... | Vitórias: n". */
static int extrairVitorias(const char *linha)
{
    const char *marcador = strstr(linha, ROTULO_VITORIAS);
    if (marcador == NULL) {
        return 0;
    }
    return atoi(marcador + strlen(ROTULO_VITORIAS));
}

/* Mostra o placar no console no mesmo formato de tabela do arquivo . */
static void imprimirPlacar(const Jogador *j1, const Jogador *j2)
{
    printf("\n========== PLACAR ==========\n");
    printf("%s | %s %d\n", j1->nome, ROTULO_VITORIAS, j1->vitorias);
    printf("%s | %s %d\n", j2->nome, ROTULO_VITORIAS, j2->vitorias);
    printf("=============================\n");
}

/* Salva o placar dos dois jogadores em "<nome1>_X_<nome2>.txt" . */
static void salvarPlacar(const Jogador *j1, const Jogador *j2)
{
    char caminho[MAX_NOME * 2 + 16];
    FILE *arquivo;

    construirCaminhoDoPlacar(j1, j2, caminho, sizeof caminho);

    arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        printf("\nAviso: nao foi possivel salvar o placar em \"%s\".\n", caminho);
        return;
    }

    fprintf(arquivo, "%s | %s %d\n", j1->nome, ROTULO_VITORIAS, j1->vitorias);
    fprintf(arquivo, "%s | %s %d\n", j2->nome, ROTULO_VITORIAS, j2->vitorias);
    fclose(arquivo);

    printf("\nPlacar salvo em \"%s\".\n", caminho);
}

/* Carrega o placar anterior (se existir) para que as vitorias acumulem
 * entre execucoes com os mesmos dois jogadores . */
static void carregarPlacar(const char *caminho, Jogador *j1, Jogador *j2)
{
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        return;                              /* primeira partida dos dois  */
    }

    char linha[TAM_LINHA];
    if (fgets(linha, sizeof linha, arquivo) != NULL) {
        char *separador = strchr(linha, '|');
        if (separador != NULL) {
            j1->vitorias = extrairVitorias(separador);
        }
    }
    if (fgets(linha, sizeof linha, arquivo) != NULL) {
        char *separador = strchr(linha, '|');
        if (separador != NULL) {
            j2->vitorias = extrairVitorias(separador);
        }
    }
    fclose(arquivo);

    printf("\nPlacar anterior carregado de \"%s\".\n", caminho);
}

/* ---------------------------------------------------------------------------
 *  PARTIDA
 * ------------------------------------------------------------------------- */

/* Controla uma partida completa: aloca o tabuleiro, alterna os jogadores,
 * valida cada jogada e decide vitoria/empate. J1 e J2 vem por ponteiro. */
static void jogarPartida(Jogador *jogador1, Jogador *jogador2)
{
    int tamanho = escolherTamanhoTabuleiro();
    char **tabuleiro = alocarTabuleiro(tamanho);
    Jogador *daVez = jogador1;               /* jogador 1 comeca com 'X'   */
    int casasTotais = tamanho * tamanho;
    int jogadasFeitas = 0;

    printf("\nNova partida: %s (%c) x %s (%c) em %dx%d\n",
           jogador1->nome, jogador1->marca,
           jogador2->nome, jogador2->marca,
           tamanho, tamanho);
    printf("Regra: vence quem completar uma linha, coluna ou diagonal inteira.\n");

    for (;;) {
        imprimirTabuleiro(tabuleiro, tamanho);
        printf("Vez de %s (%c).\n", daVez->nome, daVez->marca);

        int linha  = lerInteiro("  Linha : ", 1, tamanho);
        int coluna = lerInteiro("  Coluna: ", 1, tamanho);

        if (!casaDisponivel(tabuleiro, tamanho, linha, coluna)) {
            printf("Casa invalida ou ja ocupada. Jogue novamente.\n");
            continue;                        /* nao gasta a vez .        */
        }

        registrarJogada(tabuleiro, linha, coluna, daVez->marca);
        jogadasFeitas++;
        imprimirTabuleiro(tabuleiro, tamanho);

        if (verificarVitoria(tabuleiro, tamanho, daVez->marca)) {
            daVez->vitorias++;
            printf(">>> Parabens, %s! Vitoria com '%c'.\n",
                   daVez->nome, daVez->marca);
            salvarPlacar(jogador1, jogador2);      /* grava apos a partida */
            break;
        }

        if (jogadasFeitas == casasTotais) {
            printf(">>> Empate (velha)! Ninguem pontua.\n");
            break;
        }

        /* troca a vez: jogador 1 <-> jogador 2 */
        daVez = (daVez == jogador1) ? jogador2 : jogador1;
    }

    liberarTabuleiro(&tabuleiro, tamanho);   /* memoria liberada .       */
}

/* ---------------------------------------------------------------------------
 *  PROGRAMA PRINCIPAL
 * ------------------------------------------------------------------------- */
int main(void)
{
#ifdef _WIN32
    /* O codigo-fonte esta em UTF-8; ajusta o console e a CRT para que os
     * acentos ("Vitórias") aparecam corretamente no terminal do Windows . */
    setlocale(LC_ALL, ".UTF8");
    system("chcp 65001 > nul");
#else
    setlocale(LC_ALL, "");
#endif

    Jogador jogador1 = { "", 'X', 0 };       /* jogador 1 sempre usa 'X'   */
    Jogador jogador2 = { "", 'O', 0 };       /* jogador 2 sempre usa 'O'   */
    char caminhoPlacar[MAX_NOME * 2 + 16];

    printf("=================================\n");
    printf("        JOGO DA VELHA N x N\n");
    printf("   3x3 | 4x4  -  X  vs  O\n");
    printf("=================================\n");

    lerNome("Nome do jogador 1 (franco 'X'): ", &jogador1);

    /* os dois nomes precisam ser diferentes para o placar fazer sentido . */
    lerNome("Nome do jogador 2 (franco 'O'): ", &jogador2);
    while (strcmp(jogador1.nome, jogador2.nome) == 0) {
        printf("Jogador 2 nao pode ter o mesmo nome do jogador 1.\n");
        lerNome("Nome do jogador 2 (franco 'O'): ", &jogador2);
    }

    construirCaminhoDoPlacar(&jogador1, &jogador2, caminhoPlacar,
                             sizeof caminhoPlacar);
    carregarPlacar(caminhoPlacar, &jogador1, &jogador2);
    imprimirPlacar(&jogador1, &jogador2);

    int opcao;
    do {
        jogarPartida(&jogador1, &jogador2);
        opcao = lerInteiro("\nDeseja jogar novamente? (1 = sim, 2 = nao): ",
                           1, 2);
    } while (opcao == 1);

    printf("\nFim de jogo. Placar final:\n");
    imprimirPlacar(&jogador1, &jogador2);
    salvarPlacar(&jogador1, &jogador2);      /* garante o arquivo em dia . */
    printf("Obrigado por jogar!\n");

    return 0;
}
