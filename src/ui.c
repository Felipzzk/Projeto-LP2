// Visual do terminal — caixas, cores e animação de "carregando..."
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ui.h"
#include "cores.h"

#define LARGURA_CAIXA 42

void imprimir_borda(void)
{
    printf("+");
    for (int i = 0; i < LARGURA_CAIXA; i++)
        printf("-");
    printf("+\n");
}

void imprimir_linha_titulo(const char *titulo, const char *cor)
{
    int len = (int)strlen(titulo);
    int espaco = LARGURA_CAIXA - len;
    if (espaco < 0)
        espaco = 0;
    int esq = espaco / 2;
    int dir = espaco - esq;

    printf("|");
    printf("%*s", esq, "");
    printf("%s" COR_NEGRITO "%s" COR_RESET, cor, titulo);
    printf("%*s|\n", dir, "");
}

void imprimir_linha_opcao(const char *texto)
{
    int len = (int)strlen(texto);
    int dir = LARGURA_CAIXA - len;
    if (dir < 0)
        dir = 0;

    printf("|%s%*s|\n", texto, dir, "");
}

// Imprime "Carregando..." com três pontinhos aparecendo aos poucos
void ui_animacao(const char *texto)
{
    printf("%s", texto);
    fflush(stdout);

    for (int i = 0; i < 3; i++) {
        usleep(400000);  // 400ms entre cada ponto
        printf(".");
        fflush(stdout);
    }

    printf("\n");
}

void ui_imprimir_menu(const char *titulo, const char *cor_titulo,
                      const char *linhas[], int qtd)
{
    imprimir_borda();
    imprimir_linha_titulo(titulo, cor_titulo);
    imprimir_borda();

    for (int i = 0; i < qtd; i++)
        imprimir_linha_opcao(linhas[i]);

    imprimir_borda();
    printf("\n");
}
