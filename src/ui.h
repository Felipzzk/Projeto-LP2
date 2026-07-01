#pragma once

void imprimir_borda(void);
void imprimir_linha_titulo(const char *titulo, const char *cor);
void imprimir_linha_opcao(const char *texto);

void ui_animacao(const char *texto);
void ui_imprimir_menu(const char *titulo, const char *cor_titulo,
                      const char *linhas[], int qtd);
