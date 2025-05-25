#ifndef _PONTUACAO_H
#define _PONTUACAO_H

// #include "tetraminos.h";
// #include "tetris.h";
// #include "mapa.h"
#define TOP_MAIORES_PONT 5
#define LIMITE_JOGADORES 50

typedef struct{
    char nome[30];
    int pontuacao;
} Jogador;

void atualizar_pontuacao(int *pontuacao, int linhas, int eh_explosiva);
void salvar_pontuacao(const char *nome, int pontuacao);
void input_nome_jogador(char *nome_destino);
void exibir_campo_nome(const char nome[]);
void receber_dados_pontuacao(Jogador pontos[], int *qtd_dados);
void colocar_ordem_cresc(Jogador pontos[], int qtd_dados);
void exibir_ranking();
void exibir_pontuacao(int *pontuacao);

#endif