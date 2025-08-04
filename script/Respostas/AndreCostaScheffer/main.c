#include <stdio.h>
#include <stdlib.h>

// Galinha
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

// Carro
typedef struct{

    int id;
    int posicao_x;

}tCarro;

void InicializaCarros(tCarro carros[], int qtdCarros, FILE *pArqConfig);

// Le e armazena config inicial
typedef struct{

    int ehAnimado;
    int larguraMapa;
    int qtdPistas;
    char configPistas[15][50];

}tConfig;

// Le e armazena skin personagens
typedef struct{

    char galinha[3];
    char carro[4][2];

}tSkin;

// Pista
typedef struct{

    int id;
    int centro_y;

    int qtdCarros;
    tCarro carros[10];
    int velocidade;
    char direcao;

}tPista;

void InicializaPistas(tPista pistas[], int qtdPistas, FILE *pArqConfig);

// Atropelamento
typedef struct{

    int iteracao;
    int pista_id;
    int carro_id;
    int galinha_x;
    int galinha_y;

}tAtropelamento;

// Mapa
typedef struct{

    int largura;
    int altura;

    int qtdPistas;
    tPista pistas[13];

    char planoCartesiano[35][101];

}tMapa;

tMapa InicializaMapa(int largura_mapa, int qtd_pistas, FILE *pArqConfig);
//já pode inicializar uma pista vazia e começar a leitura a partir da próxima pista;

// Jogo
typedef struct{

    tConfig config;
    tSkin skin;

    tGalinha galinha;
    tMapa mapa;
    int iteracao;
    int ehAnimado;

    int qtdAtropelamentos;
    tAtropelamento atropelamentos[100];

    int heatmap[35][100];

}tJogo;

tJogo InicializaJogo(int argv,char *diretorio[]);

int main(int argc, char *argv[]){

    InicializaJogo(argc, argv);

    return 0;
}

void InicializaCarros(tCarro carros[], int qtdCarros, FILE *pArqConfig){
    char a, b;
    fscanf
    printf("%c %c",a,b);
}

void InicializaPistas(tPista pistas[], int qtdPistas, FILE *pArqConfig)
{

    char linhaLida[50];
    int i;
    for(i = 0; i < qtdPistas; i++){

        fgets(linhaLida, 50, pArqConfig);

        if(linhaLida[0] == '\n'){
            pistas[i].id = (i+1);
            pistas[i].qtdCarros = 0;
            pistas[i].velocidade = 0;
            pistas[i].direcao = '\0';
            pistas[i].centro_y = i*3 + 1;
            continue;
        }

        pistas[i].id = (i+1);
        pistas[i].centro_y = i*3 + 1;
        sscanf(linhaLida,"%c %d %d",&pistas[i].direcao, 
                                    &pistas[i].velocidade, 
                                    &pistas[i].qtdCarros);
        
        InicializaCarros(pistas[i].carros, pistas[i].qtdCarros, linhaLida);
    }
}

tMapa InicializaMapa(int largura_mapa, int qtd_pistas, FILE *pArqConfig)
{

    tMapa mapa;

    mapa.largura = largura_mapa;
    mapa.altura = qtd_pistas*2 + qtd_pistas;

    mapa.qtdPistas = qtd_pistas;

    InicializaPistas(mapa.pistas, mapa.qtdPistas, pArqConfig);

    return mapa;
}

tJogo InicializaJogo(int argc, char *diretorio[])
{

    tJogo jogo;
    char linhaLidaAtualLeitura[20];

// Arquivo de Configuracao Inicial
    char arqConfigInicial[1021];
    sprintf(arqConfigInicial,"%s/config_inicial.txt",diretorio[1]);
    FILE *pArqConfigInicial = fopen(arqConfigInicial,"r");
    if(pArqConfigInicial == NULL){
        printf("ERRO: Informe o diretorio com os arquivos de configuracao.\n");
        exit(1);
    }

    fgets(linhaLidaAtualLeitura,20,pArqConfigInicial);
    if(sscanf(linhaLidaAtualLeitura, "%d",&jogo.ehAnimado) != 1){
        printf("Nao foi possivel escolher animacao\n");
        exit(1);
    }

    fgets(linhaLidaAtualLeitura,20,pArqConfigInicial);
    int largura_mapa;
    int qtd_pistas;
    if(sscanf(linhaLidaAtualLeitura, "%d %d",&largura_mapa,&qtd_pistas) != 2){
        printf("Nao foi possivel inicializar mapa\n");
        exit(1);
    }

// Copia o conteudo do pArqConfigInicial para uma matriz bidimensional
/*
    char textoPistas[qtd_pistas][50];
    int i;
    for(i = 0; fgets(linhaLidaAtualLeitura, 20, pArqConfigInicial) != NULL; i++){
        
        int k;
        for(k = 0; linhaLidaAtualLeitura[k] != '\0'; k++){

            textoPistas[i][k] = linhaLidaAtualLeitura[k];
        }

        textoPistas[i][k] = '\0';
    }*/

    jogo.mapa = InicializaMapa(largura_mapa, qtd_pistas, pArqConfigInicial);

    fclose(pArqConfigInicial);

// Arquivo de desenho de personagens
    char arqDesenhoPersonagens[1021];
    sprintf(arqDesenhoPersonagens,"%s/personagens.txt",diretorio[1]);
    FILE *pArqDesenhoPersonagens = fopen(arqDesenhoPersonagens,"r");
    if(pArqDesenhoPersonagens == NULL){
        printf("ERRO: Informe o diretorio com os arquivos de Desenho dos Personagens.\n");
        exit(1);
    }
    fclose(pArqDesenhoPersonagens);

    return jogo;
}
