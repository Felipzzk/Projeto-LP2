#pragma once

#include "item.h"
#include "inventario.h"

typedef void (*CallbackItem)(Item *item, Inventario *inv);

void eventos_inicializar(void);
CallbackItem eventos_obter_callback(const Item *item);
int eventos_usar_item(Item *item, Inventario *inv);
