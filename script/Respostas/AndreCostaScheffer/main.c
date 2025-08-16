#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    int alturaAnterior;
    int alturaMaxAtinginda;
    int vidas;
    int pontuacao;
    int qtdMovimentoTotal;
    int qtdMovimentoBaixo;

}tGalinha;
tGalinha InicializaGalinha(tGalinha galinha,int pistaGalinha_id, FILE *pFile);
tGalinha tGalinha_reseta(tGalinha galinha);
tGalinha AtualizaPosicaoGalinha(tGalinha galinha, int novaPosicao_x);
tGalinha MoveGalinha(tGalinha galinha, int heatmap[][100], char respostaUsuario);
tGalinha AtualizaPontuacaoGalinha(tGalinha galinha, int qtdGanha);

int GalinhaPosicao_y(tGalinha galinha);
int GalinhaPontuacao(tGalinha galinha);
int GalinhaVidas(tGalinha galinha);

// Carro
typedef struct{

    int id;
    int posicao_x;

}tCarro;
void InicializaCarros(tCarro carros[], int qtdCarros, FILE *pFile);
void AtualizaPosicaoCarros(tCarro Carros[], int qtdCarros, char direcao, int velocidade, int limitePista);
int CarroPosicao_x(tCarro carro);
int Carro_id(tCarro carro);

// Pista
typedef struct{

    int id;
    int centro_y;

    int qtdCarros;
    tCarro carros[10];
    int velocidade;
    char direcao;

}tPista;
void InicializaPistas(tPista pistas[], int qtdPistas, FILE *pFile);
int CentroPistaPosicao_y(tPista pista);

void AtualizaPistas(tPista pistas[], int qtdPistas, int largura);

// Atropelamento;
// usar iteracao como busca pra
// espaco vazio
typedef struct{

    int iteracao;
    int pista_id;
    int carro_id;
    int galinha_x;
    int galinha_y;

}tAtropelamento;
void InicializaAtropIteracao(tAtropelamento atropelamento[]);
void RegistraAtropelamento( tAtropelamento atropelamento[], tPista pista, 
                            tGalinha galinha, int carro_id,int iteracao );
void tAtropelamento_sorting(tAtropelamento atropelamentos[]);
int MinimaAlturaAtropelada(tAtropelamento atropelamento[], int alturaMapa);
int MaxAlturaAtropelada(tAtropelamento atropelamento[], int alturaMapa);

// Mapa
typedef struct{

    int largura;
    int altura;

    int qtdPistas;
    tPista pistas[13];

    // 100 colunas de espaço, 2 para bordas -0 e 101-, e 1 \0
    char desenhoMapa[35][102];

}tMapa;
tMapa InicializaMapa(tMapa mapa, FILE *pFile);
tMapa AtualizaMapa(tMapa mapa);
int tMapa_qtdPistas(tMapa mapa);

tMapa DesenhaCenario(tMapa mapa);
// 1 chamar desenhaMapa
// 2 chamar ImprimeMapa
void ImprimeMapa(tMapa mapa, FILE *saida);

int CalculaPosicao_y(int ordemPistaCimaBaixo);
int CalculaAlturaSemBorda(int qtdPistas);
int InverteAltura(int altura, int alturaTotal);

// Jogo
typedef struct{

    tSkin skin;
    int ehAnimado;

    tGalinha galinha;
    tMapa mapa;
    int iteracao;

    int qtdAtropelamentos;
    tAtropelamento atropelamentos[100];

    int ultimaIteracaoResumida;

    int heatmap[35][100];

}tJogo;

tJogo tJogo_LeConfiguracoes(tJogo jogo, char *argv[]);
tJogo InicializaJogo(char *argv[]);
tJogo ExecutaJogo(tJogo jogo, char *argv[]);
tJogo AtualizaEntidades(tJogo jogo, char inputUsuario);
tJogo TerminaJogo(tJogo jogo, int condicoesFim, char *argv[]);
tGalinha ProcessaGalinhaAtropelamento(  tMapa mapa, tGalinha galinha, tAtropelamento atropleamentos[], 
                                        int heatmap[][100] , int iteracao);

