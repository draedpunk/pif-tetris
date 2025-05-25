#include "music.h"

Mix_Music* musica_menu_tetris(){
    Mix_Music *tetris_musica = Mix_LoadMUS("./assets/musicas/tetris_song.mp3");
    if (!tetris_musica) {
        fprintf(stderr, "Erro ao carregar musica do tetris: %s\n", Mix_GetError());
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

Mix_Music* som_peca_fixada(){
    Mix_Music *peca_fixada_musica = Mix_LoadMUS("./assets/musicas/drop.wav");
    if (!peca_fixada_musica) {
        fprintf(stderr, "Erro ao carregar musica de peca fixada: %s\n", Mix_GetError());
        return NULL;
    }

    return peca_fixada_musica; 

}

Mix_Music* som_explosao(){
    Mix_Music *explosao_musica = Mix_LoadMUS("./assets/musicas/explosion.wav");
    if (!explosao_musica) {
        fprintf(stderr, "Erro ao carregar musica de explosao: %s\n", Mix_GetError());
        return NULL;
    }

    return explosao_musica; 

}

Mix_Music* som_linha_remov(){
    Mix_Music *linha_removida_musica = Mix_LoadMUS("./assets/clear.wav");
    if (!linha_removida_musica) {
        fprintf(stderr, "Erro ao carregar musica da linha: %s\n", Mix_GetError());
        return NULL;
    }

    return linha_removida_musica;

}

Mix_Music* som_novo_nivel(){
    Mix_Music *nivel_musica = Mix_LoadMUS("./assets/levelup.wav");
    if (!nivel_musica) {
        fprintf(stderr, "Erro ao carregar musica de novo nivel: %s\n", Mix_GetError());
        return NULL;
    }

    return nivel_musica;

}

void inicializar_audio(){
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Nao foi possível inicializar o SDL: %s\n", SDL_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4100) < 0) {
        fprintf(stderr, "Erro ao inicializar SDL_mixer: %s\n", Mix_GetError());
    }

}