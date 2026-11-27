# Calculadora de Matrizes em C

## Sobre o projeto

Este projeto consiste no desenvolvimento de uma calculadora de matrizes utilizando a linguagem C. O programa permite cadastrar diferentes matrizes, informar suas dimensões e seus elementos, além de realizar operações matemáticas entre elas.

O objetivo é aplicar conceitos de programação estruturada, modularização, alocação dinâmica de memória, manipulação de matrizes e validação de dados.

## Funcionalidades

O programa oferece as seguintes operações:

- **Adição:** soma duas ou mais matrizes de mesmas dimensões.
- **Subtração:** subtrai matrizes que possuem as mesmas dimensões.
- **Multiplicação:** realiza a multiplicação entre matrizes, respeitando as regras matemáticas.
- **Divisão matricial:** realiza a multiplicação da primeira matriz pela inversa da segunda.
- **Cadastro de matrizes:** permite cadastrar de 2 a 26 matrizes.
- **Validação de dados:** verifica dimensões incompatíveis, entradas inválidas e matrizes que não possuem inversa.
- **Novas operações:** permite realizar outras operações utilizando as mesmas matrizes ou cadastrar novas.

## Organização dos arquivos

O projeto foi dividido em arquivos para facilitar a organização e a manutenção do código.

| Arquivo | Descrição |
|---|---|
| `main.c` | Controla a execução do programa, apresenta o menu e exibe os resultados. |
| `matriz.h` | Define a estrutura da matriz e declara suas funções. |
| `matriz.c` | Implementa a criação, cópia, exibição e liberação das matrizes. |
| `entrada.h` | Declara as funções responsáveis pela entrada de dados. |
| `entrada.c` | Realiza a leitura e validação dos valores informados pelo usuário. |
| `operacoes.h` | Define os tipos de operações, códigos de erro e suas funções. |
| `operacoes.c` | Implementa os cálculos de adição, subtração, multiplicação, inversa e divisão. |

## Tecnologias e conceitos utilizados

- Linguagem C.
- Biblioteca `stdio.h` para entrada e saída de dados.
- Biblioteca `stdlib.h` para alocação dinâmica de memória.
- Biblioteca `math.h` para operações matemáticas.
- Biblioteca `string.h` para manipulação de dados.
- Biblioteca `ctype.h` para validação de caracteres.
- Ponteiros e estruturas (`struct`).
- Alocação dinâmica de memória com `calloc`.
- Modularização por meio de arquivos `.c` e `.h`.
- Estruturas de repetição e decisão.
- Validação e tratamento de erros.

## Como executar

É necessário ter um compilador C instalado, como o GCC.

1. Abra a pasta do projeto no VS Code.
2. Abra o terminal.
3. Compile os arquivos utilizando o comando:

```bash
gcc main.c matriz.c entrada.c operacoes.c -o calculadora -lm
```

4. Execute o programa:

**Windows:**
```bash
calculadora.exe
```

**Linux:**
```bash
./calculadora
```

## Funcionamento

Ao iniciar o programa, o usuário informa a quantidade de matrizes que deseja cadastrar. Em seguida, define o número de linhas e colunas e preenche os elementos de cada matriz.

Depois do cadastro, o sistema apresenta um menu com as operações disponíveis. O usuário escolhe a operação desejada e o programa realiza os cálculos, exibindo o resultado ou informando o motivo caso não seja possível executar a operação.

Na divisão matricial, o programa utiliza o método de Gauss-Jordan com pivoteamento parcial para calcular a matriz inversa.

## Considerações finais

O desenvolvimento deste projeto permitiu trabalhar na prática com operações matriciais e conceitos importantes da linguagem C. A divisão do código em diferentes módulos também contribuiu para uma melhor organização, facilitando a compreensão e a manutenção do programa.# Calculadora-Matrizes-C