void DesenhaQualquerEntidade(   char desenhoMapa[][102], int centro_x, int centro_y, 
                                int larguraMapa, const char skin[], int inicioSkinMatriz);

void DesenhaGalinha(char desenhoMapa[][102], int larguraMapa, tGalinha galinha, const char skinGalinha[]);
void DesenhaCarros(char desenhoMapa[][102], int larguraMapa, tPista pista, const char skinCarro[]);
void CriaArquivoInicializacao(tMapa mapa, tGalinha galinha, char *argv[]);
tMapa DesenhaPersonagensMapa(tGalinha galinha, tSkin skins, tMapa mapa);
tMapa DesenhaMapa(tJogo jogo);

void ImprimePlacar(tJogo jogo);

char TerminalInput(tJogo jogo);
int EstaColidindo(tPista pista, tGalinha galinha);
int EhVitoria(tMapa mapa, tGalinha galinha);
int FimDeJogo(tJogo jogo);

void CriaEstatistaFile(tGalinha galinha, tAtropelamento atropelamentos[], tMapa mapa, char *argv[]);
void ResumeRodadaArquivo(int iteracao, tAtropelamento atropelamentos[], char *argv[]);
void EncerraResumo(int iteracao, char *argv[]);
void CriaRankingFile(tAtropelamento atropelamentos[], char *argv[]);

void tJogo_InicializaHeatMap(int heatmap[][100], tMapa mapa);
void tJogo_RegistroNormalHeatmap(int heatmap[][100], tGalinha galinha);
void tJogo_RegistroAtropelamentoHeatmap(int heatmap[][100], tGalinha galinha, tMapa mapa);
void CriaHeatmapFile(int heatmap[][100], tMapa mapa, char *argv[]);

int main(int argc, char *argv[]){

    tJogo jogo = InicializaJogo(argv);

    int fimJogo = FimDeJogo(jogo);
    while(!fimJogo){

        jogo = ExecutaJogo(jogo, argv);
        fimJogo = FimDeJogo(jogo);
    }

    // talvez somar 11
    jogo = TerminaJogo(jogo, fimJogo, argv);

    return 0;
}

tGalinha InicializaGalinha(tGalinha galinha, int pistaGalinha_id, FILE *pFile){

    char ch;
    while(fscanf(pFile,"%c",&ch) == 1 && ch != 'G');
    fscanf(pFile,"%d %d",&galinha.posicaoInicial_x,&galinha.vidas);

    galinha.posicaoInicial_y = CalculaPosicao_y(pistaGalinha_id);

    galinha.posicao_x = galinha.posicaoInicial_x;
    galinha.posicao_y = galinha.posicaoInicial_y;
    galinha.alturaAnterior = galinha.posicaoInicial_y;
    galinha.alturaMaxAtinginda = galinha.posicaoInicial_y;

    galinha.pistaInicial_id = pistaGalinha_id;
    galinha.pontuacao = 0;
    galinha.qtdMovimentoTotal = 0;
    galinha.qtdMovimentoBaixo = 0;
    
    return galinha;
}

tGalinha AtualizaPosicaoGalinha(tGalinha galinha, int novaPosicao_y){
    
    galinha.posicao_y = novaPosicao_y;

    return galinha;
}

// Move a galinha se for possivel
tGalinha MoveGalinha(tGalinha galinha, int heatmap[][100], char respostaUsuario){

    galinha.alturaAnterior = galinha.posicao_y;

    if(respostaUsuario == 'w'){
        int moveCima  = -3;
        galinha.qtdMovimentoTotal++;
        galinha = AtualizaPosicaoGalinha(galinha, (galinha.posicao_y + moveCima));

    }else if(respostaUsuario == 's'){
        
        if(galinha.posicao_y != galinha.posicaoInicial_y){
            int moveBaixo = + 3;
            galinha.qtdMovimentoTotal++;
            galinha.qtdMovimentoBaixo++;
            galinha = AtualizaPosicaoGalinha(galinha, (galinha.posicao_y + moveBaixo));

        }else if(galinha.posicao_y == galinha.posicaoInicial_y){
            int parada = 0;
            galinha = AtualizaPosicaoGalinha(galinha, (galinha.posicao_y + parada));
        }

    }else if(respostaUsuario == ' '){
        int parada = 0;
        galinha = AtualizaPosicaoGalinha(galinha, (galinha.posicao_y + parada));
    }

    tJogo_RegistroNormalHeatmap(heatmap, galinha);
    return galinha;
}

