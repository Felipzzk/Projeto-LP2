#include <stdio.h>
#include <string.h>
#include "eventos.h"
#include "cores.h"

void callback_pocao_vida(Item *item, Inventario *inv)
{
    (void)inv;
    printf("  " COR_VERDE "[Evento]" COR_RESET " Voce bebe %s e sente a vida voltar (+50 HP)!\n", item->nome);
}

void callback_pocao_mana(Item *item, Inventario *inv)
{
    (void)inv;
    printf("  " COR_VERDE "[Evento]" COR_RESET " Voce bebe %s e sua mana se restaura (+30 MP)!\n", item->nome);
}

void callback_equipar(Item *item, Inventario *inv)
{
    (void)inv;
    if (item_tem_flag(item, FLAG_AMALDICOADO))
        printf("  " COR_VERDE "[Evento]" COR_RESET " Voce empunha %s... uma energia sombria percorre seu corpo!\n", item->nome);
    else
        printf("  " COR_VERDE "[Evento]" COR_RESET " Voce equipa %s e se prepara para a batalha.\n", item->nome);
}

void callback_item_raro(Item *item, Inventario *inv)
{
    (void)inv;
    printf("  " COR_VERDE "[Evento]" COR_RESET " %s emite um brilho raro e pulsa com poder!\n", item->nome);
}

void callback_generico(Item *item, Inventario *inv)
{
    (void)inv;
    printf("  " COR_VERDE "[Evento]" COR_RESET " Voce interage com %s.\n", item->nome);
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
        printf("  Cuidado! %s carrega uma maldicao antiga!\n", item->nome);

    if (item_tem_flag(item, FLAG_RARO))
        callback_item_raro(item, inv);

    CallbackItem cb = eventos_obter_callback(item);
    cb(item, inv);

    if (item_tem_flag(item, FLAG_CONSUMIVEL)) {
        inventario_remover_por_id(inv, item->id);
        printf("  O item se desfaz em sua mao e desaparece da mochila.\n");
    }

    return 1;
}
