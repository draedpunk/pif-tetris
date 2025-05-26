#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "timer.h"
#include "keyboard.h"

#include "tetris.h"
#include "ui.h"
#include "musica.h"

void banner_titulo() {
    int largura_util = 68;
    int altura_util = 22;
    int bannerX = (largura_util - 29) / 2 + 2;
    int bannerY = (altura_util - 5) / 2 + 2;

    screenGotoxy(bannerX, bannerY);
    screenSetColor(RED, BLACK);     printf(" ___ ");
    screenSetColor(YELLOW, BLACK);  printf(" ___ ");
    screenSetColor(GREEN, BLACK);   printf(" ___ ");
    screenSetColor(CYAN, BLACK);    printf(" ___ ");
    screenSetColor(BLUE, BLACK);    printf(" _ ");
    screenSetColor(MAGENTA, BLACK); printf(" ___ ");
    screenGotoxy(bannerX, bannerY + 1);
    screenSetColor(RED, BLACK);     printf("|_ _|");
    screenSetColor(YELLOW, BLACK);  printf("| __>");
    screenSetColor(GREEN, BLACK);   printf("|_ _|");
    screenSetColor(CYAN, BLACK);    printf("| . \\");
    screenSetColor(BLUE, BLACK);    printf("| |");
    screenSetColor(MAGENTA, BLACK); printf("/ __>");
    screenGotoxy(bannerX, bannerY + 2);
    screenSetColor(RED, BLACK);     printf(" | | ");
    screenSetColor(YELLOW, BLACK);  printf("| _> ");
    screenSetColor(GREEN, BLACK);   printf(" | | ");
    screenSetColor(CYAN, BLACK);    printf("|   /");
    screenSetColor(BLUE, BLACK);    printf("| |");
    screenSetColor(MAGENTA, BLACK); printf("\\__ \\");
    screenGotoxy(bannerX, bannerY + 3);
    screenSetColor(RED, BLACK);     printf(" |_| ");
    screenSetColor(YELLOW, BLACK);  printf("|___>");
    screenSetColor(GREEN, BLACK);   printf(" |_| ");
    screenSetColor(CYAN, BLACK);    printf("|_\\_\\");
    screenSetColor(BLUE, BLACK);    printf("|_|");
    screenSetColor(MAGENTA, BLACK); printf("<___/");
    screenGotoxy(bannerX, bannerY + 4);
    screenSetColor(WHITE, BLACK);   printf("                             ");


    screenGotoxy(26, 16);
    screenSetColor(YELLOW, BLACK);
    printf("Selecione uma opcao: ");
    screenSetColor(LIGHTBLUE, BLACK);
    screenGotoxy(26, 17);
    printf("[1] INICIAR JOGO");
    screenGotoxy(26, 18);
    printf("[2] VER RANKING");
    screenGotoxy(26, 19);
    printf("[3] SAIR");

}

void banner_gameover() {
    int largura_util = 68;
    int altura_util = 22;
    int bannerX = (largura_util - 43) / 2 + 2;
    int bannerY = (altura_util - 4) / 2 + 2;

    screenGotoxy(bannerX, bannerY);
    screenSetColor(LIGHTRED, BLACK);
    screenGotoxy(bannerX, bannerY);
    printf(" ___   ___  __ __  ___   ___  _ _  ___  ___ ");
    screenGotoxy(bannerX, bannerY + 1);
    printf("/  _> | . ||  \\  \\| __> | . || | || __>| . \\");
    screenGotoxy(bannerX, bannerY + 2);
    printf("| <_/\\|   ||     || _>  | | || ' || _> |   /");
    screenGotoxy(bannerX, bannerY + 3);
    printf("\\____/|_|_||_|_|_||___> \\___/|__/ |___>|_\\_\\");
    screenSetColor(WHITE, BLACK);

    screenGotoxy(10, 16);
    voltar_menu();
}

void exibir_banner_titulo() {
    screenInit(1);
    dimensoes_tela_inicio_fim();
    banner_titulo();
    screenUpdate();
    sleep(3);
    screenClear();
}

void exibir_banner_gameover() {
    screenInit(1);
    dimensoes_tela_inicio_fim();
    banner_gameover();
    screenUpdate();
    sleep(3);
    screenClear();
}

void bordas_personalizadas(int min_x, int max_x, int min_y,int max_y) {
    char hbc = BOX_HLINE;
    char vbc = BOX_VLINE;
    
    screenClear();
    screenBoxEnable();
    
    screenGotoxy(min_x, min_y);
    printf("%c", BOX_UPLEFT);

    for (int i=min_x+1; i<max_x; i++)
    {
        screenGotoxy(i, min_y);
        printf("%c", hbc);
    }
    screenGotoxy(max_x, min_y);
    printf("%c", BOX_UPRIGHT);

    for (int i=min_y+1; i<max_y; i++)
    {
        screenGotoxy(min_x, i);
        printf("%c", vbc);
        screenGotoxy(max_x, i);
        printf("%c", vbc);
    }

    screenGotoxy(min_x, max_x);
    printf("%c", BOX_DWNLEFT);
    for (int i=min_y+1; i<max_x; i++)
    {
        screenGotoxy(i, max_y);
        printf("%c", hbc);
    }
    screenGotoxy(max_x, max_y);
    printf("%c", BOX_DWNRIGHT);

    screenBoxDisable();
    
}

void dimensoes_tela_inicio_fim() {
    //bordas_personalizadas(int min_x, int max_x, int min_y,int max_y)
    screenSetColor(CYAN, BLACK);
    bordas_personalizadas(2, 70,2, 24);
}

void dimensoes_tela_jogo() {
    screenSetColor(CYAN, BLACK);
    bordas_personalizadas(2, 36, 2, 24);
}

void voltar_menu() {
    screenSetColor(YELLOW, BLACK);
    printf("Pressione 0 ou ENTER para voltar ao menu principal.\n");
    screenUpdate();

    int tecla;
    do {
        tecla = readch();
        //  ENTER (13)
    } while (tecla != '0' && tecla != 13);
}



// typedef enum {BLACK, RED, GREEN, BROWN, BLUE, MAGENTA, CYAN, LIGHTGRAY,
//         DARKGRAY, LIGHTRED, LIGHTGREEN, YELLOW, LIGHTBLUE, 
//         LIGHTMAGENTA, LIGHTCYAN, WHITE} screenColor; 





