#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventario.h"
#include "cores.h"

static int redimensionar(Item **vetor, int *capacidade)
{
    int nova_cap = *capacidade * 2;
    Item *novo = realloc(*vetor, nova_cap * sizeof(Item));
    if (novo == NULL)
        return 0;
    *vetor = novo;
    *capacidade = nova_cap;
    return 1;
}

static int comparar_itens(const Item *a, const Item *b, CriterioOrdenacao criterio)
{
    switch (criterio) {
        case ORD_NOME:
            return strcmp(a->nome, b->nome);
        case ORD_PESO:
            if (a->peso < b->peso) return -1;
            if (a->peso > b->peso) return 1;
            return 0;
        case ORD_VALOR:
            if (a->valor < b->valor) return -1;
            if (a->valor > b->valor) return 1;
            return 0;
        default:
            return 0;
    }
}

static void quicksort_recursivo(Item *itens, int inicio, int fim, CriterioOrdenacao criterio)
{
    if (inicio >= fim)
        return;

    Item pivo = itens[fim];
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        if (comparar_itens(&itens[j], &pivo, criterio) <= 0) {
            i++;
            Item temp = itens[i];
            itens[i] = itens[j];
            itens[j] = temp;
        }
    }

    Item temp = itens[i + 1];
    itens[i + 1] = itens[fim];
    itens[fim] = temp;

    int p = i + 1;
    quicksort_recursivo(itens, inicio, p - 1, criterio);
    quicksort_recursivo(itens, p + 1, fim, criterio);
}

Inventario *inventario_criar(void)
{
    Inventario *inv = malloc(sizeof(Inventario));
    if (inv == NULL)
        return NULL;

    inv->itens = malloc(CAPACIDADE_INICIAL * sizeof(Item));
    if (inv->itens == NULL) {
        free(inv);
        return NULL;
    }

    inv->quantidade = 0;
    inv->capacidade = CAPACIDADE_INICIAL;
    return inv;
}

void inventario_destruir(Inventario *inv)
{
    if (inv == NULL)
        return;
    free(inv->itens);
    free(inv);
}

Catalogo *catalogo_criar(void)
{
    Catalogo *cat = malloc(sizeof(Catalogo));
    if (cat == NULL)
        return NULL;

    cat->itens = malloc(CAPACIDADE_INICIAL * sizeof(Item));
    if (cat->itens == NULL) {
        free(cat);
        return NULL;
    }

    cat->quantidade = 0;
    cat->capacidade = CAPACIDADE_INICIAL;
    return cat;
}

void catalogo_destruir(Catalogo *cat)
{
    if (cat == NULL)
        return;
    free(cat->itens);
    free(cat);
}

static int vetor_adicionar(Item **itens, int *quantidade, int *capacidade, Item item)
{
    if (*quantidade >= *capacidade) {
        if (!redimensionar(itens, capacidade))
            return 0;
    }

    (*itens)[*quantidade] = item;
    (*quantidade)++;
    return 1;
}

int catalogo_adicionar(Catalogo *cat, Item item)
{
    if (cat == NULL)
        return 0;

    if (catalogo_buscar_por_id(cat, item.id) != NULL) {
        printf("Erro: ja existe item com ID %d no grimorio.\n", item.id);
        return 0;
    }

    return vetor_adicionar(&cat->itens, &cat->quantidade, &cat->capacidade, item);
}

int catalogo_remover_por_id(Catalogo *cat, int id)
{
    if (cat == NULL)
        return 0;

    for (int i = 0; i < cat->quantidade; i++) {
        if (cat->itens[i].id == id) {
            for (int j = i; j < cat->quantidade - 1; j++)
                cat->itens[j] = cat->itens[j + 1];
            cat->quantidade--;
            return 1;
        }
    }
    return 0;
}

Item *catalogo_buscar_por_id(Catalogo *cat, int id)
{
    if (cat == NULL)
        return NULL;

    for (int i = 0; i < cat->quantidade; i++) {
        if (cat->itens[i].id == id)
            return &cat->itens[i];
    }
    return NULL;
}

Item *catalogo_buscar_por_nome(Catalogo *cat, const char *nome)
{
    if (cat == NULL || nome == NULL)
        return NULL;

    for (int i = 0; i < cat->quantidade; i++) {
        if (strcmp(cat->itens[i].nome, nome) == 0)
            return &cat->itens[i];
    }
    return NULL;
}

void catalogo_listar(const Catalogo *cat)
{
    if (cat == NULL || cat->quantidade == 0) {
        printf("Grimorio vazio.\n");
        return;
    }

    printf("\n" COR_MAGENTA "=== Grimorio de Itens (%d itens) ===\n" COR_RESET, cat->quantidade);
    for (int i = 0; i < cat->quantidade; i++) {
        printf("[%d] ", i);
        item_imprimir(&cat->itens[i]);
    }
    printf("===================================\n");
}

int inventario_adicionar(Inventario *inv, Item item)
{
    if (inv == NULL)
        return 0;
    return vetor_adicionar(&inv->itens, &inv->quantidade, &inv->capacidade, item);
}

int inventario_remover_por_id(Inventario *inv, int id)
{
    if (inv == NULL)
        return 0;

    for (int i = 0; i < inv->quantidade; i++) {
        if (inv->itens[i].id == id) {
            for (int j = i; j < inv->quantidade - 1; j++)
                inv->itens[j] = inv->itens[j + 1];
            inv->quantidade--;
            return 1;
        }
    }
    return 0;
}

int inventario_remover_por_indice(Inventario *inv, int indice)
{
    if (inv == NULL || indice < 0 || indice >= inv->quantidade)
        return 0;

    for (int j = indice; j < inv->quantidade - 1; j++)
        inv->itens[j] = inv->itens[j + 1];
    inv->quantidade--;
    return 1;
}

Item *inventario_buscar_por_id(Inventario *inv, int id)
{
    if (inv == NULL)
        return NULL;

    for (int i = 0; i < inv->quantidade; i++) {
        if (inv->itens[i].id == id)
            return &inv->itens[i];
    }
    return NULL;
}

Item *inventario_buscar_por_nome(Inventario *inv, const char *nome)
{
    if (inv == NULL || nome == NULL)
        return NULL;

    for (int i = 0; i < inv->quantidade; i++) {
        if (strcmp(inv->itens[i].nome, nome) == 0)
            return &inv->itens[i];
    }
    return NULL;
}

void inventario_listar(const Inventario *inv)
{
    if (inv == NULL || inv->quantidade == 0) {
        printf("Inventario vazio.\n");
        return;
    }

    printf("\n" COR_AMARELO "=== Inventario (%d itens) ===\n" COR_RESET, inv->quantidade);
    Item *ptr = inv->itens;
    for (int i = 0; i < inv->quantidade; i++, ptr++) {
        printf("[%d] ", i);
        item_imprimir(ptr);
    }
    printf("Peso total: %.2f\n", inventario_peso_total(inv));
    printf("=============================\n");
}

float inventario_peso_total(const Inventario *inv)
{
    if (inv == NULL)
        return 0.0f;

    float total = 0.0f;
    Item *ptr = inv->itens;
    for (int i = 0; i < inv->quantidade; i++, ptr++)
        total += ptr->peso;
    return total;
}

void inventario_ordenar(Inventario *inv, CriterioOrdenacao criterio)
{
    if (inv == NULL || inv->quantidade <= 1)
        return;
    quicksort_recursivo(inv->itens, 0, inv->quantidade - 1, criterio);
}
