#pragma once

#include "item.h"

#define TAM_ACAO 64

// Nó da lista encadeada — cada registro é uma ação do jogador
typedef struct NoHistorico {
    char acao[TAM_ACAO];
    int item_id;
    char item_nome[TAM_NOME];
    struct NoHistorico *proximo;
} NoHistorico;

typedef struct {
    NoHistorico *inicio;  // aponta pro registro mais recente
    int quantidade;
} Historico;

Historico *historico_criar(void);
void historico_destruir(Historico *hist);
void historico_registrar(Historico *hist, const char *acao, int item_id, const char *nome);
void historico_listar(const Historico *hist);
