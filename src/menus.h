#pragma once

#include "inventario.h"
#include "historico.h"

void limpar_buffer(void);
int ler_inteiro(const char *prompt);
unsigned char ler_flags(void);

void menu_cadastrar_item(Catalogo *cat);
void menu_catalogo(Catalogo *cat);
void menu_inventario(Inventario *inv, Catalogo *cat, Historico *hist);
void menu_principal(Catalogo *catalogo, Inventario *inventario, Historico *historico);
