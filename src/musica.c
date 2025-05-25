#include "musica.h"
#include <stdio.h>

Mix_Music* musica_menu_tetris(){
    Mix_Music *tetris_musica = Mix_LoadMUS("./assets/musicas/tetris_song.mp3");
    if (!tetris_musica) {
        fprintf(stderr, "erro ao carregar musica do tetris: %s\n", Mix_GetError());
        return NULL;
    }
    return tetris_musica;
}

Mix_Music* som_gameover(){
    Mix_Music *gameover_musica = Mix_LoadMUS("./assets/musicas/game_over_tetris_ver.mp3");
    if (!gameover_musica) {
        fprintf(stderr, "Erro ao carregar musica de game over: %s\n", Mix_GetError());
        return NULL;
    }
    return gameover_musica;
}

Mix_Chunk* som_peca_fixada(){
    Mix_Chunk *peca_fixada = Mix_LoadWAV("./assets/musicas/drop.wav");
    if (!peca_fixada) {
        fprintf(stderr, "Erro ao carregar som da peca fixada: %s\n", Mix_GetError());
        return NULL;
    }
    return peca_fixada;
}

Mix_Chunk* som_explosao(){
    Mix_Chunk *explosao = Mix_LoadWAV("./assets/musicas/explosion.wav");
    if (!explosao) {
        fprintf(stderr, "Erro ao carregar som de explosao: %s\n", Mix_GetError());
        return NULL;
    }
    return explosao;
}

Mix_Chunk* som_linha_remov(){
    Mix_Chunk *linha = Mix_LoadWAV("./assets/clear.wav");
    if (!linha) {
        fprintf(stderr, "Erro ao carregar som de linha removida: %s\n", Mix_GetError());
        return NULL;
    }
    return linha;
}

Mix_Chunk* som_novo_nivel(){
    Mix_Chunk *nivel = Mix_LoadWAV("./assets/levelup.wav");
    if (!nivel) {
        fprintf(stderr, "Erro ao carregar som de novo nivel: %s\n", Mix_GetError());
        return NULL;
    }
    return nivel;
}

void inicializar_audio(){
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Nao foi possível inicializar o SDL: %s\n", SDL_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4100) < 0) {
        fprintf(stderr, "Erro ao inicializar SDL_mixer: %s\n", Mix_GetError());
    }
}
