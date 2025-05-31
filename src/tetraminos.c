#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "screen.h"
#include "tetraminos.h"
#include "mapa.h"

Tetramino tetraminos[9];

void carregar_tetraminos() {

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
        .cor = LIGHTMAGENTA,
        .simbolo = '#'
    };

    tetraminos[8] = (Tetramino){ // T EXPLOSIVA
        .forma = "...."
                 ".XXX"
                 "..X."
                 "....", 
        .cor = RED,
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

// void desenhar_tetramino(int tipo, int rotacao, int px, int py) {
//     screenSetColor(tetraminos[tipo].cor, BLACK);
//     for (int y = 0; y < 4; y++) {
//         for (int x = 0; x < 4; x++) {
//             int pi = rotacionar(x, y, rotacao);
//             if (tetraminos[tipo].forma[pi] != '.') {
//                 screenGotoxy(px + x, py + y);
//                 printf("%c", tetraminos[tipo].simbolo);
//             }
//         }
//     }
//     screenSetColor(WHITE, BLACK);
// }

int rotacionar(int x, int y, int rotacao) {
    switch (rotacao % 4) {
        case 0: // 0 graus           //  0  1  2  3
            return y * 4 + x;       //  4  5  6  7
                                    //  8  9 10 11
                                    // 12 13 14 15

        case 1: // 90 graus          // 12  8  4  0
            return 12 + y - (x * 4); // 13  9  5  1
                                     // 14 10  6  2
                                     // 15 11  7  3

        case 2: // 180 graus         // 15 14 13 12
            return 15 - (y * 4) - x; // 11 10  9  8
                                     //  7  6  5  4
                                     //  3  2  1  0

        case 3: // 270 graus         //  3  7 11 15
            return 3 - y + (x * 4);  //  2  6 10 14
                                     //  1  5  9 13
                                     //  0  4  8 12
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
    for (int y = -2; y <= 2; y++) {
        for (int x = -2; x <= 2; x++) {
            int px = cx + x;
            int py = cy + y;

            if (px >= 0 && px < LARGURA_JOGO && py >= 0 && py < ALTURA_JOGO) {
                // verfica se eh parede ou a base
                if (t->matriz[py][px].caracter != PAREDE && t->matriz[py][px].caracter != BASE) {
                    t->matriz[py][px].caracter = ' ';
                    t->matriz[py][px].cor = RED;
                }
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

int sortear_proxima_peca() {
    return rand() % 9;
}


void exibir_prox_peca(int proxima_peca) {
    screenSetColor(BLUE, BLACK);
    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 12);
    printf("+---Proxima--+");

    for (int y = 0; y < 4; y++) {
        screenSetColor(BLUE, BLACK);
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
        screenSetColor(BLUE, BLACK);
        printf("    |");
    }

    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 17);
    screenSetColor(BLUE, BLACK);
    printf("+------------+");
}


void exibir_linhas_removidas(int total_linhas_remov) {
    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 8);
    screenSetColor(MAGENTA, BLACK);
    printf("+---Linhas---+");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 9);
    screenSetColor(MAGENTA, BLACK);
    printf("|            |");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 10);
    screenSetColor(MAGENTA, BLACK);
    printf("+------------+");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 9);
    screenSetColor(LIGHTRED, BLACK);
    printf("%4d", total_linhas_remov);
}