#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "inventario.h"
#include "arquivo.h"
#include "eventos.h"
#include "historico.h"

static void limpar_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static int ler_inteiro(const char *prompt)
{
    int valor;
    printf("%s", prompt);
    if (scanf("%d", &valor) != 1) {
        limpar_buffer();
        return -1;
    }
    limpar_buffer();
    return valor;
}

static unsigned char ler_flags(void)
{
    unsigned char flags = 0;
    int op;

    printf("  Flags (0=nao, 1=sim para cada):\n");
    printf("    Equipavel? "); scanf("%d", &op); if (op) flags |= FLAG_EQUIPAVEL;
    printf("    Consumivel? "); scanf("%d", &op); if (op) flags |= FLAG_CONSUMIVEL;
    printf("    Raro? ");      scanf("%d", &op); if (op) flags |= FLAG_RARO;
    printf("    Amaldicoado? "); scanf("%d", &op); if (op) flags |= FLAG_AMALDICOADO;
    limpar_buffer();
    return flags;
}

static void menu_cadastrar_item(Catalogo *cat)
{
    int id = ler_inteiro("  ID do item: ");
    if (id < 0) { printf("ID invalido.\n"); return; }

    if (catalogo_buscar_por_id(cat, id) != NULL) {
        printf("Ja existe item com esse ID.\n");
        return;
    }

    char nome[TAM_NOME], tipo[TAM_TIPO];
    float peso;
    int valor;

    printf("  Nome: ");
    if (scanf("%49[^\n]", nome) != 1) { limpar_buffer(); return; }
    limpar_buffer();

    printf("  Peso: ");
    if (scanf("%f", &peso) != 1) { limpar_buffer(); return; }
    limpar_buffer();

    valor = ler_inteiro("  Valor: ");
    if (valor < 0) return;

    printf("  Tipo (pocao, pocao_mp, arma, armadura, misc): ");
    if (scanf("%19[^\n]", tipo) != 1) { limpar_buffer(); return; }
    limpar_buffer();

    unsigned char flags = ler_flags();
    Item item = item_criar(id, nome, peso, valor, tipo, flags);

    if (catalogo_adicionar(cat, item))
        printf("Item cadastrado com sucesso!\n");
}

