#ifndef MUSICS_H
#define MUSICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

Mix_Music* musica_menu_tetris();
Mix_Music* som_gameover();
Mix_Music* som_peca_fixada();
Mix_Music* som_explosao();
Mix_Music* som_linha_remov();
Mix_Music* som_novo_nivel();
void inicializar_audio();

#endif