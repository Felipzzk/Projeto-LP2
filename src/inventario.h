#pragma once

#include "item.h"

#define CAPACIDADE_INICIAL 8  // vetor começa pequeno e cresce com realloc

typedef enum {
    ORD_NOME,
    ORD_PESO,
    ORD_VALOR
} CriterioOrdenacao;

// Vetor dinâmico de itens — usado tanto no grimório quanto na mochila
typedef struct {
    Item *itens;
    int quantidade;
    int capacidade;
} VItens;

typedef VItens Inventario;  // mochila do jogador
typedef VItens Catalogo;    // grimório (todos os itens do jogo)

Inventario *inventario_criar(void);
void inventario_destruir(Inventario *inv);

Catalogo *catalogo_criar(void);
void catalogo_destruir(Catalogo *cat);

int catalogo_adicionar(Catalogo *cat, Item item);
int catalogo_remover_por_id(Catalogo *cat, int id);
Item *catalogo_buscar_por_id(Catalogo *cat, int id);
Item *catalogo_buscar_por_nome(Catalogo *cat, const char *nome);
void catalogo_listar(const Catalogo *cat);

int inventario_adicionar(Inventario *inv, Item item);
int inventario_remover_por_id(Inventario *inv, int id);
int inventario_remover_por_indice(Inventario *inv, int indice);
Item *inventario_buscar_por_id(Inventario *inv, int id);
Item *inventario_buscar_por_nome(Inventario *inv, const char *nome);
void inventario_listar(const Inventario *inv);
float inventario_peso_total(const Inventario *inv);

void inventario_ordenar(Inventario *inv, CriterioOrdenacao criterio);

int redimensionar(Item **vetor, int *capacidade);
int comparar_itens(const Item *a, const Item *b, CriterioOrdenacao criterio);
void quicksort_recursivo(Item *itens, int inicio, int fim, CriterioOrdenacao criterio);
int vetor_adicionar(Item **itens, int *quantidade, int *capacidade, Item item);
