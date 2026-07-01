# Sistema de Inventário RPG

Programa em C que simula o inventário de um jogo de RPG: cadastro de itens, gerenciamento do inventário do jogador, persistência em arquivo e eventos ao usar itens.

Projeto da disciplina de Linguagem de Programação 2 (LP2).

---

## O que o programa faz

O sistema separa duas coleções de itens:

- **Catálogo** — todos os itens disponíveis no jogo (cadastro mestre).
- **Inventário** — itens que o jogador está carregando.

Cada item possui: `id`, `nome`, `peso`, `valor`, `tipo` e `flags` (equipável, consumível, raro, amaldiçoado).

### Funcionalidades

| Área | O que é possível fazer |
|---|---|
| Catálogo | Cadastrar, listar, buscar por ID/nome, salvar/carregar arquivo texto |
| Inventário | Adicionar (do catálogo), remover, usar, listar, buscar, ordenar |
| Persistência | Save do jogador em arquivo binário |
| Histórico | Ver transações (adicionou, removeu, usou) em lista encadeada |
| Eventos | Ao usar item, dispara callback conforme o tipo (poção, arma, etc.) |

---

## Estrutura do projeto

```
projetolp2/
├── src/
│   ├── main.c           # Menu e loop principal
│   ├── item.c / item.h           # Struct Item e flags bit a bit
│   ├── inventario.c / inventario.h # Vetor dinâmico (catálogo + inventário)
│   ├── arquivo.c / arquivo.h       # Leitura/escrita de arquivos
│   ├── eventos.c / eventos.h       # Callbacks ao usar itens
│   └── historico.c / historico.h   # Lista encadeada de transações
├── data/
│   ├── catalogo_itens.txt  # Catálogo em texto (carregado na abertura)
│   └── save_jogador.dat    # Save binário (criado ao salvar)
├── build/                  # Arquivos objeto (.o) — gerado na compilação
├── Makefile
└── README.md
```

---

## Pré-requisitos

- **GCC** (ou outro compilador C compatível com C99)
- Terminal/Prompt de comando
- **Opcional:** `make` (GNU Make) — facilita compilar, mas não é obrigatório

