#include <SDL2/SDL.h>  
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include "musica.h"

void inicializar_audio(Musica *audio) {
    audio->audio_ativado = 1;  
    
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Audio desativado: %s\n", SDL_GetError());
        audio->audio_ativado = 0;
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Mixer desativado: %s\n", Mix_GetError());
        audio->audio_ativado = 0;
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return;
    }
}

Musica* carregar_sons() {
    Musica *audio = calloc(1, sizeof(Musica));
    if (!audio) return NULL;
    
    inicializar_audio(audio);
    
    if (!audio->audio_ativado) {
        return audio;
    }

    audio->musica_tetris = Mix_LoadMUS("assets/musicas/tetris.ogg");
    audio->musica_gameover = Mix_LoadMUS("assets/musicas/gameover.ogg");
    audio->som_linha = Mix_LoadWAV("assets/musicas/clear.wav");
    audio->som_nivel = Mix_LoadWAV("assets/musicas/levelup.wav");
    audio->som_peca = Mix_LoadWAV("assets/musicas/drop.wav");
    audio->som_explosao = Mix_LoadWAV("assets/musicas/explosion.wav");

    return audio;
}

void liberar_sons(Musica *audio) {
    if (!audio) return;
    
    if (audio->audio_ativado) {
        Mix_HaltMusic();
        Mix_HaltChannel(-1);
        
        Mix_FreeMusic(audio->musica_tetris);
        Mix_FreeMusic(audio->musica_gameover);
        Mix_FreeChunk(audio->som_linha);
        Mix_FreeChunk(audio->som_nivel);
        Mix_FreeChunk(audio->som_peca);
        Mix_FreeChunk(audio->som_explosao);
        
        Mix_CloseAudio();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
    
    free(audio);
}

Musica* inicializar_tudo_audio(int *audio_ok) {
    Musica *audio = carregar_sons();

    if (!audio || !audio->audio_ativado) {
        fprintf(stderr, "Audio desativado, seguindo sem som\n");
        *audio_ok = 0;
    } else {
        *audio_ok = 1;
    }

    return audio;
}
