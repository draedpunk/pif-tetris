#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include "musica.h"
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

void processar_input(int *teclas, int *x, int *y, int *rot, int *pode_rodar_hold, MAPA *t, int peca_atual) {
    if (teclas[0] && pode_encaixar(t, peca_atual, *rot, *x + 1, *y)) (*x)++;
    if (teclas[1] && pode_encaixar(t, peca_atual, *rot, *x - 1, *y)) (*x)--;
    if (teclas[2] && pode_encaixar(t, peca_atual, *rot, *x, *y + 1)) {
        (*y)++;
        timerUpdateTimer(500);
    }
    if (teclas[3]) {
        if (*pode_rodar_hold && pode_encaixar(t, peca_atual, *rot + 1, *x, *y))
            (*rot)++;
        *pode_rodar_hold = 0;
    } else {
        *pode_rodar_hold = 1;
    }
}

int ler_opcao_menu() {
    int c;
    do {
        c = getchar();
    } while (c == '\n');
    
    while (getchar() != '\n');

    return c;
}

int verificar_game_over(MAPA *t, int peca_atual, int rot, int x, int y) {
    return !pode_encaixar(t, peca_atual, rot, x, y);
}

void inicializar_jogo(MAPA* t) {
    ler_mapa(t);
    srand(time(NULL));
    carregar_tetraminos();
    dimensoes_tela_jogo();
    timerInit(500);
}

int subir_nivel(int *nivel_atual, int acumulador_linhas, int *velocidade) {
    int novo_nivel = acumulador_linhas / 5 + 1;

    if (novo_nivel > *nivel_atual) {
        *velocidade = (*velocidade > 100) ? *velocidade - 100 : 100;
        *nivel_atual = novo_nivel;
        return 1; // subiu de nivel!
    }

    return 0; // nivel nao mudou
}

void exibir_nivel(int nivel_atual){
    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 4);
    screenSetColor(YELLOW, BLACK);
    printf("+---Nivel----+");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 5);
    screenSetColor(YELLOW, BLACK);
    printf("|            |");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 6);
    screenSetColor(YELLOW, BLACK);
    printf("+------------+");

    screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 5);
    screenSetColor(LIGHTRED, BLACK);
    printf("%4d", nivel_atual);
}

