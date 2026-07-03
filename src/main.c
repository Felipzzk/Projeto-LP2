// Ponto de entrada do programa — cria as estruturas e roda o menu principal
#include <stdio.h>
#include <stdlib.h>
#include "inventario.h"
#include "arquivo.h"
#include "eventos.h"
#include "ui.h"
#include "menus.h"

int main(void)
{
    // As três "pilhas" do jogo: catálogo, mochila e log de ações
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

    ui_animacao("Carregando o mundo");
    catalogo_carregar_texto(catalogo, CAMINHO_CATALOGO);  // lê o grimório do .txt

    menu_principal(catalogo, inventario, historico);

    // Libera tudo antes de sair
    catalogo_destruir(catalogo);
    inventario_destruir(inventario);
    historico_destruir(historico);
    return 0;
}
