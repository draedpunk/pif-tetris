#ifndef MUSICS_H
#define MUSICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef struct {
    Mix_Music *musica_tetris; // menu, no jogo em si e no ranking
    Mix_Music *musica_gameover;
    Mix_Chunk *som_linha; // linha completa
    Mix_Chunk *som_nivel;
    Mix_Chunk *som_peca; // som da peça fixa 
    Mix_Chunk *som_explosao;
} Musica;

// musica de fundo//mp3
// Mix_Music* musica_menu_tetris();
// Mix_Music* som_gameover();

// // efeitos sonoros
// Mix_Chunk* som_peca_fixada();
// Mix_Chunk* som_explosao();
// Mix_Chunk* som_linha_remov();
// Mix_Chunk* som_novo_nivel();

// void inicializar_audio();

Musica* carregar_sons();
void liberar_sons(Musica *audio);
void inicializar_audio();

#endif
