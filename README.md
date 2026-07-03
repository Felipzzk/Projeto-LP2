# Sistema de Inventário RPG

Projeto em C da disciplina de Linguagem de Programação 2 (LP2).

É um inventário de jogo de RPG no terminal: você cadastra itens, coloca na mochila, usa poções e armas, salva o progresso em arquivo e vê o histórico do que fez.

---

## O que tem no jogo

O programa trabalha com duas listas:

- **Grimório (catálogo)** — todos os itens que existem no jogo.
- **Inventário (mochila)** — itens que o jogador está carregando.

Cada item possui: `id`, `nome`, `peso`, `valor`, `tipo` e `flags` 
(equipável, consumível, raro, amaldiçoado).

Ao **usar** um item, o jogo reage conforme o tipo (poção cura, arma equipa, etc.). Tudo que você adiciona, remove ou usa fica registrado no **histórico**.

---

## Como rodar

Você precisa do **GCC** e, de preferência, do **make**. No Windows, o [MSYS2](https://www.msys2.org/) com GCC funciona bem.


```bash
make
./inventario_rpg
```

Para compilar e executar de uma vez:

```bash
make run
```

Para limpar os arquivos de compilação:

```bash
make clean
```

No Windows (PowerShell ou MSYS2):

```bash
.\inventario_rpg.exe
```

---

## Menus do programa

**Menu principal**

| Opção | O que faz |
|-------|-----------|
| 1 | Grimório — cadastrar, listar e buscar itens |
| 2 | Inventário — mochila do jogador |
| 3 | Salvar jogo (arquivo binário) |
| 4 | Carregar jogo |
| 5 | Ver histórico de transações |
| 0 | Sair (salva grimório e inventário automaticamente) |

Ao abrir, o grimório é carregado de `data/catalogo_itens.txt`. Ao sair com **0**, tudo é salvo de novo.

---

## Arquivos importantes

```
projetolp2/
├── src/          # código-fonte (.c e .h)
├── data/
│   ├── catalogo_itens.txt   # grimório em texto (lido na abertura)
│   └── save_jogador.bin     # save da mochila (criado ao salvar)
├── Makefile
└── inventario_rpg           # executável (depois do make)
```

Os nomes dos arquivos de save estão definidos em `src/arquivo.h`:

```c
#define CAMINHO_CATALOGO "data/catalogo_itens.txt"
#define CAMINHO_SAVE     "data/save_jogador.bin"
```

---

## Formato do catálogo (`.txt`)

Uma linha por item, separada por `|`:

```
id|nome|peso|valor|tipo|flags
```

Linhas com `#` no início são comentário e são ignoradas.

**Flags** (some os valores que quiser):

| Flag | Valor |
|------|-------|
| Equipável | 1 |
| Consumível | 2 |
| Raro | 4 |
| Amaldiçoado | 8 |

Exemplo: poção consumível = `2`. Espada equipável e rara = `1 + 4 = 5`.

---

## Formato do save (`.bin`)

Arquivo binário: primeiro vem a quantidade de itens, depois os dados de cada item em sequência. Só o inventário vai para esse arquivo; o grimório fica no `.txt`.

---


Itens interessantes para testar flags:

| ID | Item | O que esperar ao usar |
|----|------|------------------------|
| 1 | Poção de Vida | Cura e some da mochila |
| 3 | Espada Longa | Equipa e fica na mochila |
| 5 | Anel Amaldiçoado | Aviso de maldição |

---

## Conceitos de LP2 usados no projeto

Structs, alocação dinâmica (`malloc`/`realloc`/`free`), ponteiros, arquivos texto e binário, modularização (`.c`/`.h`), flags com operadores bit a bit, lista encadeada (histórico), quicksort recursivo e callbacks (eventos ao usar item).
