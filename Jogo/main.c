#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // Para _kbhit() e _getch()
#include <time.h>   // Para srand() e rand()
#include <string.h> // Para strcmp
#ifdef _WIN32
#include <windows.h> // Para Sleep() no Windows
#else
#include <unistd.h>  // Para sleep() no Linux
#endif


#define MAP_SIZE 5
#define PLAYER 'P'
#define EMPTY '.'
#define ENEMY 'E'
#define ITEM 'I'
#define INVENTORY_KEY 'i'
#define EXIT_KEY 'q'

typedef struct {
    char nome[30];
    char classe[30];
    int vida;
    int mana;
    int forca;
    int destreza;
    char inventario[MAP_SIZE * MAP_SIZE];  // Array para armazenar itens coletados
    int num_itens;  // Contador de itens no inventário
} Personagem;

typedef struct {
    int forca;
} Inimigo;


// Função para inicializar o personagem
void criar_personagem(Personagem *p) {
    printf("Digite o nome do personagem: ");
    scanf("%s", p->nome);

    printf("Escolha a classe (Guerreiro/Mago/Arqueiro): ");
    scanf("%s", p->classe);

    if (strcmp(p->classe, "Guerreiro") == 0) {
        p->vida = 100;
        p->mana = 50;
        p->forca = 30;
        p->destreza = 10;
    } else if (strcmp(p->classe, "Mago") == 0) {
        p->vida = 70;
        p->mana = 100;
        p->forca = 10;
        p->destreza = 20;
    } else if (strcmp(p->classe, "Arqueiro") == 0) {
        p->vida = 80;
        p->mana = 60;
        p->forca = 20;
        p->destreza = 30;
    } else {
        printf("Classe inválida! Atribuindo valores padrão.\n");
        p->vida = 80;
        p->mana = 60;
        p->forca = 20;
        p->destreza = 20;
    }
    p->num_itens = 0;
}

// Função para desenhar o mapa
void desenhar_mapa(char mapa[MAP_SIZE][MAP_SIZE], int player_x, int player_y) {
    int i, j;
    system("cls");  // Limpar a tela (no Windows)

    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            if (i == player_x && j == player_y) {
                printf("%c ", PLAYER);
            } else {
                printf("%c ", mapa[i][j]);
            }
        }
        printf("\n");
    }
}

// Função para gerar posições aleatórias no mapa
void gerar_posicoes(char mapa[MAP_SIZE][MAP_SIZE], int num, char tipo) {
    int i, x, y;

    for (i = 0; i < num; i++) {
        do {
            x = rand() % MAP_SIZE;
            y = rand() % MAP_SIZE;
        } while (mapa[x][y] != EMPTY);

        mapa[x][y] = tipo;
    }
}

void adicionar_item(Personagem *p, char item) {
    if (p->num_itens < MAP_SIZE * MAP_SIZE) {
        p->inventario[p->num_itens] = item;
        p->num_itens++;
        int aumento_forca = rand() % 10 + 1; // Poder aleatório de 1 a 10
        p->forca += aumento_forca;
        printf("Você coletou um item! Aumentou sua força em %d.\n", aumento_forca);
        #ifdef _WIN32
        Sleep(2000);  // Pausa por 2000 milissegundos (2 segundos) no Windows
        #else
        sleep(2);     // Pausa por 2 segundos no Linux
        #endif
    } else {
        printf("Inventário cheio!\n");
    }
}

void mostrar_inventario(Personagem *p) {
    int i;
    system("cls");  // Limpar a tela (no Windows)
    printf("=== INVENTÁRIO ===\n");
    printf("Força atual: %d\n", p->forca);
    if (p->num_itens == 0) {
        printf("Nenhum item no inventário.\n");
    } else {
        for (i = 0; i < p->num_itens; i++) {
            printf("Item %d: %c\n", i + 1, p->inventario[i]);
        }
    }
    printf("\nPressione qualquer tecla para voltar ao jogo...\n");
    _getch();  // Espera o usuário pressionar uma tecla
}

void combate(Personagem *jogador, Inimigo *inimigo) {
    printf("Você encontrou um inimigo com força %d! Iniciando o combate...\n", inimigo->forca);

    if (jogador->forca > inimigo->forca) {
        printf("Você venceu o combate! Ganhou a força do inimigo.\n");
        jogador->forca += inimigo->forca;
    } else {
        printf("Você perdeu o combate.\n");
    }
    printf("Pressione qualquer tecla para continuar...\n");
    _getch();  // Espera o usuário pressionar uma tecla
}


void gerar_inimigos(Inimigo inimigos[], int num) {
    int i;
    for (i = 0; i < num; i++) {
        inimigos[i].forca = rand() % 30 + 1;  // Força aleatória de 1 a 30
    }
}

int main() {
    char mapa[MAP_SIZE][MAP_SIZE];
    int player_x = 0, player_y = 0;
    int i, j;
    Personagem jogador;
    Inimigo inimigos[4];

    // Inicializar o mapa com espaços vazios
    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            mapa[i][j] = EMPTY;
        }
    }

    // Criar o personagem
    criar_personagem(&jogador);

    // Inicializar o gerador de números aleatórios
    srand(time(NULL));

    // Gerar inimigos e itens no mapa
    gerar_posicoes(mapa, 4, ENEMY);  // 4 inimigos
    gerar_posicoes(mapa, 4, ITEM);   // 4 itens
    gerar_inimigos(inimigos, 4);     // Inicializar inimigos com força

    desenhar_mapa(mapa, player_x, player_y);

    while (1) {
        if (_kbhit()) {
            char tecla = _getch();

            if (tecla == INVENTORY_KEY) {
                mostrar_inventario(&jogador);
                desenhar_mapa(mapa, player_x, player_y);
            } else if (tecla == EXIT_KEY) {
                return 0;
            } else {
                // Atualizar a posição do jogador
                switch (tecla) {
                    case 'w': // Para cima
                        if (player_x > 0) player_x--;
                        break;
                    case 's': // Para baixo
                        if (player_x < MAP_SIZE - 1) player_x++;
                        break;
                    case 'a': // Para esquerda
                        if (player_y > 0) player_y--;
                        break;
                    case 'd': // Para direita
                        if (player_y < MAP_SIZE - 1) player_y++;
                        break;
                }

                // Verificar se o jogador encontrou um inimigo ou item
                if (mapa[player_x][player_y] == ENEMY) {
                    int inimigo_index = rand() % 4;  // Escolhe um inimigo aleatório
                    combate(&jogador, &inimigos[inimigo_index]);
                    mapa[player_x][player_y] = EMPTY;  // Remove o inimigo após o combate
                } else if (mapa[player_x][player_y] == ITEM) {
                    adicionar_item(&jogador, ITEM);
                    mapa[player_x][player_y] = EMPTY;  // Remove o item após a coleta
                }

                desenhar_mapa(mapa, player_x, player_y);
            }
        }
    }

    return 0;
}
