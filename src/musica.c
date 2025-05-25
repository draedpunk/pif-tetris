#include "musica.h"
#include <stdio.h>

Musica* carregar_sons() {
    Musica *audio = malloc(sizeof(Musica));
    if (!audio) {
        return NULL;
    }
    memset(audio, 0, sizeof(Musica));

    audio->musica_tetris= Mix_LoadMUS("assets/musicas/tetris_song.mp3");
    audio->musica_gameover = Mix_LoadMUS("assets/musicas/game_over_tetris_ver.mp3");
    audio->som_linha = Mix_LoadWAV("assets/musicas/clear.wav");
    audio->som_nivel = Mix_LoadWAV("assets/musicas/levelup.wav");
    audio->som_peca = Mix_LoadWAV("assets/musicas/drop.wav");
    audio->som_explosao = Mix_LoadWAV("assets/musicas/explosion.wav");

    if (!audio->musica_tetris || !audio->musica_gameover ||
        !audio->som_linha || !audio->som_nivel || !audio->som_peca || !audio->som_explosao) {
        fprintf(stderr, "Erro ao carregar audio: %s\n", Mix_GetError());
        liberar_sons(audio);
        return NULL;
    }

    return audio;
}

void liberar_sons(Musica *audio) {
    if (!audio) return;
    
    Mix_FreeMusic(audio->musica_tetris);
    Mix_FreeMusic(audio->musica_gameover);
    Mix_FreeChunk(audio->som_linha);
    Mix_FreeChunk(audio->som_nivel);
    Mix_FreeChunk(audio->som_peca);
    Mix_FreeChunk(audio->som_explosao);
    
    free(audio);
}

void inicializar_audio(){
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Nao foi possível inicializar o SDL: %s\n", SDL_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4100) < 0) {
        fprintf(stderr, "Erro ao inicializar SDL_mixer: %s\n", Mix_GetError());
    }
}