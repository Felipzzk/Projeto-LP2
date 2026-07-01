#include <stdio.h>
#include <string.h>
#include "eventos.h"

static void callback_pocao_vida(Item *item, Inventario *inv)
{
    (void)inv;
    printf("  [Evento] Voce bebeu %s e recuperou 50 HP!\n", item->nome);
}

static void callback_pocao_mana(Item *item, Inventario *inv)
{
    (void)inv;
    printf("  [Evento] Voce bebeu %s e recuperou 30 MP!\n", item->nome);
}

static void callback_equipar(Item *item, Inventario *inv)
{
    (void)inv;
    if (item_tem_flag(item, FLAG_AMALDICOADO))
        printf("  [Evento] Voce equipou %s... sinto uma energia sombria!\n", item->nome);
    else
        printf("  [Evento] Voce equipou %s.\n", item->nome);
}

static void callback_item_raro(Item *item, Inventario *inv)
{
    (void)inv;
    printf("  [Evento] Item raro ativado: %s brilha intensamente!\n", item->nome);
}

static void callback_generico(Item *item, Inventario *inv)
{
    (void)inv;
    printf("  [Evento] Voce usou %s.\n", item->nome);
}

typedef struct {
    const char *tipo;
    CallbackItem callback;
} EntradaDispatch;

static EntradaDispatch tabela_dispatch[] = {
    { "pocao",    callback_pocao_vida  },
    { "pocao_mp", callback_pocao_mana  },
    { "arma",     callback_equipar     },
    { "armadura", callback_equipar     },
    { NULL,       NULL                 }
};

void eventos_inicializar(void)
{
    /* Tabela estatica ja inicializada; ponto de extensao futuro. */
}

CallbackItem eventos_obter_callback(const Item *item)
{
    if (item == NULL)
        return callback_generico;

    for (int i = 0; tabela_dispatch[i].tipo != NULL; i++) {
        if (strcmp(item->tipo, tabela_dispatch[i].tipo) == 0)
            return tabela_dispatch[i].callback;
    }

    if (item_tem_flag(item, FLAG_CONSUMIVEL))
        return callback_pocao_vida;

    if (item_tem_flag(item, FLAG_EQUIPAVEL))
        return callback_equipar;

    return callback_generico;
}

int eventos_usar_item(Item *item, Inventario *inv)
{
    if (item == NULL || inv == NULL)
        return 0;

    if (item_tem_flag(item, FLAG_AMALDICOADO))
        printf("  AVISO: Este item esta amaldicoado!\n");

    if (item_tem_flag(item, FLAG_RARO))
        callback_item_raro(item, inv);

    CallbackItem cb = eventos_obter_callback(item);
    cb(item, inv);

    if (item_tem_flag(item, FLAG_CONSUMIVEL)) {
        inventario_remover_por_id(inv, item->id);
        printf("  Item consumido e removido do inventario.\n");
    }

    return 1;
}
