#ifndef MUSICA_H
#define MUSICA_H

#include <SDL2/SDL_mixer.h>

typedef struct {
    Mix_Music* musica_tetris;
    Mix_Music* musica_gameover;
    Mix_Chunk* som_linha;
    Mix_Chunk* som_nivel;
    Mix_Chunk* som_peca;
    Mix_Chunk* som_explosao;
    int audio_ativado;  
} Musica;

Musica* carregar_sons();
void liberar_sons(Musica *audio);
void inicializar_audio(Musica *audio); 
Musica* inicializar_tudo_audio(int *audio_ok);

#endif