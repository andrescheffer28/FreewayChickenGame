#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Le e armazena config inicial
typedef struct{

    int ehAnimado;
    int larguraMapa;
    int qtdPistas;
    
    //configPistas
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

    char galinha[8];
    char carro[25];

}tSkin;
tSkin LeSkins(char *argv[]);

// Galinha
typedef struct{

    int posicao_x;
    int posicao_y;
    int posicaoInicial_x;
    int posicaoInicial_y;
    int pistaInicial_id;
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
int CarroPosicao_x(tCarro carro);

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
int CentroPistaPosicao_y(tPista pista);

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

    // 100 colunas de espaço, 2 para bordas -0 e 101-, e 1 \0
    char desenhoMapa[35][102];

}tMapa;
tMapa InicializaMapa(tConfig config);
tMapa DesenhaCenario(tMapa mapa);
void ImprimeMapa(tMapa mapa, FILE *saida);
int CalculaPosicao_y(int ordemPistaCimaBaixo);
int CalculaAlturaSemBorda(int qtdPistas);

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
void DesenhaQualquerEntidade(char desenhoMapa[][102], int centro_x, int centro_y, int larguraMapa, const char skin[], int inicioSkinMatriz);
void DesenhaGalinha(char desenhoMapa[][102], int larguraMapa, tGalinha galinha, const char skinGalinha[]);
void DesenhaCarros(char desenhoMapa[][102], int larguraMapa, tPista pista, const char skinCarro[]);
void CriaArquivoInicializacao(tMapa mapa, tGalinha galinha);
tMapa DesenhaPersonagensMapa(tGalinha galinha, tSkin skins, tMapa mapa);
tMapa DesenhaMapa(tJogo jogo);

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
    galinha.pistaInicial_id = QtdPistas(config);
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

