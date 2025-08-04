#include <stdio.h>

typedef struct{

    int posicao_x;
    int posicao_y;
    int posicaoInicial_y;
    int alturaMaxAtinginda;
    int vidas;
    int pontuacao;
    int qtdMovimentoTotal;
    int qtdMovimentoBaixo;

}tGalinha;

// funcoes tGalinha

typedef struct{

    int id;
    int posicao_x;

}tCarro;

// funcoes tCarro

typedef struct{

    int id;
    int centro_y;

    int qtdCarros;
    tCarro carros[10];
    int velocidade;
    char direcao;

}tPista;

//funcoes tPista

typedef struct{

    int iteracao;
    int pista_id;
    int carro_id;
    int galinha_x;
    int galinha_y;

}tAtropelamento;

// funcoes tAtropelamento

typedef struct{

    int largura;
    int altura;

    int qtdPistas;
    tPista pistas[13];

    char planoCartesiano[35][101];

}tMapa;

//funcoes tMapa

typedef struct{

    tGalinha galinha;
    tMapa mapa;
    int iteracao;
    int ehAnimado;

    int qtdAtropelamentos;
    tAtropelamento atropelamentos[100];

    int heatmap[35][100];

}tJogo;

//funcoes tJogo

int main(){

    

    return 0;
}