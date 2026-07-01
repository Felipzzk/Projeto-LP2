#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"

int catalogo_salvar_texto(const Catalogo *cat, const char *caminho)
{
    if (cat == NULL || caminho == NULL)
        return 0;

    FILE *arq = fopen(caminho, "w");
    if (arq == NULL) {
        printf("Erro ao abrir '%s' para escrita.\n", caminho);
        return 0;
    }

    fprintf(arq, "# Catalogo de Itens RPG\n");
    fprintf(arq, "# Formato: id|nome|peso|valor|tipo|flags\n");

    for (int i = 0; i < cat->quantidade; i++) {
        const Item *item = &cat->itens[i];
        fprintf(arq, "%d|%s|%.2f|%d|%s|%u\n",
                item->id, item->nome, item->peso, item->valor,
                item->tipo, item->flags);
    }

    fclose(arq);
    printf("Catalogo salvo em '%s' (%d itens).\n", caminho, cat->quantidade);
    return 1;
}

int catalogo_carregar_texto(Catalogo *cat, const char *caminho)
{
    if (cat == NULL || caminho == NULL)
        return 0;

    FILE *arq = fopen(caminho, "r");
    if (arq == NULL) {
        printf("Erro ao abrir '%s' para leitura.\n", caminho);
        return 0;
    }

    char linha[256];
    int carregados = 0;

    while (fgets(linha, sizeof(linha), arq) != NULL) {
        if (linha[0] == '#' || linha[0] == '\n')
            continue;

        linha[strcspn(linha, "\n")] = '\0';

        int id, valor;
        unsigned int flags;
        float peso;
        char nome[TAM_NOME];
        char tipo[TAM_TIPO];

        if (sscanf(linha, "%d|%49[^|]|%f|%d|%19[^|]|%u",
                   &id, nome, &peso, &valor, tipo, &flags) == 6) {
            Item item = item_criar(id, nome, peso, valor, tipo, (unsigned char)flags);
            if (catalogo_adicionar(cat, item))
                carregados++;
        }
    }

    fclose(arq);
    printf("Catalogo carregado de '%s' (%d itens).\n", caminho, carregados);
    return carregados > 0;
}

int inventario_salvar_binario(const Inventario *inv, const char *caminho)
{
    if (inv == NULL || caminho == NULL)
        return 0;

    FILE *arq = fopen(caminho, "wb");
    if (arq == NULL) {
        printf("Erro ao abrir '%s' para escrita binaria.\n", caminho);
        return 0;
    }

    fwrite(&inv->quantidade, sizeof(int), 1, arq);
    if (inv->quantidade > 0)
        fwrite(inv->itens, sizeof(Item), inv->quantidade, arq);

    fclose(arq);
    printf("Save salvo em '%s' (%d itens).\n", caminho, inv->quantidade);
    return 1;
}

int inventario_carregar_binario(Inventario *inv, const char *caminho)
{
    if (inv == NULL || caminho == NULL)
        return 0;

    FILE *arq = fopen(caminho, "rb");
    if (arq == NULL) {
        printf("Erro ao abrir '%s' para leitura binaria.\n", caminho);
        return 0;
    }

    int qtd;
    if (fread(&qtd, sizeof(int), 1, arq) != 1) {
        fclose(arq);
        printf("Arquivo de save invalido.\n");
        return 0;
    }

    if (qtd > inv->capacidade) {
        Item *novo = realloc(inv->itens, qtd * sizeof(Item));
        if (novo == NULL) {
            fclose(arq);
            printf("Erro de memoria ao carregar save.\n");
            return 0;
        }
        inv->itens = novo;
        inv->capacidade = qtd;
    }

    inv->quantidade = 0;
    if (qtd > 0) {
        size_t lidos = fread(inv->itens, sizeof(Item), qtd, arq);
        if (lidos != (size_t)qtd) {
            fclose(arq);
            printf("Erro ao ler itens do save.\n");
            inv->quantidade = 0;
            return 0;
        }
        inv->quantidade = qtd;
    }

    fclose(arq);
    printf("Save carregado de '%s' (%d itens).\n", caminho, inv->quantidade);
    return 1;
}