int CarroPosicao_x(tCarro carro){
    return carro.posicao_x;
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

tSkin LeSkins(char *argv[]){

    tSkin skin;

    char diretorio[1020];
    sprintf(diretorio,"%s/personagens.txt",argv[1]);

    FILE *pfile = fopen(diretorio, "r");
    if(pfile == NULL){
        printf("ERRO: Informe o argv com os arquivos de personagens.\n");
        exit(1);
    }

    char ch[50];
    fgets(ch,50,pfile);
    strncpy(skin.galinha, ch, 3);

    fgets(ch,50,pfile);
    strncpy(skin.galinha + 3, ch, 3);

    skin.galinha[6] = '\0';

    int i;
    for(i = 0; i < 4 ; i++){

        int offset = i*6;

        fgets(ch,50,pfile);
        strncpy(skin.carro + offset, ch, 3);

        fgets(ch,50,pfile);
        strncpy(skin.carro + offset + 3, ch, 3);
    }
    skin.carro[24] = '\0';

    fclose(pfile);
    return skin;
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

int CentroPistaPosicao_y(tPista pista){
    return pista.centro_y;
}

tMapa InicializaMapa(tConfig config)
{

    tMapa mapa;

    mapa.largura = LarguraMapa(config);
    mapa.qtdPistas = QtdPistas(config);

    // Desconsidera as bordas
    // 2 linhas em branco + 1 linha padrao, por pista
    //mapa.altura = mapa.qtdPistas*2 + mapa.qtdPistas;
    mapa.altura = CalculaAlturaSemBorda(mapa.qtdPistas);

    InicializaPistas(mapa.pistas, mapa.qtdPistas, config);

    return mapa;
}

// qtd de linhas totais - inclui bordas
// 27 = 25 + 2
// 47 = 45 + 2

// qtd de colunas totais - inclui bordas
// 13 = 4*3 + 1
// 16 = 5*3 + 1

tMapa DesenhaCenario(tMapa mapa){

    int i;
    for(i = 0; i < mapa.qtdPistas - 1; i++){

        int altura = CalculaAlturaSemBorda(i+1);
        int j;
        for(j = (altura - 3); j < altura; j++){

            //desenha 2 linhas vazias
            if(j <= (altura - 2)){
                int k;
                for(k = 0; k < mapa.largura; k++){
                    mapa.desenhoMapa[j][k] = ' ';
                }

            //desenha 1 linha padrao entre pistas
            }else{
                int k;
                for(k = 0; k < mapa.largura; k++){

                    if((k + 1)%3){
                        mapa.desenhoMapa[j][k] = '-';
                    }else{
                        mapa.desenhoMapa[j][k] = ' ';
                    }
                }
            }
        }
    }

    // Desenha Pista Inicial Galinha
    int altura  = CalculaAlturaSemBorda(mapa.qtdPistas);
    for(i = altura - 3; i < altura; i++){
        int j;
        for(j = 0; j < mapa.largura; j++){
            mapa.desenhoMapa[i][j] = ' ';
        }
    }
    return mapa;
}

// Imprime a area Jogavel Com Moldura:
// stdout - terminal
// ou informe um diretorio com arquivo valido
// simplificar quando tiver tempo a logica
void ImprimeMapa(tMapa mapa, FILE *saida){

    fprintf(saida,"|");
    int i;
    for(i = 0; i < mapa.largura; i ++){
        fprintf(saida,"-");
    }
    fprintf(saida,"|\n");

    for(i = 0; i < mapa.altura-1; i++){

        if(i < (mapa.altura-1)) 
        fprintf(saida,"|");

        int j;
        for(j = 0; j < mapa.largura; j++){

            fprintf(saida,"%c",mapa.desenhoMapa[i][j]);
        }

        if(i < (mapa.altura-1)) 
        fprintf(saida,"|");

        // i tem q ser menor do que, n pode colocar i !=
        if(i < (mapa.altura-2)) 
        fprintf(saida,"\n");
    }

    fprintf(saida,"\n|");
    for(i = 0; i < mapa.largura; i++){
        fprintf(saida,"-");
    }
    fprintf(saida,"|");
}

int CalculaPosicao_y(int ordemPistaCimaBaixo){
    int posicao_y = ordemPistaCimaBaixo*3 - 2;
    return posicao_y;
}

// 1 pista corresponde a 1
int CalculaAlturaSemBorda(int qtdPistas){

    // Cada Pista tem 2 linhas vazias
    // e uma linha padrao
    int altura;
    altura = qtdPistas*2 + qtdPistas;

    return altura;
}

tJogo InicializaJogo(int argc, char *argv[])
{

    tJogo jogo;

    jogo.config = LeConfiguracoes(argv);
    jogo.mapa = InicializaMapa(jogo.config);
    jogo.galinha = InicializaGalinha(jogo.config);
    jogo.skin = LeSkins(argv);
    jogo.mapa = DesenhaMapa(jogo);

    CriaArquivoInicializacao(jogo.mapa, jogo.galinha);

    return jogo;
}

// se for do tipo 2altura x 3 largura
void DesenhaQualquerEntidade(char desenhoMapa[][102], int centro_x, int centro_y, int larguraMapa, const char skin[], int inicioSkinMatriz){
    int j = inicioSkinMatriz;
    int centroMatriz_x = centro_x - 1;
    int cabecaCentro_y = centro_y - 1;

    int esquerda_x = centroMatriz_x - 1;
    int direita_x = centroMatriz_x + 1;

    esquerda_x = (esquerda_x%larguraMapa + larguraMapa)%larguraMapa;
    centroMatriz_x = (centroMatriz_x%larguraMapa + larguraMapa)%larguraMapa;
    direita_x = (direita_x%larguraMapa + larguraMapa)%larguraMapa;

    desenhoMapa[cabecaCentro_y][esquerda_x] = skin[j+0];
    desenhoMapa[cabecaCentro_y][centroMatriz_x] = skin[j+1];
    desenhoMapa[cabecaCentro_y][direita_x] = skin[j+2];

    int corpoCentro_y = cabecaCentro_y + 1;
    desenhoMapa[corpoCentro_y][esquerda_x] = skin[j+3];
    desenhoMapa[corpoCentro_y][centroMatriz_x] = skin[j+4];
    desenhoMapa[corpoCentro_y][direita_x] = skin[j+5];
}

void DesenhaGalinha(char desenhoMapa[][102], int larguraMapa, tGalinha galinha, const char skinGalinha[]){

    int inicioSkinMatriz = 0;
    DesenhaQualquerEntidade(desenhoMapa, galinha.posicao_x, galinha.posicao_y, larguraMapa, skinGalinha, inicioSkinMatriz);
}

void DesenhaCarros(char desenhoMapa[][102], int larguraMapa, tPista pista, const char skinCarro[]){

    int i;
    for(i = 0; i < pista.qtdCarros; i++){

        int posicao_x = CarroPosicao_x(pista.carros[i]);
        int posicao_y = pista.centro_y;
        int inicioSkinMatriz = 0;
        DesenhaQualquerEntidade(desenhoMapa, posicao_x, posicao_y, larguraMapa, skinCarro, inicioSkinMatriz);
    }
}

void CriaArquivoInicializacao(tMapa mapa, tGalinha galinha){

    FILE *saida = fopen("./saida/inicializacao.txt","w");
    if(saida == NULL){
        printf("Nao foi possivel criar arquivo Inicializacao.txt na pasta saida\n");
        exit(1);
    }

    ImprimeMapa(mapa,saida);
    int pos_x = galinha.posicaoInicial_x;
    int pos_y = galinha.posicaoInicial_y;
    fprintf(saida,"\nA posicao central da galinha iniciara em (%d %d).",pos_x,pos_y);

    fclose(saida);
}

tMapa DesenhaPersonagensMapa(tGalinha galinha, tSkin skins, tMapa mapa){

    int i;
    for(i = 0; i < mapa.qtdPistas; i++){
        
        int posicaoPista_y = CentroPistaPosicao_y(mapa.pistas[i]);
        if(posicaoPista_y == galinha.posicao_y){
            DesenhaGalinha(mapa.desenhoMapa, mapa.largura, galinha, skins.galinha);
        }

        DesenhaCarros(mapa.desenhoMapa, mapa.largura, mapa.pistas[i], skins.carro);
    }
    return mapa;
}

tMapa DesenhaMapa(tJogo jogo){

    jogo.mapa = DesenhaCenario(jogo.mapa);
    jogo.mapa = DesenhaPersonagensMapa(jogo.galinha, jogo.skin, jogo.mapa);

    return jogo.mapa;
}