tGalinha AtualizaPontuacaoGalinha(tGalinha galinha, int qtdGanha){
    galinha.pontuacao += qtdGanha;  
    return galinha;
}

int GalinhaPosicao_y(tGalinha galinha){
    return galinha.posicao_y;
}

int GalinhaPontuacao(tGalinha galinha){
    return galinha.pontuacao;
}

int GalinhaVidas(tGalinha galinha){
    return galinha.vidas;
}

void InicializaCarros(tCarro carros[], int qtdCarros, FILE *pFile){

    int i;
    for(i = 0; i < qtdCarros; i++){
        carros[i].id = (i+1);
        fscanf(pFile, "%d",&carros[i].posicao_x);
    }

    // fim de linha
    char ch;
    fscanf(pFile,"%c",&ch);
    if(ch == ' ')
    fgetc(pFile);
}

void AtualizaPosicaoCarros(tCarro Carros[], int qtdCarros, char direcao, int velocidade, int limitePista){

    int i;
    for(i = 0; i < qtdCarros; i++){

        int deslocamento;

        if(direcao == 'D'){
            deslocamento = Carros[i].posicao_x + velocidade;
            deslocamento = (deslocamento%limitePista + limitePista)%limitePista;

        }else{
            deslocamento = Carros[i].posicao_x - velocidade;
            deslocamento = (deslocamento%limitePista + limitePista)%limitePista;

        }

        Carros[i].posicao_x = deslocamento;
    }
}

int CarroPosicao_x(tCarro carro){
    return carro.posicao_x;
}

int Carro_id(tCarro carro){
    return carro.id;
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

    char ch;
    int i;
    for(i = 0; i < 6; i++){

        fscanf(pfile,"%c",&ch);

        if(ch == '\n'){
            i--;
            continue;
        }

        skin.galinha[i] = ch;
    }
    skin.galinha[6] = '\0';

    for(i = 0; i < 24; i++){

        fscanf(pfile,"%c",&ch);

        if(ch == ' ' && !i){
            i--;
            continue;
        }

        if(ch == '\n'){
            i--;
            continue;
        }

        skin.carro[i] = ch;
    }
    skin.carro[24] = '\0';

    fclose(pfile);
    return skin;
}

void InicializaPistas(tPista pistas[], int qtdPistas, FILE *pFile){

    int i;
    int tirandoGalinha = qtdPistas-1;
    for(i = 0; i < tirandoGalinha; i++){
        char tipoPista;
        fscanf(pFile,"%c",&tipoPista);

        if(tipoPista == '\n'){
            pistas[i].id = (i+1);
            pistas[i].qtdCarros = 0;
            pistas[i].velocidade = 0;
            pistas[i].direcao = '0';
            pistas[i].centro_y = i*3 + 1;
            continue;
        }

        pistas[i].id = (i+1);
        fscanf(pFile,"%d %d",   &pistas[i].velocidade,
                                &pistas[i].qtdCarros);
        pistas[i].direcao = tipoPista;
        pistas[i].centro_y = i*3 + 1;
        InicializaCarros(pistas[i].carros, pistas[i].qtdCarros, pFile);
    }

    // Inicializa Pista Galinha
    pistas[i].id = (i+1);
    pistas[i].qtdCarros = 0;
    pistas[i].velocidade = 0;
    pistas[i].direcao = '0';
    pistas[i].centro_y = i*3 + 1;
}

int CentroPistaPosicao_y(tPista pista){
    return pista.centro_y;
}

