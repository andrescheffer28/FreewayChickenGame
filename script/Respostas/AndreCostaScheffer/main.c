#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Le e armazena config inicial
typedef struct{

    int ehAnimado;
    int larguraMapa;
    int qtdPistas;
    char cfPistas[15][50];

}tConfig;
tConfig LeConfiguracoes(char *argv[]);
void CopiaLinhaConfigPistas(tConfig config, char copia[], int posicaoLinha, int limite);
void LinhaConfigPistasProcurada(tConfig config, char linhaCopia[], int limite, char keyLetra);
int EhAnimado(tConfig config);
int LarguraMapa(tConfig config);
int QtdPistas(tConfig config);

// Le e armazena skin personagens
typedef struct{

    char galinha[3];
    char carro[4][2];

}tSkin;
tSkin LeSkins(char *argv[]);

// Galinha
typedef struct{

    int posicao_x;
    int posicao_y;
    int posicaoInicial_x;
    int posicaoInicial_y;
    int alturaMaxAtinginda;
    int vidas;
    int pontuacao;
    int qtdMovimentoTotal;
    int qtdMovimentoBaixo;

}tGalinha;
tGalinha InicializaGalinha(tConfig config);

// Carro
typedef struct{

    int id;
    int posicao_x;

}tCarro;
void InicializaCarros(tCarro carros[], int qtdCarros, int posicoesCarros_x[]);

// Pista
typedef struct{

    int id;
    int centro_y;

    int qtdCarros;
    tCarro carros[10];
    int velocidade;
    char direcao;

}tPista;
void InicializaPistas(tPista pistas[], int qtdPistas, tConfig config);

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
tMapa InicializaMapa(tConfig config);
int CalculaPosicao_y(int ordemPistaCimaBaixo);

// Jogo
typedef struct{

    tConfig config;
    tSkin skin;

    tGalinha galinha;
    tMapa mapa;
    int iteracao;

    int qtdAtropelamentos;
    tAtropelamento atropelamentos[100];

    int heatmap[35][100];

}tJogo;

tJogo InicializaJogo(int argc,char *argv[]);

int main(int argc, char *argv[]){

    InicializaJogo(argc, argv);

    return 0;
}

tGalinha InicializaGalinha(tConfig config){

    tGalinha galinha;

    char linhaConfigGalinha[10];
    LinhaConfigPistasProcurada(config, linhaConfigGalinha, 10, 'G');

    sscanf(linhaConfigGalinha, "%*c %d %d", &galinha.posicaoInicial_x,
                                            &galinha.vidas);
    int pistaGalinha = QtdPistas(config);
    galinha.posicaoInicial_y = CalculaPosicao_y(pistaGalinha);

    galinha.posicao_x = galinha.posicaoInicial_x;
    galinha.posicao_y = galinha.posicaoInicial_y;
    galinha.pontuacao = 0;
    galinha.qtdMovimentoTotal = 0;
    galinha.qtdMovimentoBaixo = 0;
    
    return galinha;
}

void InicializaCarros(tCarro carros[], int qtdCarros, int posicoesCarros_x[])
{

    int i;
    for(i = 0; i < qtdCarros; i++){
        carros[i].id = (i+1);
        carros[i].posicao_x = posicoesCarros_x[i];
    }
}

tConfig LeConfiguracoes(char *argv[]){

    tConfig config;

    char diretorio[1025];
    sprintf(diretorio,"%s/config_inicial.txt",argv[1]);
    FILE *pfile = fopen(diretorio,"r");

    if(pfile == NULL){
        printf("ERRO: Informe o argv com os arquivos de configuracao.\n");
        exit(1);
    }

    char linha[50];

    if(fgets(linha,50,pfile) == NULL){
        printf("Nao foi possivel ler 1nd linha arq Config\n");
        fclose(pfile);
        exit(1);
    }
    sscanf(linha,"%d",&config.ehAnimado);

    if(fgets(linha,50,pfile) == NULL){
        printf("Nao foi possivel ler 2nd linha arq Config\n");
        fclose(pfile);
        exit(1);
    }
    sscanf(linha,"%d %d",&config.larguraMapa,&config.qtdPistas);

// Leitura das config de Pistas
    int i;
    for(i = 0; fgets(linha,50,pfile) != NULL; i++){

        sprintf(config.cfPistas[i],"%s",linha);
    }


    fclose(pfile);

    return config;
}

void CopiaLinhaConfigPistas(tConfig config, char copia[], int posicaoLinha, int limite){

    strncpy(copia, config.cfPistas[posicaoLinha], (limite-1));
    copia[limite-1] = '\0';
}

void LinhaConfigPistasProcurada(tConfig config, char linhaCopia[], int limite, char keyLetra){

    int i;
    for(i = 0; i < config.qtdPistas; i++){

        if(config.cfPistas[i][0] == keyLetra){
            strncpy(linhaCopia, config.cfPistas[i], (limite-1));
            linhaCopia[limite-1] = '\0';
            break;
        }
    }
}

int EhAnimado(tConfig config){
    return config.ehAnimado;
}

int LarguraMapa(tConfig config){
    return config.larguraMapa;
}

int QtdPistas(tConfig config){
    return config.qtdPistas;
}

void InicializaPistas(tPista pistas[], int qtdPistas, tConfig config)
{

    char configPistas[52];

    int i;
    for(i = 0; i < qtdPistas; i++){

        CopiaLinhaConfigPistas(config, configPistas, i, 52);

        if(configPistas[0] == '\n'){
            pistas[i].id = (i+1);
            pistas[i].qtdCarros = 0;
            pistas[i].velocidade = 0;
            pistas[i].direcao = '0';
            pistas[i].centro_y = i*3 + 1;
            continue;
        }

        if(configPistas[0]  == 'G'){
            pistas[i].id = (i+1);
            pistas[i].qtdCarros = 0;
            pistas[i].velocidade = 0;
            pistas[i].direcao = '0';
            pistas[i].centro_y = i*3 + 1;
            continue;
        }

        pistas[i].id = (i+1);
        pistas[i].centro_y = i*3 + 1;

        int offset = 0;
        sscanf(configPistas,"%c %d %d %n",  &pistas[i].direcao, 
                                            &pistas[i].velocidade, 
                                            &pistas[i].qtdCarros,
                                            &offset);
        
        int posicoesCarros_x[11];
        int j;
        for(j = 0; j < pistas[i].qtdCarros; j++){
            int qtdLida = 0;
            sscanf(configPistas + offset, "%d %n",&posicoesCarros_x[j],&qtdLida);
            offset = offset + qtdLida;
        }

        InicializaCarros(pistas[i].carros, pistas[i].qtdCarros, posicoesCarros_x);
    }
}

tMapa InicializaMapa(tConfig config)
{

    tMapa mapa;

    mapa.largura = LarguraMapa(config);
    mapa.qtdPistas = QtdPistas(config);
    mapa.altura = mapa.qtdPistas*2 + mapa.qtdPistas;

    InicializaPistas(mapa.pistas, mapa.qtdPistas, config);

    return mapa;
}

int CalculaPosicao_y(int ordemPistaCimaBaixo){
    int posicao_y = ordemPistaCimaBaixo*3 - 2;
    return posicao_y;
}

tJogo InicializaJogo(int argc, char *argv[])
{

    tJogo jogo;

    jogo.config = LeConfiguracoes(argv);
    jogo.mapa = InicializaMapa(jogo.config);
    jogo.galinha = InicializaGalinha(jogo.config);

// Arquivo de desenho de personagens

    return jogo;
}