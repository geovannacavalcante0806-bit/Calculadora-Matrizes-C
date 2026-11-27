#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"
#include "entrada.h"
#include "operacoes.h"

static char letra(int indice)
{
    return (char)('A' + indice);
}

/* Retorna 1 se a entrada foi bem-sucedida; caso contrario, imprime o motivo e retorna 0. */
static int entrada_ok(StatusEntrada status)
{
    switch (status) {
    case ENTRADA_OK:
        return 1;
    case ENTRADA_EOF:
        printf("\nEntrada encerrada. Finalizando o programa.\n");
        return 0;
    case ENTRADA_ERRO_MEMORIA:
        printf("\nErro: memória insuficiente. Finalizando o programa.\n");
        return 0;
    }
    return 0;
}

static void exibir_menu(void)
{
    printf("\n=========== MENU ===========\n");
    printf(" 1 - Adição\n");
    printf(" 2 - Subtração\n");
    printf(" 3 - Multiplicação\n");
    printf(" 4 - Divisão matricial (A x inversa(B))\n");
    printf(" 0 - Encerrar\n");
    printf("============================\n");
}

static void exibir_cadastradas(Matriz **mats, int n)
{
    int i;

    printf("\n--- Matrizes cadastradas ---\n");
    for (i = 0; i < n; i++) {
        printf("\nMatriz %c (%dx%d):\n", letra(i), mats[i]->linhas, mats[i]->colunas);
        matriz_exibir(mats[i]);
    }
}

static const char *nome_operacao(int op)
{
    switch (op) {
    case OPER_SOMA:          return "Adição";
    case OPER_SUBTRACAO:     return "Subtração";
    case OPER_MULTIPLICACAO: return "Multiplicação";
    case OPER_DIVISAO:       return "Divisão matricial";
    }
    return "Operação";
}

static void relatar_erro(StatusOp status, const InfoErro *info)
{
    char dir = letra(info->etapa);

    printf("\nNão foi possível realizar a operação.\n");
    switch (status) {
    case OP_DIM_SOMA_INCOMPATIVEL:
        printf("Motivo: adição e subtração exigem matrizes de mesmas dimensões.\n");
        printf("Na etapa com a matriz %c: resultado parcial %dx%d e matriz %c %dx%d.\n",
               dir, info->linhas_esq, info->colunas_esq,
               dir, info->linhas_dir, info->colunas_dir);
        break;
    case OP_DIM_MULT_INCOMPATIVEL:
        printf("Motivo: na multiplicação, as colunas da primeira devem ser iguais às linhas da segunda.\n");
        printf("Na etapa com a matriz %c: resultado parcial %dx%d e matriz %c %dx%d "
               "(%d colunas != %d linhas).\n",
               dir, info->linhas_esq, info->colunas_esq,
               dir, info->linhas_dir, info->colunas_dir,
               info->colunas_esq, info->linhas_dir);
        break;
    case OP_DIVISOR_NAO_QUADRADA:
        printf("Motivo: a matriz divisora %c (%dx%d) não é quadrada, logo não possui inversa.\n",
               dir, info->linhas_dir, info->colunas_dir);
        break;
    case OP_DIVISAO_DIM_INCOMPATIVEL:
        printf("Motivo: dimensões incompatíveis para a divisão.\n");
        printf("O dividendo parcial %dx%d tem %d colunas, mas o divisor %c é %dx%d "
               "(precisa ter %d linhas).\n",
               info->linhas_esq, info->colunas_esq, info->colunas_esq,
               dir, info->linhas_dir, info->colunas_dir, info->colunas_esq);
        break;
    case OP_DIVISOR_SINGULAR:
        printf("Motivo: a matriz divisora %c é singular (não possui inversa).\n", dir);
        break;
    case OP_ERRO_MEMORIA:
        printf("Motivo: memória insuficiente durante o cálculo.\n");
        break;
    case OP_ERRO_ARGUMENTO:
        printf("Motivo: argumentos inválidos (erro interno).\n");
        break;
    case OP_OK:
        break;
    }
}

int main(void)
{
    Matriz **mats = NULL;
    int n = 0;
    int ok = 1;
    int continuar = 1;

    printf("=== CALCULADORA DE MATRIZES ===\n");

    if (!entrada_ok(cadastrar_matrizes(&mats, &n))) {
        return EXIT_FAILURE;
    }

    while (continuar) {
        int escolha = 0;
        int proximo = 0;
        Matriz *resultado = NULL;
        InfoErro info;
        StatusOp status;

        exibir_cadastradas(mats, n);
        exibir_menu();

        if (!entrada_ok(ler_inteiro("Escolha uma opção: ", 0, 4, &escolha))) {
            ok = 0;
            break;
        }
        if (escolha == 0) {
            break;
        }

        status = executar_sequencia(mats, n, (TipoOperacao)escolha, &resultado, &info);
        if (status == OP_OK) {
            printf("\nResultado da %s (%dx%d):\n\n",
                   nome_operacao(escolha), resultado->linhas, resultado->colunas);
            matriz_exibir(resultado);
            matriz_liberar(resultado);
            resultado = NULL;
        } else {
            relatar_erro(status, &info);
        }

        printf("\nO que deseja fazer agora?\n");
        printf(" 1 - Nova operação com as mesmas matrizes\n");
        printf(" 2 - Cadastrar novas matrizes\n");
        printf(" 0 - Encerrar\n");
        if (!entrada_ok(ler_inteiro("Escolha: ", 0, 2, &proximo))) {
            ok = 0;
            break;
        }

        if (proximo == 0) {
            continuar = 0;
        } else if (proximo == 2) {
            liberar_matrizes(mats, n);
            mats = NULL;
            n = 0;
            if (!entrada_ok(cadastrar_matrizes(&mats, &n))) {
                ok = 0;
                break;
            }
        }
    }

    liberar_matrizes(mats, n);
    if (ok) {
        printf("\nPrograma encerrado.\n");
    }
    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}