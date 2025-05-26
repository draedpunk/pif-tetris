#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "tetris.h"
#include "tetraminos.h"
#include "mapa.h"
#include "pontuacao.h"
#include "ui.h"

// limite maximo de jogadores pro jogo 50
// ranking vai mostrar no max as 5 maiores pontuacoes

void atualizar_pontuacao(int *pontuacao, int linhas, int eh_explosiva) {

    if (eh_explosiva){
        *pontuacao -= 50; 
        if (*pontuacao < 0){
            *pontuacao =0;
        } 
        return; 
    }
    *pontuacao += 25;
    if (linhas > 0) {
        *pontuacao += linhas * 75; 
    }
}

void salvar_pontuacao(const char *nome, int pontuacao) {
    FILE *arquivo = fopen("./assets/ranking.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar pontuação.\n");
        return;
    }

    fprintf(arquivo, "%s | %d\n", nome, pontuacao);
    fclose(arquivo);
}

void exibir_campo_nome(const char nome[]) {
    int campo_largura = 30;
    int inicio_x = (2 + 70 - campo_largura) / 2; 
    int inicio_y = 6;

    screenGotoxy(inicio_x + 7, inicio_y + 5); 
    printf("Informe seu nome");

    screenGotoxy(inicio_x, inicio_y + 6);
    printf("------------------------------");

    screenGotoxy(inicio_x, inicio_y + 7);
    printf("|                            |");

    screenGotoxy(inicio_x, inicio_y + 8);
    printf("------------------------------");

    screenGotoxy(inicio_x, inicio_y + 9);
    printf("Pressione ENTER para continuar");

    screenGotoxy(inicio_x, inicio_y + 7); 
    printf("%s", nome);
}

void input_nome_jogador(char *nome_jogador) {
    screenClear();
    dimensoes_tela_inicio_fim();
    exibir_campo_nome("");

    int campo_largura = 30;
    int inicio_x = (2 + 70 - campo_largura)/2;
    int inicio_y = 6;

    screenGotoxy(inicio_x + 2, inicio_y + 7);

    screenShowCursor();
    fgets(nome_jogador, 30, stdin);

    size_t len = strlen(nome_jogador);
    if (len > 0 && nome_jogador[len - 1] == '\n') {
        nome_jogador[len - 1] = '\0';
    }

    screenHideCursor();
}

void receber_dados_pontuacao(Jogador pontos[], int *qtd_dados) {
    FILE *arquivo_pontos = fopen("./assets/ranking.txt", "r");
    if (arquivo_pontos == NULL) {
        printf("arquivo nao disponivel.\n");
        perror("Error");
        return;
    }

    *qtd_dados = 0;
    while(fscanf(arquivo_pontos, " %25[^|] | %d", 
                pontos[*qtd_dados].nome, 
                &pontos[*qtd_dados].pontuacao) == 2) {
        (*qtd_dados)++;
        
        if ((*qtd_dados) >= LIMITE_JOGADORES) {
            break;
        }
    }
    fclose(arquivo_pontos);
}

void exibir_ranking() {
    //dimensoes_tela_inicio_fim();
    Jogador pontos[LIMITE_JOGADORES];
    int cont_jogadores;
    receber_dados_pontuacao(pontos, &cont_jogadores);
    
    screenSetColor(LIGHTRED, BLACK);
    screenGotoxy(26, 9);
    printf("+-- RANKING --+\n");

    if (cont_jogadores == 0) {
        screenGotoxy(26, 7);
        screenSetColor(LIGHTRED, BLACK);
        printf("Nenhuma pontuacao disponivel =//\n");
    } 
    else {
        colocar_ordem_cresc(pontos, cont_jogadores);
        int maior_pontuacao = (cont_jogadores < TOP_MAIORES_PONT) ? cont_jogadores : TOP_MAIORES_PONT;

        for (int i = 0; i < maior_pontuacao; i++) {
            screenGotoxy(22, 11 + i);
            
            if (i == 0) {
                screenSetColor(LIGHTGREEN, BLACK);
                printf("%d. ★ %s- %d pontos", i+1, pontos[i].nome, pontos[i].pontuacao);
            } 
            else {
                screenSetColor(LIGHTGRAY, BLACK);
                printf("%d. %s- %d pontos", i+1, pontos[i].nome, pontos[i].pontuacao);
            }
        }
    }

    screenGotoxy(10, 13 + (cont_jogadores > 0 ? cont_jogadores : 1));
    //voltar_menu();
}


void colocar_ordem_cresc(Jogador pontos[], int qtd_dados){
    Jogador valor_temporario;

    for (int i =0; i < qtd_dados-1; i++){
        for (int j= i +1; j <qtd_dados; j++){

            if(pontos[i].pontuacao < pontos[j].pontuacao){
                valor_temporario = pontos[i];
                pontos[i] = pontos[j];
                pontos[j] = valor_temporario;
            }
        }
    }
}

void exibir_pontuacao(int *pontuacao){

    screenSetColor(CYAN, BLACK);
    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 0);
    printf("+---Pontos---+");

    screenSetColor(CYAN, BLACK);
    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 1);
    printf("|            |");

    screenSetColor(CYAN, BLACK);
    screenGotoxy(INICIO_X + LARGURA_JOGO + 6, INICIO_Y + 2);
    printf("+------------+");

    screenSetColor(LIGHTRED, BLACK);
    screenGotoxy(INICIO_X + LARGURA_JOGO + 9, INICIO_Y + 1);
    printf("%4d", *pontuacao);
}