void AtualizaPistas(tPista pistas[], int qtdPistas, int largura){

    int i;
    for(i = 0; i < qtdPistas; i++){
        
        if(pistas[i].qtdCarros){
            int qtdCarros = pistas[i].qtdCarros;
            int velocidade = pistas[i].velocidade;
            AtualizaPosicaoCarros(pistas[i].carros, qtdCarros, pistas[i].direcao, velocidade, largura);
        }
    }
}

void InicializaAtropIteracao(tAtropelamento atropelamento[]){

    int i;
    for(i = 0; i < 90; i++){
        atropelamento[i].iteracao = 0;
    }
}

void RegistraAtropelamento( tAtropelamento atropelamento[], tPista pista, 
                            tGalinha galinha, int carro_id,int iteracao){

    int i;
    int limiteAtrop = 90;
    for(i = 0; i < limiteAtrop; i++){
        
        if(!atropelamento[i].iteracao){

            atropelamento[i].iteracao = iteracao;
            atropelamento[i].pista_id = pista.id;
            atropelamento[i].carro_id = carro_id;
            atropelamento[i].galinha_x = galinha.posicao_x;
            atropelamento[i].galinha_y = galinha.posicao_y;
            break;
        }
    }
}

int MinimaAlturaAtropelada(tAtropelamento atropelamento[], int alturaMapa){

    int alturaMinima = 100; 
    int i;
    for(i = 0; atropelamento[i].iteracao; i++){

        int alturaAtual = InverteAltura(atropelamento[i].galinha_y, alturaMapa);
        if(alturaMinima > alturaAtual){
            alturaMinima = alturaAtual;
        }
    }

    if(alturaMinima == 100){
        return 0;
    }

    return alturaMinima;
}

int MaxAlturaAtropelada(tAtropelamento atropelamento[], int alturaMapa){

    int alturaMax = 0; 
    int i;
    for(i = 0; atropelamento[i].iteracao; i++){

        int alturaAtual = InverteAltura(atropelamento[i].galinha_y, alturaMapa);
        if(alturaMax < alturaAtual){
            alturaMax = alturaAtual;
        }
    }
 
    return alturaMax;
}

tMapa InicializaMapa(tMapa mapa, FILE *pFile){

    fscanf(pFile, "%d %d",&mapa.largura,&mapa.qtdPistas);
    // fim de linha
    fgetc(pFile);

    // Desconsidera as bordas
    // 2 linhas em branco + 1 linha padrao, por pista
    //mapa.altura = mapa.qtdPistas*2 + mapa.qtdPistas;
    mapa.altura = CalculaAlturaSemBorda(mapa.qtdPistas);

    InicializaPistas(mapa.pistas, mapa.qtdPistas, pFile);

    return mapa;
}

tMapa AtualizaMapa(tMapa mapa){

    AtualizaPistas(mapa.pistas, mapa.qtdPistas, mapa.largura);
    return mapa;
}

int tMapa_qtdPistas(tMapa mapa){
    return mapa.qtdPistas;
}

// qtd de linhas totais - inclui bordas
// 27 = 25 + 2
// 47 = 45 + 2

// qtd de colunas totais - inclui bordas
// 13 = 4*3 + 1
// 16 = 5*3 + 1