No Windows, o [MSYS2](https://www.msys2.org/) com o pacote `mingw-w64-ucrt-x86_64-gcc` funciona bem.

---

## Como compilar

Execute os comandos na **raiz do projeto** (`projetolp2/`), pois o programa lê arquivos em `data/`.

### Opção 1 — Com Makefile (recomendado no Linux/macOS/MSYS2)

```bash
make
```

Para compilar e executar de uma vez:

```bash
make run
```

Para limpar artefatos de compilação:

```bash
make clean
```

### Opção 2 — Compilação manual (funciona em qualquer sistema com GCC)

**Linux/macOS/MSYS2:**

```bash
mkdir -p build
gcc -Wall -Wextra -std=c99 -g -c src/main.c       -o build/main.o
gcc -Wall -Wextra -std=c99 -g -c src/item.c       -o build/item.o
gcc -Wall -Wextra -std=c99 -g -c src/inventario.c -o build/inventario.o
gcc -Wall -Wextra -std=c99 -g -c src/arquivo.c    -o build/arquivo.o
gcc -Wall -Wextra -std=c99 -g -c src/eventos.c    -o build/eventos.o
gcc -Wall -Wextra -std=c99 -g -c src/historico.c  -o build/historico.o
gcc -Wall -Wextra -std=c99 -g -o inventario_rpg build/*.o
```

**Windows (PowerShell):**

```powershell
New-Item -ItemType Directory -Force -Path build
gcc -Wall -Wextra -std=c99 -g -c src/main.c       -o build/main.o
gcc -Wall -Wextra -std=c99 -g -c src/item.c       -o build/item.o
gcc -Wall -Wextra -std=c99 -g -c src/inventario.c -o build/inventario.o
gcc -Wall -Wextra -std=c99 -g -c src/arquivo.c    -o build/arquivo.o
gcc -Wall -Wextra -std=c99 -g -c src/eventos.c    -o build/eventos.o
gcc -Wall -Wextra -std=c99 -g -c src/historico.c  -o build/historico.o
gcc -Wall -Wextra -std=c99 -g -o inventario_rpg.exe build/main.o build/item.o build/inventario.o build/arquivo.o build/eventos.o build/historico.o
```

---

## Como executar

Na raiz do projeto:

```bash
./inventario_rpg        # Linux/macOS/MSYS2
.\inventario_rpg.exe    # Windows
```

Na abertura, o catálogo em `data/catalogo_itens.txt` é carregado automaticamente. Ao sair (opção **0**), o programa salva o catálogo e o inventário automaticamente.

---

## Roteiro de testes

Sugestão de fluxo para quem for avaliar o programa:

### 1. Catálogo

1. Execute o programa.
2. Menu principal → **1** (Gerenciar catálogo).
3. **2** — Listar catálogo (deve mostrar 6 itens de exemplo).
4. **3** — Buscar por ID `3` (Espada Longa).
5. **1** — Cadastrar um item novo (ex.: ID `7`, nome `Arco`, peso `2.5`, valor `180`, tipo `arma`, flags equipável).
6. **5** — Salvar catálogo e conferir o arquivo `data/catalogo_itens.txt`.

### 2. Inventário

1. Menu principal → **2** (Gerenciar inventário).
2. **1** — Adicionar item ID `1` (Poção de Vida).
3. **1** — Adicionar item ID `3` (Espada Longa).
4. **4** — Listar inventário (verificar peso total: `3.50`).
5. **3** — Usar item ID `1` (deve exibir evento de poção e remover do inventário).
6. **3** — Usar item ID `3` (deve exibir evento de equipar; item permanece).
7. **7** — Ordenar por nome, depois listar novamente.

### 3. Busca

1. No inventário, **5** — Buscar por ID.
2. No inventário, **6** — Buscar por nome (`Espada Longa`).

### 4. Save binário

1. Menu principal → **3** — Salvar jogo.
2. Remova todos os itens do inventário.
3. Menu principal → **4** — Carregar jogo (itens devem voltar).

### 5. Histórico

1. Menu principal → **5** — Ver histórico de transações (adicionou, usou, removeu).

### 6. Flags especiais

Teste itens do catálogo com flags distintas:

| ID | Item | Flags | Comportamento esperado ao usar |
|---|---|---|---|
| 1 | Poção de Vida | Consumível | Recupera HP, item é removido |
| 3 | Espada Longa | Equipável + Raro | Equipa + brilho de item raro |
| 5 | Anel Amaldiçoado | Raro + Amaldiçoado | Aviso de maldição |

---

## Formato dos arquivos

### Catálogo (`data/catalogo_itens.txt`)

Arquivo texto, uma linha por item:

```
id|nome|peso|valor|tipo|flags
```

Linhas começando com `#` são comentários.

**Flags** (valores somados):

| Flag | Valor |
|---|---|
| Equipável | 1 |
| Consumível | 2 |
| Raro | 4 |
| Amaldiçoado | 8 |

Exemplo: poção consumível = `2`; espada equipável e rara = `1 + 4 = 5`.

### Save (`data/save_jogador.dat`)

Arquivo binário com a quantidade de itens seguida dos structs `Item` em sequência.

---

## Sobre o Makefile

O `Makefile` **não é obrigatório** — ele só automatiza a compilação. Sem ele, você compila com `gcc` manualmente (veja acima).

### O que cada parte faz

| Trecho | Função |
|---|---|
| `CC = gcc` | Define o compilador |
| `CFLAGS = -Wall -Wextra -std=c99 -g` | Flags: avisos, padrão C99, símbolos de debug |
| `TARGET = inventario_rpg` | Nome do executável gerado |
| `SRCS` / `OBJS` | Lista dos `.c` e dos `.o` correspondentes |
| `all` | Alvo padrão: cria `build/` e gera o executável |
| `$(OBJDIR)/%.o` | Regra que compila cada `.c` em `.o` separado |
| `$(TARGET)` | Liga todos os `.o` no executável final |
| `run` | Compila (se necessário) e executa |
| `clean` | Remove `build/` e o executável |

### Por que usar Makefile?

- **Compilação incremental:** só recompila arquivos `.c` que mudaram.
- **Menos digitação:** `make` em vez de vários comandos `gcc`.
- **Padrão em projetos C:** facilita para quem já conhece.

No Windows puro (sem MSYS2), o `make` muitas vezes não está instalado — nesse caso, use a compilação manual ou instale o Make pelo MSYS2.

---

## Conceitos de LP2 utilizados

- Structs (`Item`, `Inventario`, `Catalogo`)
- Alocação dinâmica (`malloc`, `realloc`, `free`)
- Ponteiros e passagem por referência
- Arquivos texto (`fprintf`, `fscanf`) e binários (`fwrite`, `fread`)
- Modularização (`.c` / `.h` com `#pragma once`)
- Operadores bit a bit (flags de item)
- Recursão (quicksort na ordenação)
- Lista encadeada (histórico de transações)
- Ponteiros para função (callbacks de eventos)
