// Menus interativos e leitura de entrada do usuário
#include <stdio.h>
#include <string.h>
#include "menus.h"
#include "item.h"
#include "arquivo.h"
#include "eventos.h"
#include "cores.h"
#include "ui.h"

// Joga fora o que sobrou no stdin depois de um scanf
void limpar_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int ler_inteiro(const char *prompt)
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

// Pergunta flag por flag e monta o byte com OR
unsigned char ler_flags(void)
{
    unsigned char flags = 0;
    int op;

    printf("  Flags (0=nao, 1=sim para cada):\n");
    printf("    Equipavel? ");
    scanf("%d", &op);
    if (op) flags |= FLAG_EQUIPAVEL;
    printf("    Consumivel? ");
    scanf("%d", &op);
    if (op) flags |= FLAG_CONSUMIVEL;
    printf("    Raro? ");
    scanf("%d", &op);
    if (op) flags |= FLAG_RARO;
    printf("    Amaldicoado? ");
    scanf("%d", &op);
    if (op) flags |= FLAG_AMALDICOADO;
    limpar_buffer();
    return flags;
}

// Formulário pra cadastrar um item novo no grimório
void menu_cadastrar_item(Catalogo *cat)
{
    int id = ler_inteiro("  ID do item: ");
    if (id < 0) {
    printf("ID invalido.\n");
    return;
    }

    if (catalogo_buscar_por_id(cat, id) != NULL) {
        printf("Ja existe item com esse ID.\n");
        return;
    }

    char nome[TAM_NOME], tipo[TAM_TIPO];
    float peso;
    int valor;

    printf("  Nome: ");
    if (scanf("%49[^\n]", nome) != 1) {
    limpar_buffer();
    return;
    }
    limpar_buffer();

    printf("  Peso: ");
    if (scanf("%f", &peso) != 1) {
    limpar_buffer();
    return;
    }
    limpar_buffer();

    valor = ler_inteiro("  Valor: ");
    if (valor < 0) return;

    printf("  Tipo (pocao, pocao_mp, arma, armadura, misc): ");
    if (scanf("%19[^\n]", tipo) != 1) {
    limpar_buffer();
    return;
    }
    limpar_buffer();

    unsigned char flags = ler_flags();
    Item item = item_criar(id, nome, peso, valor, tipo, flags);

    if (catalogo_adicionar(cat, item))
        printf(COR_VERDE "O item foi registrado no grimorio do reino!\n" COR_RESET);
}

void menu_catalogo(Catalogo *cat)
{
    static const char *opcoes[] = {
        "  1. Cadastrar item",
        "  2. Listar grimorio",
        "  3. Buscar por ID",
        "  4. Buscar por nome",
        "  5. Salvar grimorio (texto)",
        "  6. Carregar grimorio (texto)",
        "  0. Voltar"
    };

    int op;
    do {
        ui_imprimir_menu("Grimorio de Itens", COR_MAGENTA, opcoes, 7);
        op = ler_inteiro("Opcao: ");

        switch (op) {
            case 1: menu_cadastrar_item(cat); break;
            case 2: catalogo_listar(cat); break;
            case 3: {
                int id = ler_inteiro("  ID: ");
                Item *item = catalogo_buscar_por_id(cat, id);
                if (item) item_imprimir(item);
                else printf("Nenhum artefato com esse ID foi encontrado.\n");
                break;
            }
            case 4: {
                char nome[TAM_NOME];
                printf("  Nome: ");
                if (scanf("%49[^\n]", nome) == 1) {
                    limpar_buffer();
                    Item *item = catalogo_buscar_por_nome(cat, nome);
                    if (item) item_imprimir(item);
                    else printf("Nenhum artefato com esse nome foi encontrado.\n");
                } else limpar_buffer();
                break;
            }
            case 5: catalogo_salvar_texto(cat, CAMINHO_CATALOGO); break;
            case 6: catalogo_carregar_texto(cat, CAMINHO_CATALOGO); break;
            case 0: break;
            default: printf("Essa passagem nao existe.\n");
        }
    } while (op != 0);
}

