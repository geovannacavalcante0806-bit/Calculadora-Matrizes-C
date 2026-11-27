#include <stdlib.h>
#include <math.h>
#include "operacoes.h"

#define LIMITE_PIVO_REL 1e-12

/* ---------- validacoes ---------- */

StatusOp validar_soma(const Matriz *a, const Matriz *b)
{
    if (a == NULL || b == NULL) {
        return OP_ERRO_ARGUMENTO;
    }
    if (a->linhas != b->linhas || a->colunas != b->colunas) {
        return OP_DIM_SOMA_INCOMPATIVEL;
    }
    return OP_OK;
}

StatusOp validar_multiplicacao(const Matriz *a, const Matriz *b)
{
    if (a == NULL || b == NULL) {
        return OP_ERRO_ARGUMENTO;
    }
    if (a->colunas != b->linhas) {
        return OP_DIM_MULT_INCOMPATIVEL;
    }
    return OP_OK;
}

StatusOp validar_divisao(const Matriz *a, const Matriz *b)
{
    if (a == NULL || b == NULL) {
        return OP_ERRO_ARGUMENTO;
    }
    if (b->linhas != b->colunas) {
        return OP_DIVISOR_NAO_QUADRADA;
    }
    if (a->colunas != b->linhas) {
        return OP_DIVISAO_DIM_INCOMPATIVEL;
    }
    return OP_OK;
}

/* ---------- soma, subtracao, multiplicacao ---------- */

static StatusOp somar_ou_subtrair(const Matriz *a, const Matriz *b, double sinal, Matriz **resultado)
{
    StatusOp st;
    Matriz *r;
    int i, j;

    if (resultado == NULL) {
        return OP_ERRO_ARGUMENTO;
    }
    *resultado = NULL;

    st = validar_soma(a, b);
    if (st != OP_OK) {
        return st;
    }
    r = matriz_criar(a->linhas, a->colunas);
    if (r == NULL) {
        return OP_ERRO_MEMORIA;
    }
    for (i = 0; i < a->linhas; i++) {
        for (j = 0; j < a->colunas; j++) {
            ELEM(r, i, j) = ELEM(a, i, j) + sinal * ELEM(b, i, j);
        }
    }
    *resultado = r;
    return OP_OK;
}

StatusOp matriz_somar(const Matriz *a, const Matriz *b, Matriz **resultado)
{
    return somar_ou_subtrair(a, b, 1.0, resultado);
}

StatusOp matriz_subtrair(const Matriz *a, const Matriz *b, Matriz **resultado)
{
    return somar_ou_subtrair(a, b, -1.0, resultado);
}

StatusOp matriz_multiplicar(const Matriz *a, const Matriz *b, Matriz **resultado)
{
    StatusOp st;
    Matriz *r;
    int i, j, k;

    if (resultado == NULL) {
        return OP_ERRO_ARGUMENTO;
    }
    *resultado = NULL;

    st = validar_multiplicacao(a, b);
    if (st != OP_OK) {
        return st;
    }
    r = matriz_criar(a->linhas, b->colunas);
    if (r == NULL) {
        return OP_ERRO_MEMORIA;
    }
    for (i = 0; i < a->linhas; i++) {
        for (j = 0; j < b->colunas; j++) {
            double soma = 0.0;
            for (k = 0; k < a->colunas; k++) {
                soma += ELEM(a, i, k) * ELEM(b, k, j);
            }
            ELEM(r, i, j) = soma;
        }
    }
    *resultado = r;
    return OP_OK;
}

/* ---------- inversa (Gauss-Jordan com pivoteamento parcial) ---------- */

