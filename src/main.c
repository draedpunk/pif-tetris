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
#include "musica.h"

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

void inicializar_jogo(MAPA *t) {
    ler_mapa(t);
    srand(time(NULL));
    carregar_tetraminos();
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
    inicializar_audio();
    char nome[30];
    MAPA t;
    int opcao;

    while (1) {
        Mix_Music *musica_menu = musica_menu_tetris();
        if (musica_menu != NULL) {
            Mix_PlayMusic(musica_menu, -1);  // Toca no menu
        }

        dimensoes_tela_inicio_fim();
        banner_titulo(); // mostra o menu
        screenHideCursor();
        opcao = getchar();
        while (getchar() != '\n');

        switch (opcao) {
            case '1': {
                Mix_HaltMusic();  // Para a música do menu
                Mix_FreeMusic(musica_menu);

                Mix_Music *musica_jogo = Mix_LoadMUS("./assets/musicas/tetris_song.mp3");
                if (musica_jogo != NULL) {
                    Mix_PlayMusic(musica_jogo, -1);  // Música do jogo
                }

                input_nome_jogador(nome);
                inicializar_jogo(&t);

                int pontuacao = 0, fim_jogo = 0, velocidade = 1000;
                int teclas[4] = {0}, bRotateHold = 1;
                int tipo = rand() % 9, rot = 0;
                int x = LARGURA_JOGO / 2 - 2, y = 0;
                int acumulador_linhas = 0;
                int nivel_atual = 1;

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

                        Mix_Chunk *som_peca = som_peca_fixada();
                        if (som_peca) Mix_PlayChannel(-1, som_peca, 0);

                        if (tipo == 8) {
                            Mix_Chunk *som_explo = som_explosao();
                            if (som_explo) Mix_PlayChannel(-1, som_explo, 0);

                            screenSetColor(RED, BLACK);
                            explodir(&t, x + 1, y + 1);
                            screenSetColor(WHITE, BLACK);
                            screenUpdate();
                            usleep(100000); 
                        }

                        int linhas = remover_linhas_completas(&t);
                        if (linhas > 0) {
                            Mix_Chunk *som_linha = som_linha_remov();
                            if (som_linha) Mix_PlayChannel(-1, som_linha, 0);
                        }

                        acumulador_linhas += linhas;
                        exibir_linhas_removidas(acumulador_linhas);

                        int novo_nivel = subir_nivel(nivel_atual, acumulador_linhas, &velocidade);
                        if (novo_nivel > nivel_atual) {
                            Mix_Chunk *som_nivel = som_novo_nivel();
                            if (som_nivel) Mix_PlayChannel(-1, som_nivel, 0);
                        }
                        nivel_atual = novo_nivel;

                        atualizar_pontuacao(&pontuacao, linhas, (tipo == 8));
                        exibir_pontuacao(&pontuacao);

                        tipo = rand() % 9;
                        rot = 0;
                        x = LARGURA_JOGO / 2 - 2;
                        y = 0;

                        if (verificar_game_over(&t, tipo, rot, x, y)) {
                            fim_jogo = 1;

                            Mix_HaltMusic();
                            Mix_Music *musica_gameover = som_gameover();
                            if (musica_gameover) Mix_PlayMusic(musica_gameover, 1);

                            screenGotoxy(INICIO_X, INICIO_Y + t.linhas / 2);
                            exibir_banner_gameover();
                        }
                    }

                    desenhar_mapa_com_peca(&t, tipo, rot, x, y);
                    exibir_prox_peca(tipo);
                    screenUpdate();
                    usleep(50000);
                }

                salvar_pontuacao(nome, pontuacao);
                screenDestroy();

                for (int i = 0; i < t.linhas; i++) {
                    free(t.matriz[i]);
                }
                free(t.matriz);

                Mix_HaltMusic();
                Mix_FreeMusic(musica_jogo);
                break;
            }

            case '2':
                dimensoes_tela_inicio_fim();
                exibir_ranking();
                break;

            case '3':
                screenClear();
                return 0;

            default:
                screenClear();
                screenGotoxy(25, 18);
                printf("Opcao invalida.\n");
                sleep(1);
        }
    }
}
