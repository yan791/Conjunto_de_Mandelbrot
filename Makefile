CC = gcc

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

CFLAGS  = -Wall -Wextra -O2 -std=c11 -pthread -I$(INC_DIR)
LDFLAGS = -pthread -lm

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    IS_CLANG := $(shell $(CC) --version 2>/dev/null | grep -qi clang && echo yes || echo no)

    ifeq ($(IS_CLANG),yes)
        LIBOMP_PREFIX := $(shell brew --prefix libomp 2>/dev/null)
        ifeq ($(LIBOMP_PREFIX),)
            $(warning [AVISO] libomp nao encontrada. A versao OpenMP pode nao compilar.)
            $(warning [AVISO] Instale com: brew install libomp)
            OMP_CFLAGS  =
            OMP_LDFLAGS =
        else
            OMP_CFLAGS  = -Xpreprocessor -fopenmp -I$(LIBOMP_PREFIX)/include
            OMP_LDFLAGS = -L$(LIBOMP_PREFIX)/lib -lomp
        endif
    else
        OMP_CFLAGS  = -fopenmp
        OMP_LDFLAGS = -fopenmp
    endif
else
    OMP_CFLAGS  = -fopenmp
    OMP_LDFLAGS = -fopenmp
endif

CFLAGS  += $(OMP_CFLAGS)
LDFLAGS += $(OMP_LDFLAGS)

SRCS   = $(wildcard $(SRC_DIR)/*.c)
OBJS   = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS   = $(wildcard $(INC_DIR)/*.h)
TARGET = mandelbrot

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) mandelbrot_*.pgm times.txt