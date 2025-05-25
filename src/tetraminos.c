#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include "tetraminos.h"
#include "tetris.h"
#include "mapa.h"

Tetramino tetraminos[9];

#include "tetraminos.h"
#include "screen.h"

Tetramino tetraminos[9];

// char letras_aleatorias_tetraminos() {
    
//     return 'A' + (rand() % 26);
// }

void carregar_tetraminos() {
    // static int sorteou = 0;
    // if (!sorteou) {
    //     srand(time(NULL));
    //     sorteou = 1;
    // }

    tetraminos[0] = (Tetramino){ // I
        .forma = "...."
                 "XXXX"
                 "...."
                 "....",
        .cor = CYAN,
        .simbolo = '#'
    };

    tetraminos[1] = (Tetramino){ // L ESPELHADA
        .forma = "..X."
                 "..X."
                 ".XX."
                 "....",
        .cor = BLUE,
        .simbolo = '#'
    };

    tetraminos[2] = (Tetramino){ // L
        .forma = ".X.."
                 ".X.."
                 ".XX."
                 "....",
        .cor = BLUE,
        .simbolo = '#'
    };

    tetraminos[3] = (Tetramino){ // O OU S
        .forma = "...."
                 ".XX."
                 ".XX."
                 "....",
        .cor = YELLOW,
        .simbolo = '#'
    };

    tetraminos[4] = (Tetramino){ //Z ESPELAHDO
        .forma = "...."
                 "..XX"
                 ".XX."
                 "....",
        .cor = GREEN,
        .simbolo = '#'
    };

    tetraminos[5] = (Tetramino){ // Z
        .forma = "...."
                 ".XX."
                 "..XX"
                 "....",
        .cor = GREEN,
        .simbolo = '#'
    };

    tetraminos[6] = (Tetramino){ // T INVERTIDO
        .forma = "...."
                 "..X."
                 ".XXX"
                 "....",
        .cor = MAGENTA,
        .simbolo = '#'
    };

    tetraminos[7] = (Tetramino){ // T NORMAL
        .forma = "...."
                 ".XXX"
                 "..X."
                 "....",
        .cor = RED,
        .simbolo = '#'
    };

    tetraminos[8] = (Tetramino){ // T EXPLOSIVA
        .forma = "...."
                 ".XXX"
                 "..X."
                 "....", 
        .cor = WHITE,
        .simbolo = '@'
    };
}

void posicionar_tetramino_no_mapa(MAPA *t, int tipo, int rot, int px, int py) {
    Tetramino tet = tetraminos[tipo];
    
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int pi = rotacionar(x, y, rot);
            int mx = px + x;
            int my = py + y;

            if (tet.forma[pi] != '.' && mx >= 0 && mx < t->colunas && my >= 0 && my < t->linhas) {
                t->matriz[my][mx].caracter = tet.simbolo;
                t->matriz[my][mx].cor = tet.cor;
            }
        }
    }
}

void desenhar_tetramino(int tipo, int rotacao, int px, int py) {
    screenSetColor(tetraminos[tipo].cor, BLACK);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int pi = rotacionar(x, y, rotacao);
            if (tetraminos[tipo].forma[pi] != '.') {
                screenGotoxy(px + x, py + y);
                printf("%c", tetraminos[tipo].simbolo);
            }
        }
    }
    screenSetColor(WHITE, BLACK);
}

int rotacionar(int x, int y, int rotacao) {
    switch (rotacao % 4) {
        case 0: return y * 4 + x;         // 0 graus
        case 1: return 12 + y - (x * 4);  // 90 graus
        case 2: return 15 - (y * 4) - x;  // 180 graus
        case 3: return 3 - y + (x * 4);   // 270 graus
    }
    return 0;
}

int pode_encaixar(MAPA* mapa, int tipo, int rot, int posX, int posY) {
    Tetramino t = tetraminos[tipo];
    
    for (int px = 0; px < 4; px++) {
        for (int py = 0; py < 4; py++) {
            int pi = rotacionar(px, py, rot);
            int gx = posX + px;
            int gy = posY + py;

            if (t.forma[pi] != '.') {
                if (gx < 0 || gx >= mapa->colunas || gy < 0 || gy >= mapa->linhas) {
                    return 0;
                }

                if (mapa->matriz[gy][gx].caracter != ' ') {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void explodir(MAPA* t, int cx, int cy) {
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            int px = cx + x;
            int py = cy + y;
            if (px >= 0 && px < LARGURA_JOGO && py >= 0 && py < ALTURA_JOGO) {
                t->matriz[py][px].caracter = ' ';
                t->matriz[py][px].cor = WHITE;
            }
        }
    }

}

int remover_linhas_completas(MAPA *t) {
    int linhas_removidas = 0;

    for (int y = t->linhas - 2; y >= 0; y--) {
        int cheia = 1;
        for (int x = 1; x < t->colunas - 1; x++) {
            if (t->matriz[y][x].caracter == ' ') { 
                cheia = 0;
                break;
            }
        }

        if (cheia) {
            for (int yy = y; yy > 0; yy--) {
                for (int x = 1; x < t->colunas - 1; x++) {
                    t->matriz[yy][x] = t->matriz[yy - 1][x];
                }
            }

            for (int x = 1; x < t->colunas - 1; x++) {
                t->matriz[0][x].caracter = ' '; 
                t->matriz[0][x].cor = DEFAULT_COLOR;
            }

            linhas_removidas++;
            y++;
        }
    }

    return linhas_removidas;
}

void exibir_prox_peca(int proxima_peca) {
    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 12);
    printf("+---Proxima--+");

    for (int y = 0; y < 4; y++) {
        screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 13 + y);
        printf("|    "); 

        for (int x = 0; x < 4; x++) {
            int pi = rotacionar(x, y, 0);
            if (tetraminos[proxima_peca].forma[pi] != '.') {
                screenSetColor(tetraminos[proxima_peca].cor, BLACK);
                printf("%c", tetraminos[proxima_peca].simbolo);
                screenSetColor(WHITE, BLACK);
            } else {
                printf(" ");
            }
        }

        printf("    |"); 
    }

    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 17);
    printf("+------------+");
}

void exibir_linhas_removidas(int total_linhas_remov) {
    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 8);
    printf("+---Linhas---+");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 9);
    printf("|            |");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 10);
    printf("+------------+");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 9);
    printf("%4d", total_linhas_remov);
}