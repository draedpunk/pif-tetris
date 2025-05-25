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

void banner_titulo() {
    int largura_util = 68;
    int altura_util = 22;
    int bannerX = (largura_util - 29) / 2 + 2;
    int bannerY = (altura_util - 5) / 2 + 2;

    screenGotoxy(bannerX, bannerY);
    printf(" ___  ___  ___  ___  _  ___ ");
    screenGotoxy(bannerX, bannerY + 1);
    printf("|_ _|| __>|_ _|| . \\| |/ __>");
    screenGotoxy(bannerX, bannerY + 2);
    printf(" | | | _>  | | |   /| |\\__ \\");
    screenGotoxy(bannerX, bannerY + 3);
    printf(" |_| |___> |_| |_\\_\\|_|<___/");
    screenGotoxy(bannerX, bannerY + 4);
    printf("                             ");

    screenGotoxy(26, 16);
    printf("Selecione uma opcao: ");
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
    printf(" ___   ___  __ __  ___   ___  _ _  ___  ___ ");
    screenGotoxy(bannerX, bannerY + 1);
    printf("/  _> | . ||  \\  \\| __> | . || | || __>| . \\");
    screenGotoxy(bannerX, bannerY + 2);
    printf("| <_/\\|   ||     || _>  | | || ' || _> |   /");
    screenGotoxy(bannerX, bannerY + 3);
    printf("\\____/|_|_||_|_|_||___> \\___/|__/ |___>|_\\_\\");
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

    bordas_personalizadas(2, 70,2, 24);
}

void dimensoes_tela_jogo() {
    bordas_personalizadas(2, 36, 2, 24);
}





