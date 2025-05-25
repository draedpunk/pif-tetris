#ifndef _TETRAMINOS_H_
#define _TETRAMINOS_H_

#include "screen.h"
#include "tetris.h"
#include "mapa.h"

typedef struct {
    char* forma;          
    screenColor cor;       
    char simbolo;          
} Tetramino;

extern Tetramino tetraminos[9];

//char letras_aleatorias_tetraminos();
void carregar_tetraminos();
void posicionar_tetramino_no_mapa(MAPA *t, int tipo, int rot, int px, int py);
void desenhar_tetramino(int tipo, int rotacao, int px, int py);
int rotacionar(int x, int y, int rotacao);
int pode_encaixar(MAPA* mapa, int tipo, int rot, int posX, int posY);
void explodir(MAPA* t, int cx, int cy);
int remover_linhas_completas(MAPA *t);
void exibir_prox_peca(int proxima_peca);
void exibir_linhas_removidas(int total_linhas_remov);

#endif