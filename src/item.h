#pragma once

#define TAM_NOME  50
#define TAM_TIPO  20

#define FLAG_EQUIPAVEL   (1 << 0)
#define FLAG_CONSUMIVEL  (1 << 1)
#define FLAG_RARO        (1 << 2)
#define FLAG_AMALDICOADO (1 << 3)

typedef struct {
    int id;
    char nome[TAM_NOME];
    float peso;
    int valor;
    char tipo[TAM_TIPO];
    unsigned char flags;
} Item;

Item item_criar(int id, const char *nome, float peso, int valor,
                const char *tipo, unsigned char flags);

void item_definir_flag(Item *item, unsigned char flag);
void item_remover_flag(Item *item, unsigned char flag);
int  item_tem_flag(const Item *item, unsigned char flag);

void item_imprimir(const Item *item);
void item_imprimir_flags(const Item *item);
