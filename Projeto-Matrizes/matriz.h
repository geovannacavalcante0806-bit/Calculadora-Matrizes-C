#ifndef MATRIZ_H
#define MATRIZ_H

#include <stddef.h>

#define MAX_DIM 100

typedef struct {
    int linhas;
    int colunas;
    double *dados; /* armazenamento linear por linhas (calloc) */
} Matriz;

/* Acesso ao elemento (i, j), com indices a partir de 0. */
#define ELEM(m, i, j) ((m)->dados[(size_t)(i) * (size_t)(m)->colunas + (size_t)(j)])

/* Cria matriz zerada; retorna NULL se as dimensoes forem invalidas ou faltar memoria. */
Matriz *matriz_criar(int linhas, int colunas);

/* Libera a matriz (aceita NULL). */
void matriz_liberar(Matriz *m);

/* Retorna uma copia independente; NULL em caso de erro. */
Matriz *matriz_copiar(const Matriz *origem);

/* Libera um vetor de matrizes e o proprio vetor (aceita NULL). */
void liberar_matrizes(Matriz **lista, int quantidade);

/* Imprime a matriz com colunas alinhadas. */
void matriz_exibir(const Matriz *m);

#endif /* MATRIZ_H */