int iniciar_jogo(char* nome, MAPA *t, Musica* audio, int audio_ok){
    input_nome_jogador(nome);
    inicializar_jogo(t);

    // inicializacao das variaveis essenciais
    int pontuacao = 0, fim_jogo = 0, velocidade = 1000;
    int teclas[4] = {0}, pode_rodar_hold = 1;
    int rot = 0;
    int x = LARGURA_JOGO/2-2, y = 0;
    int acumulador_linhas = 0;
    int nivel_atual = 1;

    int peca_proxima = sortear_proxima_peca();
    int peca_atual = peca_proxima;
    peca_proxima = sortear_proxima_peca();

    exibir_pontuacao(&pontuacao);
    exibir_linhas_removidas(acumulador_linhas);
    exibir_prox_peca(peca_proxima);
    exibir_nivel(nivel_atual);

    while (!fim_jogo) { // enquanto o jogo tiver rodando...
        int cair = timerTimeOver();
        ler_input(teclas);
        processar_input(teclas, &x, &y, &rot, &pode_rodar_hold, t, peca_atual);

        if (cair && pode_encaixar(t, peca_atual, rot, x, y + 1)) {
            y++;
            timerUpdateTimer(velocidade);
        } else if (!pode_encaixar(t, peca_atual, rot, x, y + 1)) { // caso nao possa encaixar no mapa, prende o tetramino
            posicionar_tetramino_no_mapa(t, peca_atual, rot, x, y);
            // chamada do som da peça fixada
            if (audio_ok && audio->som_peca) {
                Mix_VolumeChunk(audio->som_peca, 40);  // Lower volume for less distortion
                Mix_PlayChannel(-1, audio->som_peca, 0);
            }

            if (peca_atual == 8) {  // peca explosiva numero 8 tetraminos[8], ai toca a musica
                if (audio_ok && audio->som_explosao) {
                    Mix_VolumeChunk(audio->som_explosao, 64);
                    Mix_PlayChannel(-1, audio->som_explosao, 0);
                }

                screenSetColor(RED, BLACK);
                explodir(t, x + 1, y + 1);
                screenUpdate();
                usleep(100000); // tentar ajudar na sincronizacao do som qd a peca explode
            }

            int linhas = remover_linhas_completas(t); // conta qts linhas foram removidas
            if (linhas > 0 && audio_ok && audio->som_linha) {
                Mix_VolumeChunk(audio->som_linha, 50);
                Mix_PlayChannel(-1, audio->som_linha, 0);
            }
            // acumula as linhas, mostra a qtd e depois verifica se atigiu a qtd minima (5) pra passar de nivel
            acumulador_linhas += linhas;
            exibir_linhas_removidas(acumulador_linhas);

            if (subir_nivel(&nivel_atual, acumulador_linhas, &velocidade)) {
                if (audio_ok && audio->som_nivel) { // linhas removi >= 5 : level up!
                    Mix_VolumeChunk(audio->som_nivel, 50);
                    Mix_PlayChannel(-1, audio->som_nivel, 0);
                }
                exibir_nivel(nivel_atual);
            }
            // att a pontuacao com base nas linhas eliminadas e nos pontos comidos pela peca explosiva
            atualizar_pontuacao(&pontuacao, linhas, (peca_atual == 8));
            exibir_pontuacao(&pontuacao); // mostra os pontos da caixinha

            // configuracoes da prox peca a ser gerada e colocada no "centro" do tabuleiro
            peca_atual = peca_proxima;
            peca_proxima = sortear_proxima_peca();
            rot = 0;
            x = LARGURA_JOGO/2-2;
            y = 0;

            if (verificar_game_over(t, peca_atual, rot, x, y)) {
                fim_jogo = 1; // é game over, ent toca a musica e mostra o banner
                if (audio_ok && audio->musica_gameover) {
                    Mix_VolumeMusic(64);
                    Mix_PlayMusic(audio->musica_gameover, 1);
                }

                screenGotoxy(INICIO_X, INICIO_Y + t->linhas / 2);
                exibir_banner_gameover();
                            
            }
        }
        // atualizam oq é mostrado na tela enquanto o jogo ainda roda
        desenhar_mapa_com_peca(t, peca_atual, rot, x, y);
        exibir_prox_peca(peca_proxima);
        screenUpdate();
        usleep(5000);
    }
    // perdeu!! salva a pontuacao do jogador beltrano e mata a tela
    salvar_pontuacao(nome, pontuacao);

    for (int i = 0; i < t->linhas; i++) { // libera a matriz
        free(t->matriz[i]);
    }
    free(t->matriz);

    // chamada da musica tetris de novo apos o game over
    if (audio_ok && audio->musica_tetris != NULL) {
        Mix_VolumeMusic(64);
        Mix_PlayMusic(audio->musica_tetris, -1);
    }

    return 0;

}

int main() {
    // inicializa teclado no modo raw (pra capturar tecla sem esperar ENTER)
    keyboardInit();

    int audio_ok;
    Musica *audio = inicializar_tudo_audio(&audio_ok);

    char nome[30];
    MAPA t;
    int opcao;
    int rodando = 1;

    if (audio_ok && audio->musica_tetris != NULL) {
        Mix_PlayMusic(audio->musica_tetris, -1); 
    }


    while (rodando) {
        // inicializa tela, configura dimensões e limpa antes do menu
        screenInit(1);
        screenClear();
        dimensoes_tela_inicio_fim();
        exibir_banner_titulo();

        // volta pro modo canônico pra digitar menu com ENTER
        keyboardDestroy();
        screenHideCursor();

        opcao = ler_opcao_menu();

        switch (opcao) {
            case '1': // iniciar jogo
                // volta pro modo raw pra jogo captar tecla direto
                keyboardInit();
                screenClear();
                //dimensoes_tela_inicio_fim();
                iniciar_jogo(nome, &t, audio, audio_ok);
                // depois do jogo, loop volta pro menu, então no começo do loop vai chamar keyboardDestroy() de novo
                break;

            case '2': // ranking
                screenClear();
                dimensoes_tela_inicio_fim();
                exibir_ranking();
                screenGotoxy(14, 19);
                voltar_menu();
                break;

            case '3': // sair
                screenClear();
                liberar_sons(audio);
                rodando = 0;
                break;

            default:
                screenClear();
                screenGotoxy(25, 18);
                printf("Opcao invalida.\n");
                sleep(1);
        }
    }

    return 0;
}
