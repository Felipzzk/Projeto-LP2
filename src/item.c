#include <stdio.h>
#include <string.h>
#include "item.h"

// Monta um Item a partir dos campos (garante que strings não estourem)
Item item_criar(int id, const char *nome, float peso, int valor,
                const char *tipo, unsigned char flags)
{
    Item item;
    item.id = id;
    strncpy(item.nome, nome, TAM_NOME - 1);
    item.nome[TAM_NOME - 1] = '\0';  // null terminator de segurança
    item.peso = peso;
    item.valor = valor;
    strncpy(item.tipo, tipo, TAM_TIPO - 1);
    item.tipo[TAM_TIPO - 1] = '\0';
    item.flags = flags;
    return item;
}

// Liga uma flag (OR bit a bit)
void item_definir_flag(Item *item, unsigned char flag)
{
    if (item != NULL)
        item->flags |= flag;
}

// Desliga uma flag (AND com o complemento)
void item_remover_flag(Item *item, unsigned char flag)
{
    if (item != NULL)
        item->flags &= ~flag;
}

// Verifica se a flag está ativa
int item_tem_flag(const Item *item, unsigned char flag)
{
    if (item == NULL)
        return 0;
    return (item->flags & flag) != 0;
}

void item_imprimir_flags(const Item *item)
{
    if (item == NULL)
        return;

    printf("  Flags: ");
    if (item->flags == 0) {
        printf("nenhuma");
    } else {
        int primeiro = 1;
        if (item_tem_flag(item, FLAG_EQUIPAVEL)) {
            printf("%sEquipavel", primeiro ? "" : ", ");
            primeiro = 0;
        }
        if (item_tem_flag(item, FLAG_CONSUMIVEL)) {
            printf("%sConsumivel", primeiro ? "" : ", ");
            primeiro = 0;
        }
        if (item_tem_flag(item, FLAG_RARO)) {
            printf("%sRaro", primeiro ? "" : ", ");
            primeiro = 0;
        }
        if (item_tem_flag(item, FLAG_AMALDICOADO)) {
            printf("%sAmaldicoado", primeiro ? "" : ", ");
        }
    }
    printf("\n");
}

void item_imprimir(const Item *item)
{
    if (item == NULL) {
        printf("  (item nulo)\n");
        return;
    }

    printf("  ID: %d | %s | Peso: %.2f | Valor: %d | Tipo: %s\n",
           item->id, item->nome, item->peso, item->valor, item->tipo);
    item_imprimir_flags(item);
}
