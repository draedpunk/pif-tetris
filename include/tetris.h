#ifndef _TETRIS_H_
#define _TETRIS_H_

#include "mapa.h"
#include "tetraminos.h"

void ler_input(int tecla[4]);
void processar_input(int *teclas, int *x, int *y, int *rot, int *bRotateHold, MAPA *t, int tipo);
int verificar_game_over(MAPA *t, int tipo, int rot, int x, int y);
void inicializar_jogo(MAPA *t);

#endif