StatusOp matriz_inversa(const Matriz *a, Matriz **resultado)
{
    StatusOp status = OP_OK;
    Matriz *w = NULL;   /* copia de trabalho de A */
    Matriz *inv = NULL; /* parte da identidade que vira a inversa */
    double maior = 0.0;
    double limite;
    int n, i, j, k;

    if (resultado == NULL) {
        return OP_ERRO_ARGUMENTO;
    }
    *resultado = NULL;

    if (a == NULL || a->dados == NULL) {
        return OP_ERRO_ARGUMENTO;
    }
    if (a->linhas != a->colunas) {
        return OP_DIVISOR_NAO_QUADRADA;
    }
    n = a->linhas;

    w = matriz_copiar(a);
    inv = matriz_criar(n, n);
    if (w == NULL || inv == NULL) {
        matriz_liberar(w);
        matriz_liberar(inv);
        return OP_ERRO_MEMORIA;
    }
    for (i = 0; i < n; i++) {
        ELEM(inv, i, i) = 1.0;
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            double v = fabs(ELEM(a, i, j));
            if (v > maior) {
                maior = v;
            }
        }
    }
    limite = LIMITE_PIVO_REL * maior;

    for (k = 0; k < n && status == OP_OK; k++) {
        int p = k;
        double pivo;

        /* pivoteamento parcial: maior |valor| da coluna k a partir da linha k */
        for (i = k + 1; i < n; i++) {
            if (fabs(ELEM(w, i, k)) > fabs(ELEM(w, p, k))) {
                p = i;
            }
        }
        pivo = ELEM(w, p, k);
        if (pivo == 0.0 || fabs(pivo) < limite) {
            status = OP_DIVISOR_SINGULAR;
            break;
        }

        if (p != k) {
            for (j = 0; j < n; j++) {
                double t = ELEM(w, k, j);
                ELEM(w, k, j) = ELEM(w, p, j);
                ELEM(w, p, j) = t;
                t = ELEM(inv, k, j);
                ELEM(inv, k, j) = ELEM(inv, p, j);
                ELEM(inv, p, j) = t;
            }
        }

        for (j = 0; j < n; j++) {
            ELEM(w, k, j) /= pivo;
            ELEM(inv, k, j) /= pivo;
        }

        for (i = 0; i < n; i++) {
            double f;
            if (i == k) {
                continue;
            }
            f = ELEM(w, i, k);
            if (f != 0.0) {
                for (j = 0; j < n; j++) {
                    ELEM(w, i, j) -= f * ELEM(w, k, j);
                    ELEM(inv, i, j) -= f * ELEM(inv, k, j);
                }
            }
        }
    }

    matriz_liberar(w);
    if (status != OP_OK) {
        matriz_liberar(inv);
        return status;
    }
    *resultado = inv;
    return OP_OK;
}

/* ---------- divisao: A / B = A x inversa(B) ---------- */

StatusOp matriz_dividir(const Matriz *a, const Matriz *b, Matriz **resultado)
{
    StatusOp st;
    Matriz *inv = NULL;

    if (resultado == NULL) {
        return OP_ERRO_ARGUMENTO;
    }
    *resultado = NULL;

    st = validar_divisao(a, b);
    if (st != OP_OK) {
        return st;
    }
    st = matriz_inversa(b, &inv);
    if (st != OP_OK) {
        return st;
    }
    st = matriz_multiplicar(a, inv, resultado);
    matriz_liberar(inv);
    return st;
}

/* ---------- execucao em sequencia ---------- */

static StatusOp aplicar(TipoOperacao op, const Matriz *a, const Matriz *b, Matriz **res)
{
    switch (op) {
    case OPER_SOMA:          return matriz_somar(a, b, res);
    case OPER_SUBTRACAO:     return matriz_subtrair(a, b, res);
    case OPER_MULTIPLICACAO: return matriz_multiplicar(a, b, res);
    case OPER_DIVISAO:       return matriz_dividir(a, b, res);
    }
    *res = NULL;
    return OP_ERRO_ARGUMENTO;
}

StatusOp executar_sequencia(Matriz **mats, int n, TipoOperacao op,
                            Matriz **resultado, InfoErro *info)
{
    Matriz *acc;
    int i;

    if (info != NULL) {
        info->etapa = 0;
        info->linhas_esq = info->colunas_esq = 0;
        info->linhas_dir = info->colunas_dir = 0;
    }
    if (resultado == NULL) {
        return OP_ERRO_ARGUMENTO;
    }
    *resultado = NULL;

    if (mats == NULL || n < 2 || info == NULL) {
        return OP_ERRO_ARGUMENTO;
    }
    for (i = 0; i < n; i++) {
        if (mats[i] == NULL) {
            return OP_ERRO_ARGUMENTO;
        }
    }

    acc = matriz_copiar(mats[0]);
    if (acc == NULL) {
        return OP_ERRO_MEMORIA;
    }

    for (i = 1; i < n; i++) {
        Matriz *novo = NULL;
        StatusOp st = aplicar(op, acc, mats[i], &novo);

        if (st != OP_OK) {
            info->etapa = i;
            info->linhas_esq = acc->linhas;
            info->colunas_esq = acc->colunas;
            info->linhas_dir = mats[i]->linhas;
            info->colunas_dir = mats[i]->colunas;
            matriz_liberar(acc);
            return st;
        }
        matriz_liberar(acc);
        acc = novo;
    }

    *resultado = acc;
    return OP_OK;
}
