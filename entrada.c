#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include "entrada.h"

#define TAM_LINHA 128

/* Le uma linha inteira. Descarta o excesso se a linha for maior que o buffer. */
static StatusEntrada ler_linha(char *buf, size_t tam, int *muito_longa)
{
    size_t len;

    *muito_longa = 0;
    if (fgets(buf, (int)tam, stdin) == NULL) {
        return ENTRADA_EOF;
    }
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else if (!feof(stdin)) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            /* descarta o restante da linha */
        }
        *muito_longa = 1;
    }
    return ENTRADA_OK;
}

StatusEntrada ler_inteiro(const char *mensagem, int minimo, int maximo, int *valor)
{
    char buf[TAM_LINHA];

    for (;;) {
        StatusEntrada st;
        int longa;
        char *fim;
        long v;

        printf("%s", mensagem);
        fflush(stdout);

        st = ler_linha(buf, sizeof buf, &longa);
        if (st != ENTRADA_OK) {
            return st;
        }
        if (longa) {
            printf("Entrada muito longa. Digite um número inteiro entre %d e %d.\n", minimo, maximo);
            continue;
        }

        errno = 0;
        v = strtol(buf, &fim, 10);
        if (fim == buf) {
            printf("Valor inválido. Digite um número inteiro entre %d e %d.\n", minimo, maximo);
            continue;
        }
        while (isspace((unsigned char)*fim)) {
            fim++;
        }
        if (*fim != '\0' || errno == ERANGE || v < minimo || v > maximo) {
            printf("Valor inválido. Digite um número inteiro entre %d e %d.\n", minimo, maximo);
            continue;
        }

        *valor = (int)v;
        return ENTRADA_OK;
    }
}

StatusEntrada ler_real(const char *mensagem, double *valor)
{
    char buf[TAM_LINHA];

    for (;;) {
        StatusEntrada st;
        int longa;
        char *fim;
        char *p;
        double v;

        printf("%s", mensagem);
        fflush(stdout);

        st = ler_linha(buf, sizeof buf, &longa);
        if (st != ENTRADA_OK) {
            return st;
        }
        if (longa) {
            printf("Entrada muito longa. Digite um número real.\n");
            continue;
        }

        /* aceita virgula decimal (ex.: 2,5) */
        for (p = buf; *p != '\0'; p++) {
            if (*p == ',') {
                *p = '.';
            }
        }

        errno = 0;
        v = strtod(buf, &fim);
        if (fim == buf) {
            printf("Valor inválido. Digite um número real (ex.: 3, -2.5).\n");
            continue;
        }
        while (isspace((unsigned char)*fim)) {
            fim++;
        }
        if (*fim != '\0' || errno == ERANGE || !isfinite(v)) {
            printf("Valor inválido. Digite um número real (ex.: 3, -2.5).\n");
            continue;
        }

        *valor = v;
        return ENTRADA_OK;
    }
}

StatusEntrada ler_matriz(int indice, Matriz **saida)
{
    char prompt[96];
    char nome = (char)('A' + indice);
    int linhas = 0;
    int colunas = 0;
    int i, j;
    StatusEntrada st;
    Matriz *m;

    *saida = NULL;
    printf("\n--- Cadastro da matriz %c ---\n", nome);

    snprintf(prompt, sizeof prompt, "Número de linhas de %c (1 a %d): ", nome, MAX_DIM);
    st = ler_inteiro(prompt, 1, MAX_DIM, &linhas);
    if (st != ENTRADA_OK) {
        return st;
    }

    snprintf(prompt, sizeof prompt, "Número de colunas de %c (1 a %d): ", nome, MAX_DIM);
    st = ler_inteiro(prompt, 1, MAX_DIM, &colunas);
    if (st != ENTRADA_OK) {
        return st;
    }

    m = matriz_criar(linhas, colunas);
    if (m == NULL) {
        return ENTRADA_ERRO_MEMORIA;
    }

    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            double v = 0.0;
            snprintf(prompt, sizeof prompt, "Elemento %c[%d][%d]: ", nome, i + 1, j + 1);
            st = ler_real(prompt, &v);
            if (st != ENTRADA_OK) {
                matriz_liberar(m);
                return st;
            }
            ELEM(m, i, j) = v;
        }
    }

    *saida = m;
    return ENTRADA_OK;
}

StatusEntrada cadastrar_matrizes(Matriz ***matrizes, int *quantidade)
{
    char prompt[96];
    int n = 0;
    int i;
    StatusEntrada st;
    Matriz **lista;

    *matrizes = NULL;
    *quantidade = 0;

    snprintf(prompt, sizeof prompt,
             "Quantas matrizes deseja usar? (mínimo %d, máximo %d): ", MIN_MATRIZES, MAX_MATRIZES);
    st = ler_inteiro(prompt, MIN_MATRIZES, MAX_MATRIZES, &n);
    if (st != ENTRADA_OK) {
        return st;
    }

    lista = (Matriz **)calloc((size_t)n, sizeof(Matriz *));
    if (lista == NULL) {
        return ENTRADA_ERRO_MEMORIA;
    }

    for (i = 0; i < n; i++) {
        st = ler_matriz(i, &lista[i]);
        if (st != ENTRADA_OK) {
            liberar_matrizes(lista, n); /* libera as ja criadas e o vetor */
            return st;
        }
    }

    *matrizes = lista;
    *quantidade = n;
    return ENTRADA_OK;
}