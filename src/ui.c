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

void banner_titulo(){
    int bannerX = (80 - 53) / 2;
    int bannerY = (24 - 6) / 2;

    screenGotoxy(bannerX, bannerY);
    printf(" _____ ______  _____  _____ ______  _____  _____ ");
    screenGotoxy(bannerX, bannerY + 1);
    printf("|_   _|| ___ \\|  ___||_   _|| ___ \\|_   _|/  ___|");
    screenGotoxy(bannerX, bannerY + 2);
    printf("  | |  | |_/ /| |__    | |  | |_/ /  | |  \\ `--. ");
    screenGotoxy(bannerX, bannerY + 3);
    printf("  | |  |    / |  __|   | |  |    /   | |   `--. \\");
    screenGotoxy(bannerX, bannerY + 4);
    printf("  | |  | |\\ \\ | |___   | |  | |\\ \\  _| |_ /\\__/ /");
    screenGotoxy(bannerX, bannerY + 5);
    printf("  \\_/  \\_| \\_|\\____/   \\_/  \\_| \\_| \\___/ \\____/ ");
}

void banner_gameover() {
    int bannerX = (80 - 59) / 2;
    int bannerY = (24 - 6) / 2;

    screenGotoxy(bannerX, bannerY);
    printf(" _____   ___  ___  ___ _____   _____  _   _  _____ ______ ");
    screenGotoxy(bannerX, bannerY + 1);
    printf("|  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | ||  ___|| ___ \\");
    screenGotoxy(bannerX, bannerY + 2);
    printf("| |  \\// /_\\ \\| .  . || |__   | | | || | | || |__  | |_/ /");
    screenGotoxy(bannerX, bannerY + 3);
    printf("| | __ |  _  || |\\/| ||  __|  | | | || | | ||  __| |    / ");
    screenGotoxy(bannerX, bannerY + 4);
    printf("| |_\\ \\| | | || |  | || |___  \\ \\_/ /\\ \\_/ /| |___ | |\\ \\ ");
    screenGotoxy(bannerX, bannerY + 5);
    printf(" \\____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/ \\____/ \\_| \\_|");
}

void exibir_banner_titulo() {
    screenInit(1);
    dimensoes_tela_inicio_fim();
    //centralizar_area_terminal(65,10);
    banner_titulo();
    screenUpdate();
    sleep(3);
    screenClear();
}

void exibir_banner_gameover() {
    screenInit(1);
    dimensoes_tela_inicio_fim();
    // centralizar_area_terminal(65,10);
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

    bordas_personalizadas(1, 80,1, 25);
}

void dimensoes_tela_jogo() {
    bordas_personalizadas(2, 36, 2, 24);
}