static void menu_catalogo(Catalogo *cat)
{
    int op;
    do {
        printf("\n--- Catalogo de Itens ---\n");
        printf("  1. Cadastrar item\n");
        printf("  2. Listar catalogo\n");
        printf("  3. Buscar por ID\n");
        printf("  4. Buscar por nome\n");
        printf("  5. Salvar catalogo (texto)\n");
        printf("  6. Carregar catalogo (texto)\n");
        printf("  0. Voltar\n");
        op = ler_inteiro("Opcao: ");

        switch (op) {
            case 1: menu_cadastrar_item(cat); break;
            case 2: catalogo_listar(cat); break;
            case 3: {
                int id = ler_inteiro("  ID: ");
                Item *item = catalogo_buscar_por_id(cat, id);
                if (item) item_imprimir(item);
                else printf("Item nao encontrado.\n");
                break;
            }
            case 4: {
                char nome[TAM_NOME];
                printf("  Nome: ");
                if (scanf("%49[^\n]", nome) == 1) {
                    limpar_buffer();
                    Item *item = catalogo_buscar_por_nome(cat, nome);
                    if (item) item_imprimir(item);
                    else printf("Item nao encontrado.\n");
                } else limpar_buffer();
                break;
            }
            case 5: catalogo_salvar_texto(cat, CAMINHO_CATALOGO); break;
            case 6: catalogo_carregar_texto(cat, CAMINHO_CATALOGO); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

static void menu_inventario(Inventario *inv, Catalogo *cat, Historico *hist)
{
    int op;
    do {
        printf("\n--- Inventario do Jogador ---\n");
        printf("  1. Adicionar item (do catalogo)\n");
        printf("  2. Remover item\n");
        printf("  3. Usar item\n");
        printf("  4. Listar inventario\n");
        printf("  5. Buscar por ID\n");
        printf("  6. Buscar por nome\n");
        printf("  7. Ordenar inventario\n");
        printf("  0. Voltar\n");
        op = ler_inteiro("Opcao: ");

        switch (op) {
            case 1: {
                if (cat->quantidade == 0) {
                    printf("Catalogo vazio. Cadastre itens primeiro.\n");
                    break;
                }
                printf("\nItens disponiveis no catalogo:\n");
                catalogo_listar(cat);
                int id = ler_inteiro("  ID do item no catalogo: ");
                Item *origem = catalogo_buscar_por_id(cat, id);
                if (origem == NULL) {
                    printf("Item nao encontrado no catalogo.\n");
                    break;
                }
                if (inventario_adicionar(inv, *origem)) {
                    historico_registrar(hist, "ADICIONOU", origem->id, origem->nome);
                    printf("'%s' adicionado ao inventario.\n", origem->nome);
                }
                break;
            }
            case 2: {
                int id = ler_inteiro("  ID do item: ");
                Item *item = inventario_buscar_por_id(inv, id);
                if (item && inventario_remover_por_id(inv, id)) {
                    historico_registrar(hist, "REMOVEU", id, item->nome);
                    printf("Item removido.\n");
                } else {
                    printf("Item nao encontrado.\n");
                }
                break;
            }
            case 3: {
                int id = ler_inteiro("  ID do item: ");
                Item *item = inventario_buscar_por_id(inv, id);
                if (item == NULL) {
                    printf("Item nao encontrado.\n");
                    break;
                }
                historico_registrar(hist, "USOU", item->id, item->nome);
                eventos_usar_item(item, inv);
                break;
            }
            case 4: inventario_listar(inv); break;
            case 5: {
                int id = ler_inteiro("  ID: ");
                Item *item = inventario_buscar_por_id(inv, id);
                if (item) item_imprimir(item);
                else printf("Item nao encontrado.\n");
                break;
            }
            case 6: {
                char nome[TAM_NOME];
                printf("  Nome: ");
                if (scanf("%49[^\n]", nome) == 1) {
                    limpar_buffer();
                    Item *item = inventario_buscar_por_nome(inv, nome);
                    if (item) item_imprimir(item);
                    else printf("Item nao encontrado.\n");
                } else limpar_buffer();
                break;
            }
            case 7: {
                printf("  Ordenar por: 1=Nome  2=Peso  3=Valor\n");
                int c = ler_inteiro("  Criterio: ");
                CriterioOrdenacao crit;
                switch (c) {
                    case 1: crit = ORD_NOME; break;
                    case 2: crit = ORD_PESO; break;
                    case 3: crit = ORD_VALOR; break;
                    default: printf("Criterio invalido.\n"); continue;
                }
                inventario_ordenar(inv, crit);
                printf("Inventario ordenado.\n");
                break;
            }
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

int main(void)
{
    Catalogo *catalogo = catalogo_criar();
    Inventario *inventario = inventario_criar();
    Historico *historico = historico_criar();

    if (catalogo == NULL || inventario == NULL || historico == NULL) {
        printf("Erro fatal: falha na alocacao de memoria.\n");
        catalogo_destruir(catalogo);
        inventario_destruir(inventario);
        historico_destruir(historico);
        return 1;
    }

    eventos_inicializar();
    catalogo_carregar_texto(catalogo, CAMINHO_CATALOGO);

    int op;
    do {
        printf("\n========================================\n");
        printf("   Sistema de Inventario RPG\n");
        printf("========================================\n");
        printf("  1. Gerenciar catalogo de itens\n");
        printf("  2. Gerenciar inventario do jogador\n");
        printf("  3. Salvar jogo (binario)\n");
        printf("  4. Carregar jogo (binario)\n");
        printf("  5. Ver historico de transacoes\n");
        printf("  0. Sair\n");
        op = ler_inteiro("Opcao: ");

        switch (op) {
            case 1: menu_catalogo(catalogo); break;
            case 2: menu_inventario(inventario, catalogo, historico); break;
            case 3: inventario_salvar_binario(inventario, CAMINHO_SAVE); break;
            case 4: inventario_carregar_binario(inventario, CAMINHO_SAVE); break;
            case 5: historico_listar(historico); break;
            case 0:
                catalogo_salvar_texto(catalogo, CAMINHO_CATALOGO);
                inventario_salvar_binario(inventario, CAMINHO_SAVE);
                printf("Dados salvos. Ate logo!\n");
                break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);

    catalogo_destruir(catalogo);
    inventario_destruir(inventario);
    historico_destruir(historico);
    return 0;
}