tMapa DesenhaCenario(tMapa mapa){

    int i;
    //desenha as pistas menos a da galinha
    for(i = 0; i < mapa.qtdPistas - 1; i++){

        int altura = CalculaAlturaSemBorda(i+1);
        int j;
        //inicia na altura 0 em relacao a pista atual
        for(j = (altura - 3); j < altura; j++){

            //desenha 2 linhas vazias
            if(j <= (altura - 2)){
                int k;
                for(k = 0; k < mapa.largura; k++){
                    mapa.desenhoMapa[j][k] = ' ';
                }

            //desenha 1 linha padrao entre pistas
            //na ultima linha da pista atual
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

    if(saida == stdout)
        printf("\n");
}

int CalculaPosicao_y(int ordemPistaCimaBaixo){
    int posicao_y = ordemPistaCimaBaixo*3 - 2;
    return posicao_y;
}

// a primeira pista corresponde ao valor 1
int CalculaAlturaSemBorda(int qtdPistas){

    // Cada Pista tem 2 linhas vazias
    // e uma linha padrao
    int altura;
    altura = qtdPistas*2 + qtdPistas;

    return altura;
}

// Ajusta para que a altura se escale conforme
// se aproxima do topo
int InverteAltura(int altura, int alturaTotal){

    altura = alturaTotal - altura;

    return altura;
}

tJogo tJogo_LeConfiguracoes(tJogo jogo, char *argv[]){

    char diretorio[1020];
    sprintf(diretorio,"%s/config_inicial.txt",argv[1]);

    FILE *pFile = fopen(diretorio,"r");
    if(pFile == NULL){
        printf("ERRO: Informe o argv com os arquivos de configuracao.\n");
        exit(1);
    }

    fscanf(pFile,"%d",&jogo.ehAnimado);
    jogo.mapa = InicializaMapa(jogo.mapa, pFile);
    jogo.galinha = InicializaGalinha(jogo.galinha, tMapa_qtdPistas(jogo.mapa), pFile);

    fclose(pFile);

    return jogo;
}

tJogo InicializaJogo(char *argv[]){

    tJogo jogo;

    jogo.ultimaIteracaoResumida = 0;
    jogo = tJogo_LeConfiguracoes(jogo, argv);
    InicializaAtropIteracao(jogo.atropelamentos);
    jogo.skin = LeSkins(argv);
    jogo.mapa = DesenhaMapa(jogo);
    tJogo_InicializaHeatMap(jogo.heatmap,jogo.mapa);

    CriaArquivoInicializacao(jogo.mapa, jogo.galinha, argv);
    ImprimePlacar(jogo);
    ImprimeMapa(jogo.mapa,stdout);

    tJogo_RegistroNormalHeatmap(jogo.heatmap, jogo.galinha);

    return jogo;
}

tJogo ExecutaJogo(tJogo jogo, char *argv[]){

    char userResposta = TerminalInput(jogo);
    if(userResposta != '0'){

        jogo.iteracao++;

        jogo = AtualizaEntidades(jogo, userResposta);
        jogo.mapa = DesenhaMapa(jogo);

        if(!FimDeJogo(jogo)){
            ImprimePlacar(jogo);
            ImprimeMapa(jogo.mapa,stdout);  
        }
    }

    if(jogo.ultimaIteracaoResumida < jogo.iteracao){
        jogo.ultimaIteracaoResumida = jogo.iteracao;

        ResumeRodadaArquivo(jogo.iteracao, jogo.atropelamentos, argv);
    }

    return jogo;
}

tJogo AtualizaEntidades(tJogo jogo, char inputUsuario){

    jogo.galinha = MoveGalinha(jogo.galinha, jogo.heatmap, inputUsuario);
    jogo.mapa = AtualizaMapa(jogo.mapa);

    jogo.galinha = ProcessaGalinhaAtropelamento(jogo.mapa, jogo.galinha, jogo.atropelamentos, 
                                                jogo.heatmap, jogo.iteracao);

    return jogo;
}

// condicoesFim;
// Retorna 1 - vitoria
// Retorna 2 - sem vidas
tJogo TerminaJogo(tJogo jogo, int condicoesFim, char *argv[]){

    if(condicoesFim == 1){
        int qtdGanha = 10;
        jogo.galinha = AtualizaPontuacaoGalinha(jogo.galinha, qtdGanha);
    }

    ImprimePlacar(jogo);
    ImprimeMapa(jogo.mapa,stdout);  

    if(condicoesFim == 1){
        printf("Parabens! Voce atravessou todas as pistas e venceu!");

    }else if(condicoesFim == 2){
        printf("Voce perdeu todas as vidas! Fim de jogo.");
    }

    CriaEstatistaFile(jogo.galinha, jogo.atropelamentos, jogo.mapa, argv);
    CriaRankingFile(jogo.atropelamentos,argv);
    CriaHeatmapFile(jogo.heatmap, jogo.mapa, argv);
    EncerraResumo(jogo.iteracao, argv);

    return jogo;
}

// verifica colisao, reseta a galinha caso haja
// salva atropelamento na matriz de atropelamentos;
// caso contrario, aumenta a pontuacao
tGalinha ProcessaGalinhaAtropelamento(  tMapa mapa, tGalinha galinha, tAtropelamento atropleamentos[], 
                                        int heatmap[][100] , int iteracao   ){

    int i;
    for(i = 0; i < mapa.qtdPistas; i++){

        int pos_y = CentroPistaPosicao_y(mapa.pistas[i]);
        if(pos_y == galinha.posicao_y){

            int carro_id = EstaColidindo(mapa.pistas[i], galinha);
            if(carro_id){

                tJogo_RegistroAtropelamentoHeatmap(heatmap, galinha, mapa);
                RegistraAtropelamento(atropleamentos, mapa.pistas[i], galinha, carro_id, iteracao);

                galinha = tGalinha_reseta(galinha);
                tJogo_RegistroNormalHeatmap(heatmap, galinha);

                return galinha;
            }else{

                if(galinha.posicao_y < galinha.alturaAnterior){
                    galinha.alturaAnterior = galinha.posicao_y;
                    galinha.pontuacao++;
                }

                if(galinha.alturaMaxAtinginda > galinha.posicao_y){
                    galinha.alturaMaxAtinginda = galinha.posicao_y;
                }

                return galinha;
            }
        }
    }

    return galinha;
}

// se for do tipo 2altura x 3 largura
void DesenhaQualquerEntidade(   char desenhoMapa[][102], int centro_x, int centro_y, 
                                int larguraMapa, const char skin[], int inicioSkinMatriz){
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

void CriaArquivoInicializacao(tMapa mapa, tGalinha galinha, char *argv[]){

    char diretorio[1020];
    sprintf(diretorio,"%s/saida/inicializacao.txt",argv[1]);

    FILE *saida = fopen(diretorio,"w");
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

// 1 chamar desenhaMapa
// 2 chamar ImprimeMapa
tMapa DesenhaMapa(tJogo jogo){

    jogo.mapa = DesenhaCenario(jogo.mapa);
    jogo.mapa = DesenhaPersonagensMapa(jogo.galinha, jogo.skin, jogo.mapa);

    return jogo.mapa;
}

void ImprimePlacar(tJogo jogo){

    int pontos = GalinhaPontuacao(jogo.galinha);
    int vidas = GalinhaVidas(jogo.galinha);

    printf("Pontos: %d | Vidas: %d | Iteracoes: %d\n",pontos, vidas, jogo.iteracao);
}

// Le stdin
// Retorna 'w','s' ou ' ' caso leia com sucesso
// Retorna 0 para caracter invalido
char TerminalInput(tJogo jogo){

    char respostaUsuario;
    scanf("%c",&respostaUsuario);

    switch(respostaUsuario){

        case 'w':
        case 's':
        case ' ':
            return respostaUsuario;
            break;
        default:
    }

    return '0';
}

// Verifique se na pista atual da galinha, ha
// alguma carro colindindo com ela
// se houver, retorna o carro_id
// se nao houver, retorna 0
int EstaColidindo(tPista pista, tGalinha galinha){

    if(!pista.qtdCarros)
    return 0;

    int galinhaEsquerda = galinha.posicao_x - 1;
    int galinhaDireita = galinha.posicao_x + 1;

    int i;
    for(i = 0; i < pista.qtdCarros; i++){

        int carro_x = CarroPosicao_x(pista.carros[i]);
        int carroEsquerda = carro_x - 1;
        int carroDireita = carro_x + 1;

        if((galinha.posicao_x >= carroEsquerda && galinha.posicao_x <= carroDireita) ||
            galinhaDireita == carroEsquerda || galinhaEsquerda == carroDireita){

            int carro_id = Carro_id(pista.carros[i]);
            return carro_id;
        }
    }

    return 0;
}

int EhVitoria(tMapa mapa, tGalinha galinha){

    int ultimaPista_y = CalculaPosicao_y(1);
    if(ultimaPista_y == galinha.posicao_y)
    return 1;

    return 0;
}

// Retorna 0 - jogo n terminado
// Retorna 1 - vitoria
// Retorna 2 - sem vidas
int FimDeJogo(tJogo jogo){

    if(EhVitoria(jogo.mapa, jogo.galinha)){

        int qtdGanhaPts = 10;
        jogo.galinha = AtualizaPontuacaoGalinha(jogo.galinha, qtdGanhaPts);
        return 1;
    }

    if(!GalinhaVidas(jogo.galinha)){
        return 2;
    }

    return 0;
}

void CriaEstatistaFile(tGalinha galinha, tAtropelamento atropelamentos[], tMapa mapa, char *argv[])
{

    char diretorio[1020];
    sprintf(diretorio, "%s/saida/estatistica.txt", argv[1]);

    FILE *pFile = fopen(diretorio,"w");
    if(pFile == NULL){
        printf("ERRO: nao foi possivel gerar arquivo de estatistica na pasta saida\n");
        return;
    }

    int alturaMaxAting = InverteAltura(galinha.alturaMaxAtinginda, mapa.altura);
    int alturaMaxAtrop = MaxAlturaAtropelada(atropelamentos, mapa.altura);
    int alturaMinAtrop = MinimaAlturaAtropelada(atropelamentos, mapa.altura);

    fprintf(pFile,"Numero total de movimentos: %d\n", galinha.qtdMovimentoTotal);
    fprintf(pFile,"Altura maxima que a galinha chegou: %d\n", alturaMaxAting);
    fprintf(pFile,"Altura maxima que a galinha foi atropelada: %d\n", alturaMaxAtrop);
    fprintf(pFile,"Altura minima que a galinha foi atropelada: %d\n", alturaMinAtrop);
    fprintf(pFile,"Numero de movimentos na direcao oposta: %d\n", galinha.qtdMovimentoBaixo);

    fclose(pFile);
}

void ResumeRodadaArquivo(int iteracao, tAtropelamento atropelamentos[], char *argv[]){

    int teveAtropelamento = 0;
    int i;
    for(i = 0; atropelamentos[i].iteracao; i++){

        if(atropelamentos[i].iteracao == iteracao){
            teveAtropelamento = 1;
            break;
        }
    }

    if(teveAtropelamento){

        char diretorio[1020];
        sprintf(diretorio,"%s/saida/resumo.txt",argv[1]);

        FILE *pFile = fopen(diretorio,"a");
        fprintf(pFile, "[%d] Na pista %d o carro %d atropelou a galinha na posicao (%d,%d).\n",
                                                                                        atropelamentos[i].iteracao,
                                                                                        atropelamentos[i].pista_id,
                                                                                        atropelamentos[i].carro_id,
                                                                                        atropelamentos[i].galinha_x,
                                                                                        atropelamentos[i].galinha_y);
    

        fclose(pFile);
    }

}

void EncerraResumo(int iteracao, char *argv[]){

    char diretorio[1020];
    sprintf(diretorio,"%s/saida/resumo.txt",argv[1]);

    FILE *pFile = fopen(diretorio,"a");
    fprintf(pFile, "[%d] Fim de jogo", iteracao);
    fclose(pFile);
}

tGalinha tGalinha_reseta(tGalinha galinha){

    galinha.vidas--;
    galinha.pontuacao = 0;
    galinha.alturaAnterior = 0;
    galinha.posicao_y = galinha.posicaoInicial_y;

    return galinha;
}

void CriaRankingFile(tAtropelamento atropelamentos[], char *argv[]){

    char diretorio[1020];
    sprintf(diretorio,"%s/saida/ranking.txt",argv[1]);

    FILE *pfile = fopen(diretorio,"w");
    if(pfile == NULL){
        printf("Nao foi possivel criar o arquivo ranking.txt na pasta saida\n");
        fclose(pfile);
    }
    fprintf(pfile,"id_pista,id_carro,iteracao\n");

    tAtropelamento_sorting(atropelamentos);
    int i;
    for(i = 0; atropelamentos[i].iteracao; i++){

        fprintf(pfile,"%d,%d,%d\n", atropelamentos[i].pista_id,
                                    atropelamentos[i].carro_id,
                                    atropelamentos[i].iteracao);
    }

    fclose(pfile);
}

void tAtropelamento_sorting(tAtropelamento atropelamentos[]){

    int estaOrganizado = 0;
    int i;
    while(!estaOrganizado){

        estaOrganizado = 1;
        for(i = 1; atropelamentos[i].iteracao; i++){

            tAtropelamento aux;
            if(atropelamentos[i-1].pista_id > atropelamentos[i].pista_id){
                aux = atropelamentos[i];
                atropelamentos[i] = atropelamentos[i-1];
                atropelamentos[i-1] = aux;
                estaOrganizado = 0;

            }else if(atropelamentos[i-1].pista_id == atropelamentos[i].pista_id){

                if(atropelamentos[i-1].carro_id > atropelamentos[i].carro_id){
                    aux = atropelamentos[i];
                    atropelamentos[i] = atropelamentos[i-1];
                    atropelamentos[i-1] = aux;
                    estaOrganizado = 0;

                }else if(atropelamentos[i-1].carro_id == atropelamentos[i].carro_id){
                        if(atropelamentos[i-1].iteracao < atropelamentos[i].iteracao){
                        aux = atropelamentos[i];
                        atropelamentos[i] = atropelamentos[i-1];
                        atropelamentos[i-1] = aux;
                        estaOrganizado = 0;
                    }
                }
            }
        }
    }
}

void tJogo_InicializaHeatMap(int heatmap[][100], tMapa mapa){

    int alturaMax = CalculaAlturaSemBorda(mapa.qtdPistas);
    int i;
    // altura -  1 retira a borda da galinha
    for(i = 0; i < alturaMax - 1; i++){

        int j;
        for(j = 0; j < mapa.largura; j++){

            heatmap[i][j] = 0;
        }
    }
}

void CriaHeatmapFile(int heatmap[][100], tMapa mapa, char *argv[]){

    char diretorio[1020];
    sprintf(diretorio,"%s/saida/heatmap.txt",argv[1]);
    FILE *pfile = fopen(diretorio,"w");
    if(pfile == NULL){
        printf("Nao foi possivel criar heatmap no diretorio de saida\n");
        fclose(pfile);
    }

    int alturaMax = CalculaAlturaSemBorda(mapa.qtdPistas);
    int i;
    // altura -  1 retira a borda da galinha
    for(i = 0; i < alturaMax - 1; i++){
        
        int j;
        for(j = 0; j < mapa.largura; j++){
            if(heatmap[i][j] != -1){
                fprintf(pfile,"%2d ",heatmap[i][j]); 

            }else{
                fprintf(pfile," * ");
            }
        }
        fprintf(pfile,"\n");
    }

    fclose(pfile);
}

void tJogo_RegistroNormalHeatmap(int heatmap[][100], tGalinha galinha){

    int centro_x = galinha.posicao_x - 1;
    int esquerda_x = (centro_x - 1);
    int direita_x = (centro_x + 1);

    int centro_y = galinha.posicao_y - 1;

    int i;
    // i deve iteirar sobre as duas linhas 
    for(i = centro_y; i <= centro_y + 1; i++){

        int j;
        for(j = esquerda_x; j <= direita_x; j++){

            if(heatmap[i][j] != -1){
                heatmap[i][j]++;
            }
        }
    }
}

void tJogo_RegistroAtropelamentoHeatmap(int heatmap[][100], tGalinha galinha, tMapa mapa){

    int centro_x = galinha.posicao_x - 1;
    int esquerda_x = (centro_x - 1);
    int direita_x = (centro_x + 1);

    int centro_y = galinha.posicao_y - 1;

    int i;
    for(i = centro_y; i <= (centro_y + 1); i++){

        int j;
        for(j = 0; j < mapa.largura; j++){
            heatmap[i][j] = -1;
        }
    }

    /*
    heatmap[centro_y][esquerda_x] = -1;
    heatmap[centro_y][centro_x] = -1;
    heatmap[centro_y][direita_x]  = -1;

    heatmap[centro_y + 1][esquerda_x] = -1;
    heatmap[centro_y + 1][centro_x]  = -1;
    heatmap[centro_y + 1][direita_x]  = -1;*/
}