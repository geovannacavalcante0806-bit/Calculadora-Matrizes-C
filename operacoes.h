#ifndef OPERACOES_H
#define OPERACOES_H

#include "matriz.h"

typedef enum {
    OP_OK = 0,
    OP_ERRO_MEMORIA,
    OP_ERRO_ARGUMENTO,
    OP_DIM_SOMA_INCOMPATIVEL,
    OP_DIM_MULT_INCOMPATIVEL,
    OP_DIVISOR_NAO_QUADRADA,
    OP_DIVISAO_DIM_INCOMPATIVEL,
    OP_DIVISOR_SINGULAR
} StatusOp;

typedef enum {
    OPER_SOMA = 1,
    OPER_SUBTRACAO = 2,
    OPER_MULTIPLICACAO = 3,
    OPER_DIVISAO = 4
} TipoOperacao;

/* Informacoes sobre a etapa em que a sequencia falhou (preenchidas em caso de erro). */
typedef struct {
    int etapa;        /* indice da matriz da direita (1 = B, 2 = C, ...) */
    int linhas_esq;   /* dimensoes do resultado parcial (lado esquerdo) */
    int colunas_esq;
    int linhas_dir;   /* dimensoes da matriz da direita */
    int colunas_dir;
} InfoErro;

/* Validacoes (nao imprimem nada). */
StatusOp validar_soma(const Matriz *a, const Matriz *b);
StatusOp validar_multiplicacao(const Matriz *a, const Matriz *b);
StatusOp validar_divisao(const Matriz *a, const Matriz *b);

/* Operacoes: em caso de erro, *resultado = NULL e nada fica alocado. */
StatusOp matriz_somar(const Matriz *a, const Matriz *b, Matriz **resultado);
StatusOp matriz_subtrair(const Matriz *a, const Matriz *b, Matriz **resultado);
StatusOp matriz_multiplicar(const Matriz *a, const Matriz *b, Matriz **resultado);
StatusOp matriz_inversa(const Matriz *a, Matriz **resultado);
StatusOp matriz_dividir(const Matriz *a, const Matriz *b, Matriz **resultado);

/* Aplica a operacao da esquerda para a direita: ((A op B) op C) ... */
StatusOp executar_sequencia(Matriz **mats, int n, TipoOperacao op,
                            Matriz **resultado, InfoErro *info);

#endif /* OPERACOES_H */