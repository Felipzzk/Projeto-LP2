#pragma once

#include "item.h"
#include "inventario.h"

// Ponteiro de função — cada tipo de item tem sua reação ao usar
typedef void (*CallbackItem)(Item *item, Inventario *inv);

void callback_pocao_vida(Item *item, Inventario *inv);
void callback_pocao_mana(Item *item, Inventario *inv);
void callback_equipar(Item *item, Inventario *inv);
void callback_item_raro(Item *item, Inventario *inv);
void callback_generico(Item *item, Inventario *inv);

CallbackItem eventos_obter_callback(const Item *item);
int eventos_usar_item(Item *item, Inventario *inv);
