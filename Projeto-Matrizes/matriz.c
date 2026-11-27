#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matriz.h"

Matriz *matriz_criar(int linhas, int colunas)
{
    Matriz *m;

    if (linhas < 1 || colunas < 1 || linhas > MAX_DIM || colunas > MAX_DIM) {
        return NULL;
    }
    m = (Matriz *)calloc(1, sizeof(Matriz));
    if (m == NULL) {
        return NULL;
    }
    m->dados = (double *)calloc((size_t)linhas * (size_t)colunas, sizeof(double));
    if (m->dados == NULL) {
        free(m);
        return NULL;
    }
    m->linhas = linhas;
    m->colunas = colunas;
    return m;
}

void matriz_liberar(Matriz *m)
{
    if (m == NULL) {
        return;
    }
    free(m->dados);
    free(m);
}

Matriz *matriz_copiar(const Matriz *origem)
{
    Matriz *copia;

    if (origem == NULL || origem->dados == NULL) {
        return NULL;
    }
    copia = matriz_criar(origem->linhas, origem->colunas);
    if (copia == NULL) {
        return NULL;
    }
    memcpy(copia->dados, origem->dados,
           (size_t)origem->linhas * (size_t)origem->colunas * sizeof(double));
    return copia;
}

void liberar_matrizes(Matriz **lista, int quantidade)
{
    int i;

    if (lista == NULL) {
        return;
    }
    for (i = 0; i < quantidade; i++) {
        matriz_liberar(lista[i]);
    }
    free(lista);
}

/* Evita imprimir "-0.0000" para valores minusculos. */
static double normalizar(double v)
{
    if (fabs(v) < 0.5e-4) {
        return 0.0;
    }
    return v;
}

void matriz_exibir(const Matriz *m)
{
    int largura = 1;
    int i, j;
    char buf[64];

    if (m == NULL || m->dados == NULL) {
        return;
    }

    /* largura comum a todas as celulas para alinhar as colunas */
    for (i = 0; i < m->linhas; i++) {
        for (j = 0; j < m->colunas; j++) {
            int n = snprintf(buf, sizeof buf, "%.4f", normalizar(ELEM(m, i, j)));
            if (n > largura) {
                largura = n;
            }
        }
    }

    for (i = 0; i < m->linhas; i++) {
        printf("| ");
        for (j = 0; j < m->colunas; j++) {
            printf("%*.4f ", largura, normalizar(ELEM(m, i, j)));
        }
        printf("|\n");
    }
}
