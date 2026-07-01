#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historico.h"
#include "cores.h"

Historico *historico_criar(void)
{
    Historico *hist = malloc(sizeof(Historico));
    if (hist == NULL)
        return NULL;

    hist->inicio = NULL;
    hist->quantidade = 0;
    return hist;
}

void historico_destruir(Historico *hist)
{
    if (hist == NULL)
        return;

    NoHistorico *atual = hist->inicio;
    while (atual != NULL) {
        NoHistorico *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    free(hist);
}

void historico_registrar(Historico *hist, const char *acao, int item_id, const char *nome)
{
    if (hist == NULL || acao == NULL)
        return;

    NoHistorico *novo = malloc(sizeof(NoHistorico));
    if (novo == NULL)
        return;

    strncpy(novo->acao, acao, TAM_ACAO - 1);
    novo->acao[TAM_ACAO - 1] = '\0';
    novo->item_id = item_id;
    if (nome != NULL) {
        strncpy(novo->item_nome, nome, TAM_NOME - 1);
        novo->item_nome[TAM_NOME - 1] = '\0';
    } else {
        novo->item_nome[0] = '\0';
    }
    novo->proximo = hist->inicio;
    hist->inicio = novo;
    hist->quantidade++;
}

void historico_listar(const Historico *hist)
{
    if (hist == NULL || hist->inicio == NULL) {
        printf("Historico vazio.\n");
        return;
    }

    printf("\n" COR_CIANO "=== Historico de Transacoes (%d registros) ===\n" COR_RESET, hist->quantidade);
    int i = 1;
    const NoHistorico *atual = hist->inicio;
    while (atual != NULL) {
        const char *cor_acao = COR_RESET;
        if (strcmp(atual->acao, "ADICIONOU") == 0)
            cor_acao = COR_VERDE;

        printf("  %d. [%s%s%s] ID %d - %s\n",
               i++, cor_acao, atual->acao, COR_RESET, atual->item_id, atual->item_nome);
        atual = atual->proximo;
    }
    printf(COR_CIANO "==============================================\n" COR_RESET);
}
