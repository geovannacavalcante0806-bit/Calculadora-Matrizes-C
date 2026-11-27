#ifndef ENTRADA_H
#define ENTRADA_H

#include "matriz.h"

#define MIN_MATRIZES 2
#define MAX_MATRIZES 26

typedef enum {
    ENTRADA_OK = 0,
    ENTRADA_EOF,
    ENTRADA_ERRO_MEMORIA
} StatusEntrada;

/* Le um inteiro em [minimo, maximo], repetindo ate ser valido. */
StatusEntrada ler_inteiro(const char *mensagem, int minimo, int maximo, int *valor);

/* Le um numero real finito, repetindo ate ser valido. Aceita virgula decimal. */
StatusEntrada ler_real(const char *mensagem, double *valor);

/* Le dimensoes e elementos de uma matriz (indice 0 = A, 1 = B, ...). */
StatusEntrada ler_matriz(int indice, Matriz **saida);

/* Pergunta a quantidade (>= 2) e cadastra todas as matrizes. */
StatusEntrada cadastrar_matrizes(Matriz ***matrizes, int *quantidade);

#endif /* ENTRADA_H */
