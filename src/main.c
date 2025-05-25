#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include "tetris.h"
#include "ui.h"
#include "tetraminos.h"
#include "mapa.h"
#include "pontuacao.h"

void ler_input(int tecla[4]) {
    for (int i = 0; i < 4; i++) tecla[i] = 0;

    if (keyhit()) {
        char t = readch();
        switch (t) {
            case 'd': case 'D': tecla[0] = 1; break; // direita
            case 'a': case 'A': tecla[1] = 1; break; // esquerda
            case 's': case 'S': tecla[2] = 1; break; // baixo
            case 'w': case 'W': tecla[3] = 1; break; // rotacionar
        }
    }
}

void processar_input(int *teclas, int *x, int *y, int *rot, int *bRotateHold, MAPA *t, int tipo) {
    if (teclas[0] && pode_encaixar(t, tipo, *rot, *x + 1, *y)) (*x)++;
    if (teclas[1] && pode_encaixar(t, tipo, *rot, *x - 1, *y)) (*x)--;
    if (teclas[2] && pode_encaixar(t, tipo, *rot, *x, *y + 1)) {
        (*y)++;
        timerUpdateTimer(500);
    }
    if (teclas[3]) {
        if (*bRotateHold && pode_encaixar(t, tipo, *rot + 1, *x, *y))
            (*rot)++;
        *bRotateHold = 0;
    } else {
        *bRotateHold = 1;
    }
}

int verificar_game_over(MAPA *t, int tipo, int rot, int x, int y) {
    return !pode_encaixar(t, tipo, rot, x, y);
}

void inicializar_jogo(MAPA *t){
    ler_mapa(t); 
    srand(time(NULL));
    carregar_tetraminos();

    exibir_banner_titulo();
    screenInit(1);
    dimensoes_tela_jogo();
    
    timerInit(500);
}

int subir_nivel(int nivel_atual, int acumulador_linhas, int *velocidade) {
    int novo_nivel = acumulador_linhas/ 5 + 1;

    if (novo_nivel > nivel_atual) {
        *velocidade = (*velocidade > 100) ? *velocidade - 100 : 100;
    }

    return novo_nivel;
}


void exibir_nivel(int nivel_atual){
    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 4);
    printf("+---Nivel----+");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 5);
    printf("|            |");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 6);
    printf("+------------+");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 5);
    printf("%4d", nivel_atual);
}


int main() {
    dimensoes_tela_inicio_fim();
    char nome_jogadr[30];
    input_nome_jogador(nome_jogadr);


    MAPA t;
    inicializar_jogo(&t);

    int pontuacao = 0, fim_jogo = 0, velocidade = 1000;
    int teclas[4] = {0}, bRotateHold = 1;
    int tipo = rand() % 9, rot = 0;
    int x = LARGURA_JOGO / 2 - 2, y = 0;
    int acumulador_linhas =0;
    int nivel_atual =1;

    exibir_pontuacao(&pontuacao);
    exibir_linhas_removidas(acumulador_linhas);
    exibir_prox_peca(tipo);
    exibir_nivel(nivel_atual);


    while (!fim_jogo) {

        int cair = timerTimeOver();
        ler_input(teclas);
        processar_input(teclas, &x, &y, &rot, &bRotateHold, &t, tipo);

        if (cair && pode_encaixar(&t, tipo, rot, x, y + 1)) {
            y++;
            timerUpdateTimer(velocidade);

        } else if (!pode_encaixar(&t, tipo, rot, x, y + 1)) {
            posicionar_tetramino_no_mapa(&t, tipo, rot, x, y);

            if (tipo == 8) {
                explodir(&t, x + 1, y + 1);
            }

            int linhas = remover_linhas_completas(&t);
            acumulador_linhas += linhas;
            exibir_linhas_removidas(acumulador_linhas);

            nivel_atual = subir_nivel(nivel_atual, acumulador_linhas, &velocidade);
            exibir_nivel(nivel_atual);


            atualizar_pontuacao(&pontuacao, linhas, (tipo == 8));
            exibir_pontuacao(&pontuacao);

            tipo = rand() % 9;
            rot = 0;
            x = LARGURA_JOGO / 2 - 2;
            y = 0;

            if (verificar_game_over(&t, tipo, rot, x, y)) {
                fim_jogo = 1;
                screenGotoxy(INICIO_X, INICIO_Y + t.linhas / 2);
                exibir_banner_gameover();
            }
        }

        desenhar_mapa_com_peca(&t, tipo, rot, x, y);

        exibir_prox_peca(tipo);
        screenUpdate();
        usleep(50000);
    }
    salvar_pontuacao(nome_jogadr, pontuacao);

    screenDestroy();
    for (int i = 0; i < t.linhas; i++) {
        free(t.matriz[i]);
    }
    free(t.matriz);
    return 0;
}