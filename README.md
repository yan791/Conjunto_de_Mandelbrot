# Conjunto de Mandelbrot em C — Serial, Pthreads e OpenMP

Projeto acadêmico da disciplina de Infraestrutura de Software (CESAR School) que implementa a geração do Conjunto de Mandelbrot em C usando quatro estratégias de computação diferentes: **serial**, **Pthreads (particionamento estático em blocos)**, **Pthreads (fila dinâmica de trabalho)** e **OpenMP**. O programa mede o tempo de execução de cada estratégia, salva as imagens resultantes em formato PGM e registra os tempos em um arquivo de log.

## Sumário

- [Visão geral](#visão-geral)
- [Estratégias de paralelização](#estratégias-de-paralelização)
- [Estrutura do projeto](#estrutura-do-projeto)
- [Requisitos](#requisitos)
- [Compilação](#compilação)
- [Uso](#uso)
- [Saída gerada](#saída-gerada)
- [Detalhes de implementação](#detalhes-de-implementação)
- [Tratamento de erros](#tratamento-de-erros)
- [Autor](#autor)

## Visão geral

O Conjunto de Mandelbrot é calculado ponto a ponto no plano complexo (`REAL_MIN`, `REAL_MAX`, `IMAG_MIN`, `IMAG_MAX`), convertendo o número de iterações de cada ponto em um valor de intensidade (0–255) que é salvo como imagem em escala de cinza no formato PGM (`.pgm`).

O `main.c` executa, em sequência, as quatro versões sobre a mesma imagem, medindo o tempo de cada uma com `clock_gettime` (via `retorna_segundos()`), salvando a imagem resultante e registrando o tempo em `times.txt`.

## Estratégias de paralelização

| Versão | Arquivo | Estratégia |
|---|---|---|
| Serial | `src/serial.c` | Cálculo sequencial, linha por linha |
| Pthreads 1 | `src/pthreads1.c` | Divide a imagem estaticamente em blocos de linhas, um por thread |
| Pthreads 2 | `src/pthreads2.c` | Fila dinâmica de trabalho: cada thread pega a próxima linha disponível via mutex |
| OpenMP | `src/openmp.c` | `#pragma omp parallel for schedule(dynamic, 1)` sobre as linhas da imagem |

## Estrutura do projeto

```
.
├── include/
│   ├── mandelbrot.h     # Structs de configuração, protótipos das funções
│   └── login.h          # Login do aluno usado no nome dos arquivos de saída
├── src/
│   ├── common.c         # Funções compartilhadas (cálculo do ponto, PGM, log de tempo)
│   ├── serial.c         # Versão serial
│   ├── pthreads1.c      # Versão Pthreads com blocos estáticos
│   ├── pthreads2.c      # Versão Pthreads com fila dinâmica
│   ├── openmp.c         # Versão OpenMP
│   └── main.c           # Orquestra a execução das 4 versões
├── Makefile
└── evidencias.log       # Log de terminal com evidências de execução/commits
```

## Requisitos

- GCC com suporte a C11
- `pthread`
- OpenMP (`libomp` no macOS via Homebrew, ou `-fopenmp` nativo no Linux)
- `make`

## Compilação

```bash
make
```

O Makefile detecta o sistema operacional automaticamente:

- **Linux**: usa `-fopenmp` nativo do GCC.
- **macOS com Clang**: usa `libomp` via Homebrew (`brew install libomp`), com flags `-Xpreprocessor -fopenmp`.

Para limpar os artefatos de build (objetos, binário, imagens `.pgm` e `times.txt`):

```bash
make clean
```

## Uso

```bash
./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]
```

| Parâmetro | Descrição |
|---|---|
| `largura` | Largura da imagem em pixels (inteiro positivo) |
| `altura` | Altura da imagem em pixels (inteiro positivo) |
| `max_iteracoes` | Limite de iterações por ponto (inteiro positivo) |
| `num_threads` | Número de threads usado nas versões paralelas (inteiro positivo) |

### Exemplo

```bash
./mandelbrot 1920 1080 1000 8
```

Isso gera 4 imagens (uma por estratégia) com resolução 1920x1080, até 1000 iterações por ponto, usando até 8 threads nas versões paralelas.

## Saída gerada

Para cada execução, são gerados:

- `mandelbrot_<login>_serial.pgm`
- `mandelbrot_<login>_openmp.pgm`
- `mandelbrot_<login>_pthreads1.pgm`
- `mandelbrot_<login>_pthreads2.pgm`
- `times.txt` — tempo de execução (em segundos) de cada estratégia

O `<login>` é definido em `include/login.h`.

## Detalhes de implementação

- **Mapeamento de coordenadas**: `col_to_real()` e `row_to_imag()` convertem posições de pixel (coluna/linha) em coordenadas do plano complexo, respeitando os limites definidos em `mandelbrot.h`.
- **Cálculo do ponto**: `mandelbrot_ponto()` itera a relação `z = z² + c` até estourar o raio 2 ou atingir `max_iter`.
- **Conversão para intensidade**: `iterations_to_intensity()` normaliza o número de iterações para uma faixa de 0 a 255.
- **Alocação da imagem**: `alloca_imagem()` valida dimensões e verifica overflow antes de alocar o buffer.
- **Pthreads 1 (blocos estáticos)**: divide as linhas da imagem em blocos de tamanho aproximadamente igual entre as `n` threads, distribuindo o resto entre as primeiras threads.
- **Pthreads 2 (fila dinâmica)**: usa um mutex (`fila_mutex`) para proteger o índice da próxima linha a ser processada, permitindo balanceamento de carga entre threads.
- **OpenMP**: usa `schedule(dynamic, 1)` para balancear a carga entre as threads automaticamente.

## Tratamento de erros

O projeto trata explicitamente os seguintes casos de erro:

- Ponteiros nulos (arquivo, imagem, configuração)
- Dimensões inválidas (largura/altura ≤ 0)
- Falha de alocação de memória (overflow e `malloc` retornando `NULL`)
- Falha de `fopen()`, `fputc()`, `fprintf()` e `fclose()` ao salvar imagens PGM e o log de tempos
- Falha na criação de threads (`pthread_create`) e do mutex (`pthread_mutex_init`), com liberação correta dos recursos já alocados
- Argumentos de linha de comando inválidos ou fora da faixa permitida

Em caso de erro, o programa imprime uma mensagem descritiva em `stderr` e retorna um código de saída específico (definidos em `main.c`: erro de argumentos, de valor, de memória, de arquivo ou de thread).