void menu_inventario(Inventario *inv, Catalogo *cat, Historico *hist)
{
    static const char *opcoes[] = {
        "  1. Adicionar item (do grimorio)",
        "  2. Remover item",
        "  3. Usar item",
        "  4. Listar inventario",
        "  5. Buscar por ID",
        "  6. Buscar por nome",
        "  7. Ordenar inventario",
        "  0. Voltar"
    };

    int op;
    do {
        ui_imprimir_menu("Inventario do Jogador", COR_AMARELO, opcoes, 8);
        op = ler_inteiro("Opcao: ");

        switch (op) {
            case 1: {
                // Copia um item do grimório pra mochila
                if (cat->quantidade == 0) {
                    printf("O grimorio esta vazio. Cadastre itens primeiro.\n");
                    break;
                }
                printf("\nArtefatos disponiveis no grimorio:\n");
                catalogo_listar(cat);
                int id = ler_inteiro("  ID do item no grimorio: ");
                Item *origem = catalogo_buscar_por_id(cat, id);
                if (origem == NULL) {
                    printf("Esse artefato nao consta no grimorio.\n");
                    break;
                }
                if (inventario_adicionar(inv, *origem)) {
                    historico_registrar(hist, "ADICIONOU", origem->id, origem->nome);
                    printf(COR_VERDE "Voce guardou '%s' na mochila.\n" COR_RESET, origem->nome);
                }
                break;
            }
            case 2: {
                if (inv->quantidade == 0) {
                    printf("Sua mochila esta vazia.\n");
                    break;
                }
                printf("\nItens na sua mochila:\n");
                inventario_listar(inv);
                int id = ler_inteiro("  ID do item: ");
                Item *item = inventario_buscar_por_id(inv, id);
                if (item && inventario_remover_por_id(inv, id)) {
                    historico_registrar(hist, "REMOVEU", id, item->nome);
                    printf(COR_VERDE "Voce largou o item fora da mochila.\n" COR_RESET);
                } else {
                    printf("Esse item nao esta na sua mochila.\n");
                }
                break;
            }
            case 3: {
                // Usar item dispara o callback certo (poção, arma, etc.)
                if (inv->quantidade == 0) {
                    printf("Sua mochila esta vazia.\n");
                    break;
                }
                printf("\nItens na sua mochila:\n");
                inventario_listar(inv);
                int id = ler_inteiro("  ID do item: ");
                Item *item = inventario_buscar_por_id(inv, id);
                if (item == NULL) {
                    printf("Esse item nao esta na sua mochila.\n");
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
                else printf("Nenhum artefato com esse ID foi encontrado.\n");
                break;
            }
            case 6: {
                char nome[TAM_NOME];
                printf("  Nome: ");
                if (scanf("%49[^\n]", nome) == 1) {
                    limpar_buffer();
                    Item *item = inventario_buscar_por_nome(inv, nome);
                    if (item) item_imprimir(item);
                    else printf("Nenhum artefato com esse nome foi encontrado.\n");
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
                    default: printf("Esse criterio de ordenacao nao existe.\n"); continue;
                }
                inventario_ordenar(inv, crit);
                printf(COR_VERDE "A mochila foi reorganizada.\n" COR_RESET);
                break;
            }
            case 0: break;
            default: printf("Essa passagem nao existe.\n");
        }
    } while (op != 0);
}

// Loop principal — fica rodando até o jogador escolher sair (0)
void menu_principal(Catalogo *catalogo, Inventario *inventario, Historico *historico)
{
    int op;
    static const char *opcoes[] = {
        "  1. Gerenciar grimorio de itens",
        "  2. Gerenciar inventario do jogador",
        "  3. Salvar jogo (binario)",
        "  4. Carregar jogo (binario)",
        "  5. Ver historico de transacoes",
        "  0. Sair"
    };

    do {
        ui_imprimir_menu("Sistema de Inventario RPG", COR_CIANO, opcoes, 6);
        op = ler_inteiro("Opcao: ");

        switch (op) {
            case 1: menu_catalogo(catalogo); break;
            case 2: menu_inventario(inventario, catalogo, historico); break;
            case 3: inventario_salvar_binario(inventario, CAMINHO_SAVE); break;
            case 4: inventario_carregar_binario(inventario, CAMINHO_SAVE); break;
            case 5: historico_listar(historico); break;
            case 0:
                // Ao sair, salva tudo automaticamente
                ui_animacao("Selando seu progresso");
                catalogo_salvar_texto(catalogo, CAMINHO_CATALOGO);
                inventario_salvar_binario(inventario, CAMINHO_SAVE);
                printf(COR_VERDE "Aventureiro, seus dados foram salvos. Ate a proxima!\n" COR_RESET);
                break;
            default: printf("Essa passagem nao existe.\n");
        }
    } while (op != 0);
}
