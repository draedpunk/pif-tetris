#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "screen.h"
#include "keyboard.h"
#include "mapa.h"

void alocar_mapa(MAPA* t) {
    t->matriz = malloc(t->linhas * sizeof(Grade_jogo*));

    for (int i = 0; i < t->linhas; i++) {
        t->matriz[i] = malloc(t->colunas * sizeof(Grade_jogo));
        for (int j = 0; j < t->colunas; j++) {
            t->matriz[i][j].caracter = ' ';  
        }
    }
}

void ler_mapa(MAPA *t) {
    FILE *f = fopen("./assets/ascii-arts/mapa.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir o mapa.\n");
        exit(1);
    }

    char linha[TAMANHO_MAX_COLUNAS + 2];
    int linhas = 0;
    int colunas = -1;

    char temp[TAMANHO_MAX_LINHAS][TAMANHO_MAX_COLUNAS];
    while (fgets(linha, sizeof(linha), f) != NULL) {
        linha[strcspn(linha, "\r\n")] = 0; 
        int len = strlen(linha);
        if (colunas == -1) colunas = len;
        if (len != colunas) {
            printf("Erro: mapa com colunas irregulares (linha %d).\n", linhas + 1);
            fclose(f);
            exit(1);
        }
        if (linhas >= TAMANHO_MAX_LINHAS) {
            printf("Erro: mapa excede o número máximo de linhas.\n");
            fclose(f);
            exit(1);
        }
        strcpy(temp[linhas], linha);
        linhas++;
    }
    fclose(f);

    t->linhas = linhas;
    t->colunas = colunas;
    alocar_mapa(t); 

    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            t->matriz[i][j].caracter = temp[i][j];
            t->matriz[i][j].cor = (temp[i][j] == ' ') ? DEFAULT_COLOR : LIGHTGRAY;
        }
    }
}

void desenhar_mapa_com_peca(MAPA* t, int tetramino_atual, int rotacao_atual, int x_atual, int y_atual) {
    Tetramino atual = tetraminos[tetramino_atual];
    
    for (int y = 0; y < t->linhas; y++) {
        for (int x = 0; x < t->colunas; x++) {
            screenGotoxy(INICIO_X + x, INICIO_Y + y);
            
            int rel_x = x - x_atual;
            int rel_y = y - y_atual;
            if (rel_x >= 0 && rel_x < 4 && rel_y >= 0 && rel_y < 4) {
                int pi = rotacionar(rel_x, rel_y, rotacao_atual);
                if (atual.forma[pi] != '.') {
                    screenSetColor(atual.cor, BLACK);
                    printf("%c", atual.simbolo);
                    continue;
                }
            }

            screenSetColor(t->matriz[y][x].cor, BLACK);
            printf("%c", t->matriz[y][x].caracter);
        }
    }
    screenSetColor(WHITE, BLACK); 
}