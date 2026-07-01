#pragma once

#include "inventario.h"

#define CAMINHO_CATALOGO "data/catalogo_itens.txt"
#define CAMINHO_SAVE     "data/save_jogador.dat"

int catalogo_salvar_texto(const Catalogo *cat, const char *caminho);
int catalogo_carregar_texto(Catalogo *cat, const char *caminho);

int inventario_salvar_binario(const Inventario *inv, const char *caminho);
int inventario_carregar_binario(Inventario *inv, const char *caminho);
