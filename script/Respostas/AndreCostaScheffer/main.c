#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    char galinha[8];
    char carro[25];
}tSkin;
tSkin LeSkins(char *argv[]);

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
tGalinha tGalinha_inicializa(tGalinha galinha,int pistaGalinha_id, FILE *pFile);
tGalinha tGalinha_reseta(tGalinha galinha);
tGalinha tGalinha_AtualizaPosicao(tGalinha galinha, int novaPosicao_x);
tGalinha tGalinha_Move(tGalinha galinha, int heatmap[][100], char respostaUsuario);
tGalinha tGalinha_AtualizaPontuacaoEAltura(tGalinha galinha);
tGalinha tGalinha_AtualizaPontuacao(tGalinha galinha, int qtdGanha);

typedef struct{
    int id;
    int posicao_x;
}tCarro;
void tCarro_Inicializa(tCarro carros[], int qtdCarros, FILE *pFile);
void tCarro_AtualizaPosicoes(   tCarro Carros[], int qtdCarros, char direcao, 
                                int velocidade, int limitePista);

typedef struct{
    int id;
    int centro_y;
    int qtdCarros;
    tCarro carros[10];
    int velocidade;
    char direcao;
}tPista;
void tPista_Inicializa(tPista pistas[], int qtdPistas, FILE *pFile);
void tPista_Atualiza(tPista pistas[], int qtdPistas, int largura);
tPista tPista_ReduzVelocidade(tPista pista, int ehAnimado);

typedef struct{
    int iteracao;
    int pista_id;
    int carro_id;
    int galinha_x;
    int galinha_y;
}tAtropelamento;
void tAtropelamento_InicializaIteracao(tAtropelamento atropelamentos[]);
void tAtropelamento_Registra( tAtropelamento atropelamentos[], tPista pista, 
                            tGalinha galinha, int carro_id,int iteracao );
void tAtropelamento_sorting(tAtropelamento atropelamentos[]);

typedef struct{
    int largura;
    int altura;
    int qtdPistas;
    tPista pistas[13];
    // 100 colunas de espaço, 2 para bordas (0 e 101), e 1 \0
    char desenhoMapa[35][102];
}tMapa;
tMapa tMapa_Inicializa(tMapa mapa, FILE *pFile);
tMapa tMapa_Atualiza(tMapa mapa);
tMapa tMapa_ReduzVelocidadePista(tMapa mapa, int indicePista, int ehAnimado);

typedef struct{
    tSkin skin;
    int ehAnimado;
    tGalinha galinha;
    tMapa mapa;
    int iteracao;
    int qtdAtropelamentos;
    tAtropelamento atropelamentos[100];
    int heatmap[35][100];
}tJogo;
tJogo tJogo_LeConfiguracoes(tJogo jogo, char *argv[]);
tJogo tJogo_Inicializa(char *argv[]);
tJogo tJogo_Executa(tJogo jogo, char *argv[]);
tJogo tJogo_AtualizaEntidades(tJogo jogo, char inputUsuario);
tJogo tJogo_Termina(tJogo jogo, int condicoesFim, char *argv[]);
tJogo tJogo_ProcessaAtropelamento(tJogo jogo);
char tJogo_TerminalInput(tJogo jogo);

void tJogo_InicializaHeatMap(int heatmap[][100], tMapa mapa);
void tJogo_RegistroNormalHeatmap(int heatmap[][100], tGalinha galinha);
void tJogo_RegistroAtropelamentoHeatmap(int heatmap[][100], tGalinha galinha, tMapa mapa);
void CriaHeatmapFile(int heatmap[][100], tMapa mapa, char *argv[]);

//=============//
// UTILITÁRIAS //
//=============//

// Genericas

int CalculaPosicao_y(int ordemPistaCimaBaixo);
int CalculaAlturaSemBorda(int qtdPistas);
int InverteAltura(int altura, int alturaTotal);

// Galinha

int tGalinha_Posicao_y(tGalinha galinha);
int tGalinha_Pontuacao(tGalinha galinha);
int tGalinha_Vidas(tGalinha galinha);

// Carro

int tCarro_Posicao_x(tCarro carro);
int tCarro_id(tCarro carro);

// Pista

int tPista_CentroPosicao_y(tPista pista);

// Atropelamento

int tAtropelamento_MinimaAltura(tAtropelamento atropelamentos[], int alturaMapa);
int tAtropelamento_MaxAltura(tAtropelamento atropelamentos[], int alturaMapa);
int tAtropelamento_EstaColidindo(tPista pista, tGalinha galinha);
int tAtropelamento_EhAtropValido(tAtropelamento atropelamento);

// Mapa

tPista tMapa_RetornaPista(tMapa mapa, int indicePista);
int tMapa_VerificaPistaColisao(tMapa mapa, int indicePista, tGalinha galinha);
int tMapa_CentroPista_y(tMapa mapa, int pistaIndice);
int tMapa_qtdPistas(tMapa mapa);

// Jogo
int tJogo_EhVitoria(tMapa mapa, tGalinha galinha);
int tJogo_FimDeJogo(tJogo jogo);

// Impressao

int InicioSkinAnimacao(int iteracao, int ehAnimado);
void DesenhaQualquerEntidade(   char desenhoMapa[][102], int centro_x, int centro_y, 
                                int larguraMapa, char skin[], int inicioSkin);
void DesenhaGalinha(char desenhoMapa[][102], int larguraMapa, tGalinha galinha, char skinGalinha[]);
void DesenhaCarros( char desenhoMapa[][102], int larguraMapa, tPista pista, 
                    char skinCarro[], int iteracao, int ehAnimado);

tMapa DesenhaCenario(tMapa mapa);
tMapa DesenhaPersonagensMapa(tGalinha galinha, tSkin skins, tMapa mapa, int iteracao, int ehAnimado);
tMapa DesenhaMapa(tJogo jogo);
void ImprimePlacar(tJogo jogo);
void ImprimeMapa(tMapa mapa, FILE *saida);

// Arquivos

void CriaArquivoInicializacao(tMapa mapa, tGalinha galinha, char *argv[]);
void ResumeRodadaArquivo(int iteracao, tAtropelamento atropelamentos[], char *argv[]);
void EncerraResumo(int iteracao, char *argv[]);
void CriaEstatistaFile(tGalinha galinha, tAtropelamento atropelamentos[], tMapa mapa, char *argv[]);
void CriaRankingFile(tAtropelamento atropelamentos[], char *argv[]);

int main(int argc, char *argv[]){

    tJogo jogo = tJogo_Inicializa(argv);

    int fimJogo = tJogo_FimDeJogo(jogo);
    while(!fimJogo){

        jogo = tJogo_Executa(jogo, argv);
        fimJogo = tJogo_FimDeJogo(jogo);
    }

    jogo = tJogo_Termina(jogo, fimJogo, argv);

    return 0;
}

/**
 * Carrega as skins dos personagens
 */
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
    // Le a skin da galinha
    for(i = 0; i < 6; i++){

        fscanf(pfile,"%c",&ch);

        if(ch == '\n'){
            i--;
            continue;
        }

        skin.galinha[i] = ch;
    }
    skin.galinha[6] = '\0';

    // Le as 4 possiveis skins dos carros
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

/**
 * Inicializa a galinha com base no arquivo de configuracao
 */
tGalinha tGalinha_inicializa(tGalinha galinha, int pistaGalinha_id, FILE *pFile){

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

/**
 * Reseta a vida, pontuacao e posicao,
 * galinha comeca do ponto de inicio
 */
tGalinha tGalinha_reseta(tGalinha galinha){
    galinha.vidas--;
    galinha.pontuacao = 0;
    galinha.alturaAnterior = 0;
    galinha.posicao_y = galinha.posicaoInicial_y;
    return galinha;
}

/**
 * Recebe uma nova Posicao e atualiza
 * a posicao atual da galinha
 */
tGalinha tGalinha_AtualizaPosicao(tGalinha galinha, int novaPosicao_y){
    galinha.posicao_y = novaPosicao_y;
    return galinha;
}

/**
 * Processa a entrada do jogador ('w', 's' ou ' ')
 * sendo que, w e s movem a galinha em 3 unidades.
 * 
 * Atualiza os dados do heatmap.
 */
tGalinha tGalinha_Move(tGalinha galinha, int heatmap[][100], char respostaUsuario){

    galinha.alturaAnterior = galinha.posicao_y;

    if(respostaUsuario == 'w'){
        int moveCima  = -3;
        galinha.qtdMovimentoTotal++;
        galinha = tGalinha_AtualizaPosicao(galinha, (galinha.posicao_y + moveCima));

    }else if(respostaUsuario == 's'){
        
        // Nao permite que a galinha se mova para baixo
        // Alem da sua linha inicial
        if(galinha.posicao_y != galinha.posicaoInicial_y){
            int moveBaixo = + 3;
            galinha.qtdMovimentoTotal++;
            galinha.qtdMovimentoBaixo++;
            galinha = tGalinha_AtualizaPosicao(galinha, (galinha.posicao_y + moveBaixo));

        }else if(galinha.posicao_y == galinha.posicaoInicial_y){
            int parada = 0;
            galinha = tGalinha_AtualizaPosicao(galinha, (galinha.posicao_y + parada));
        }

    }else if(respostaUsuario == ' '){
        int parada = 0;
        galinha = tGalinha_AtualizaPosicao(galinha, (galinha.posicao_y + parada));
    }

    tJogo_RegistroNormalHeatmap(heatmap, galinha);
    return galinha;
}

/**
 * Incrementa a pontuacao da galinha caso avance uma pista
 * atualiza altura maxima atingida
 */
tGalinha tGalinha_AtualizaPontuacaoEAltura(tGalinha galinha){

    if(galinha.posicao_y < galinha.alturaAnterior){
        galinha.alturaAnterior = galinha.posicao_y;
        galinha.pontuacao++;
    }

    if(galinha.alturaMaxAtinginda > galinha.posicao_y){
        galinha.alturaMaxAtinginda = galinha.posicao_y;
    }

    return galinha;
}

/**
 * Incrementa a pontuacao da galinha
 * em uma certa quantidade
 */
tGalinha tGalinha_AtualizaPontuacao(tGalinha galinha, int qtdGanha){
    galinha.pontuacao += qtdGanha;  
    return galinha;
}

/**
 * Inicializa uma matriz de carros
 * Baseado no arquivo de configuracao inicial
 */
void tCarro_Inicializa(tCarro carros[], int qtdCarros, FILE *pFile){

    int i;
    for(i = 0; i < qtdCarros; i++){
        // primeiro id corresponde a 1
        // nao a zero
        carros[i].id = (i+1);
        fscanf(pFile, "%d",&carros[i].posicao_x);
    }

    // Garante que o caracter '\n' sera consumido
    // no final da leitura, mesmo que haja espaco antes
    char ch;
    fscanf(pFile,"%c",&ch);
    if(ch == ' ')
    fgetc(pFile);
}

/**
 * Atualiza a posicao de todos os carros dentro de uma matriz.
 * Verifica e corrigi caso estejam fora do mapa
 */
void tCarro_AtualizaPosicoes(tCarro Carros[], int qtdCarros, char direcao, int velocidade, int limitePista){

    int i;
    for(i = 0; i < qtdCarros; i++){

        int deslocamento;

        // Verifica o sentido da pista.
        // Garante que a posicao dos carros 
        // nao saia do limite da pista.
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

/**
 * Inicializa cada pista de uma array com base no arquivo de configuracao inicial
 */
void tPista_Inicializa(tPista pistas[], int qtdPistas, FILE *pFile){

    int i;
    // itera sobre todas as linhas exceto a da galinha
    int tirandoGalinha = qtdPistas-1;
    for(i = 0; i < tirandoGalinha; i++){
        char tipoPista;
        fscanf(pFile,"%c",&tipoPista);

        // caso seja '\n' (pista vazia)
        if(tipoPista == '\n'){
            pistas[i].id = (i+1);
            pistas[i].qtdCarros = 0;
            pistas[i].velocidade = 0;
            pistas[i].direcao = '0';
            pistas[i].centro_y = i*3 + 1;
            continue;
        }

        // Caso seja uma pista com carros
        pistas[i].id = (i+1);
        fscanf(pFile,"%d %d",   &pistas[i].velocidade,
                                &pistas[i].qtdCarros);
        pistas[i].direcao = tipoPista;

        // calcula a altura y do centro da pista
        pistas[i].centro_y = i*3 + 1;

        tCarro_Inicializa(pistas[i].carros, pistas[i].qtdCarros, pFile);
    }

    // Fora do loop,
    // Inicializa a ultima pista (galinha)
    pistas[i].id = (i+1);
    pistas[i].qtdCarros = 0;
    pistas[i].velocidade = 0;
    pistas[i].direcao = '0';
    pistas[i].centro_y = i*3 + 1;
}

/**
 * Atualiza a posicao dos carros de cada pista dentro de uma array
 */
void tPista_Atualiza(tPista pistas[], int qtdPistas, int largura){

    int i;
    for(i = 0; i < qtdPistas; i++){
        
        if(pistas[i].qtdCarros){
            int qtdCarros = pistas[i].qtdCarros;
            int velocidade = pistas[i].velocidade;
            tCarro_AtualizaPosicoes(pistas[i].carros, qtdCarros, pistas[i].direcao, velocidade, largura);
        }
    }
}

/**
 * Reduz a velocidade de uma pista em 1 unidade.
 * Velocidade minima eh 1.
 * Funcao para caso bonus
 */
tPista tPista_ReduzVelocidade(tPista pista, int ehAnimado){

    if(ehAnimado && (pista.velocidade > 1)){
        pista.velocidade--;
    }

    return pista;
}

/**
 * Inicializa a Iteracao em zero de cada atropelamento em uma array.
 */
void tAtropelamento_InicializaIteracao(tAtropelamento atropelamentos[]){

    int i;
    for(i = 0; i < 90; i++){
        atropelamentos[i].iteracao = 0;
    }

    // OBS: Caso mude, atualizar a funcao tAtropelamento_EhAtropValido(...)
}

/**
 * Atualiza uma array de atropelamentos.
 */
void tAtropelamento_Registra( tAtropelamento atropelamentos[], tPista pista, 
                            tGalinha galinha, int carro_id,int iteracao){

    int i;
    // Defini um limite maximo de atropelamento
    // que o jogo pode registrar
    int limiteAtrop = 90;
    for(i = 0; i < limiteAtrop; i++){
        
        // Procura um slot vazio para registrar.
        if(!tAtropelamento_EhAtropValido(atropelamentos[i])){

            atropelamentos[i].iteracao = iteracao;
            atropelamentos[i].pista_id = pista.id;
            atropelamentos[i].carro_id = carro_id;
            atropelamentos[i].galinha_x = galinha.posicao_x;
            atropelamentos[i].galinha_y = galinha.posicao_y;
            break;
        }
    }
}

/**
 * Ordena uma array de atropelamentos, ordem de preferencia:
 * 1. ID da pista, ordem crescente.
 * 2. ID do carro, ordem crescente.
 * 3. Iteracao, ordem decrescente
 */
void tAtropelamento_sorting(tAtropelamento atropelamentos[]){

    int estaOrganizado = 0;
    int i;
    while(!estaOrganizado){

        estaOrganizado = 1;
        // Esse for() Compara elementos adjacentes. 
        for(i = 1; tAtropelamento_EhAtropValido(atropelamentos[i]); i++){

            tAtropelamento aux;
            // Ordena pelo Id da pista, crescente
            if(atropelamentos[i-1].pista_id > atropelamentos[i].pista_id){
                aux = atropelamentos[i];
                atropelamentos[i] = atropelamentos[i-1];
                atropelamentos[i-1] = aux;
                estaOrganizado = 0;

            }else if(atropelamentos[i-1].pista_id == atropelamentos[i].pista_id){

                // Ordena pelo Id do carro, crescente
                if(atropelamentos[i-1].carro_id > atropelamentos[i].carro_id){
                    aux = atropelamentos[i];
                    atropelamentos[i] = atropelamentos[i-1];
                    atropelamentos[i-1] = aux;
                    estaOrganizado = 0;

                }else if(atropelamentos[i-1].carro_id == atropelamentos[i].carro_id){

                        // Ordena pela Iteracao, descrescente
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

/**
 * Inicializa o mapa com base no arquivo de configuracao inicial.
 * Chama funcao auxiliar para inicializar pistas
 */
tMapa tMapa_Inicializa(tMapa mapa, FILE *pFile){

    fscanf(pFile, "%d %d",&mapa.largura,&mapa.qtdPistas);
    // Consome o caractere '\n' para evitar erros
    // Na proxima leitura
    fgetc(pFile);

    mapa.altura = CalculaAlturaSemBorda(mapa.qtdPistas);
    tPista_Inicializa(mapa.pistas, mapa.qtdPistas, pFile);
    return mapa;
}

/**
 * Atualiza as pistas de uma mapa
 */
tMapa tMapa_Atualiza(tMapa mapa){
    tPista_Atualiza(mapa.pistas, mapa.qtdPistas, mapa.largura);
    return mapa;
}

/**
 * Atualiza a pista no mapa onde ocorreu o atropelamento.
 * Funcao ativa para casos bonus.
 */
tMapa tMapa_ReduzVelocidadePista(tMapa mapa, int indicePista, int ehAnimado){
    mapa.pistas[indicePista] = tPista_ReduzVelocidade(mapa.pistas[indicePista], ehAnimado);
    return mapa;
}

/**
 * Responsavel pela leitura do arquivo de configuracao 
 * e chamada de funcoes auxiliares
 */
tJogo tJogo_LeConfiguracoes(tJogo jogo, char *argv[]){

    char diretorio[1020];
    sprintf(diretorio,"%s/config_inicial.txt",argv[1]);

    FILE *pFile = fopen(diretorio,"r");
    if(pFile == NULL){
        printf("ERRO: Informe o argv com os arquivos de configuracao.\n");
        exit(1);
    }

    fscanf(pFile,"%d",&jogo.ehAnimado);
    jogo.mapa = tMapa_Inicializa(jogo.mapa, pFile);
    jogo.galinha = tGalinha_inicializa(jogo.galinha, tMapa_qtdPistas(jogo.mapa), pFile);

    fclose(pFile);

    return jogo;
}

/**
 * Cria a estrutura jogo e inicializa tudo
 */
tJogo tJogo_Inicializa(char *argv[]){
    tJogo jogo;
    // Carrega dados e configuracoes
    jogo = tJogo_LeConfiguracoes(jogo, argv);
    jogo.skin = LeSkins(argv);
    tAtropelamento_InicializaIteracao(jogo.atropelamentos);
    tJogo_InicializaHeatMap(jogo.heatmap,jogo.mapa);
    tJogo_RegistroNormalHeatmap(jogo.heatmap, jogo.galinha);

    // Gera saidas
    jogo.mapa = DesenhaMapa(jogo);
    CriaArquivoInicializacao(jogo.mapa, jogo.galinha, argv);
    ImprimePlacar(jogo);
    ImprimeMapa(jogo.mapa,stdout);
    return jogo;
}

/**
 * Executa um turno do jogo
 */
tJogo tJogo_Executa(tJogo jogo, char *argv[]){

    char userResposta = tJogo_TerminalInput(jogo);
    if(userResposta != '0'){

        jogo.iteracao++;
        jogo = tJogo_AtualizaEntidades(jogo, userResposta);
        jogo = tJogo_ProcessaAtropelamento(jogo);

        jogo.mapa = DesenhaMapa(jogo);

        if(!tJogo_FimDeJogo(jogo)){
            ImprimePlacar(jogo);
            ImprimeMapa(jogo.mapa,stdout);  
        }

        // Atualiza o arquivo de resumo.
        ResumeRodadaArquivo(jogo.iteracao, jogo.atropelamentos, argv);
    }
    return jogo;
}

/**
 * Atualiza as entidades galinha e carros
 */
tJogo tJogo_AtualizaEntidades(tJogo jogo, char inputUsuario){
    jogo.galinha = tGalinha_Move(jogo.galinha, jogo.heatmap, inputUsuario);
    jogo.mapa = tMapa_Atualiza(jogo.mapa);
    return jogo;
}

/**
 * Coordena finalizacao do jogo
 */
tJogo tJogo_Termina(tJogo jogo, int condicoesFim, char *argv[]){

    // Atualiza a pontuacao da galinha se vencer
    if(condicoesFim == 1){
        int qtdGanha = 10;
        jogo.galinha = tGalinha_AtualizaPontuacao(jogo.galinha, qtdGanha);
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

/**
 * Coordena a Rotina de atropelamento
 * 
 * Verifica colisao, atualiza uma array de atropelamentos
 * Atualiza Heatmap e reseta a galinha.
 */
tJogo tJogo_ProcessaAtropelamento(tJogo jogo){

    int i;
    int qtdPistas = tMapa_qtdPistas(jogo.mapa);
    for(i = 0; i < qtdPistas; i++){

        int indicePistaMatrix = i;
        int pistaPos_y = tMapa_CentroPista_y(jogo.mapa, indicePistaMatrix);
        int galinhaPos_y = tGalinha_Posicao_y(jogo.galinha);
        if(pistaPos_y == galinhaPos_y){
            
            int carro_id = tMapa_VerificaPistaColisao(jogo.mapa, indicePistaMatrix, jogo.galinha);
            if(carro_id){

                tPista pista = tMapa_RetornaPista(jogo.mapa, indicePistaMatrix);

                tJogo_RegistroAtropelamentoHeatmap(jogo.heatmap, jogo.galinha, jogo.mapa);
                tAtropelamento_Registra(jogo.atropelamentos, pista, jogo.galinha, carro_id, jogo.iteracao);
                jogo.mapa = tMapa_ReduzVelocidadePista(jogo.mapa, indicePistaMatrix, jogo.ehAnimado);

                jogo.galinha = tGalinha_reseta(jogo.galinha);
                tJogo_RegistroNormalHeatmap(jogo.heatmap, jogo.galinha);

                return jogo;
            }else{

                jogo.galinha = tGalinha_AtualizaPontuacaoEAltura(jogo.galinha);

                return jogo;
            }
        }
    }
    return jogo;
}
/**
 * Le stdin
 * Retorna 'w','s' ou ' ' caso leia com sucesso.
 * Retorna 0 para caracter invalido.
 */
char tJogo_TerminalInput(tJogo jogo){
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

/**
 * Inicializa o heatmap preenchendo 
 * todas as areas jogaveis com 0
 */
void tJogo_InicializaHeatMap(int heatmap[][100], tMapa mapa){

    int alturaMax = CalculaAlturaSemBorda(mapa.qtdPistas);
    int i;
    // OBS: (altura - 1) retira a borda da pista da galinha
    // pois nao eh area jogavel
    for(i = 0; i < alturaMax - 1; i++){
        int j;
        for(j = 0; j < mapa.largura; j++){
            heatmap[i][j] = 0;
        }
    }
}

/**
 * Atualiza o heatmap
 */
void tJogo_RegistroNormalHeatmap(int heatmap[][100], tGalinha galinha){

    int centro_x = galinha.posicao_x - 1;
    int esquerda_x = (centro_x - 1);
    int direita_x = (centro_x + 1);

    int centro_y = galinha.posicao_y - 1;

    int i;
    // O laco externo itera sobre as 2 linhas de altura da galinha 
    for(i = centro_y; i <= centro_y + 1; i++){

        int j;
        for(j = esquerda_x; j <= direita_x; j++){

            // Condicao para nao alterar que registrou um atropelamento
            if(heatmap[i][j] != -1){
                heatmap[i][j]++;

                // Garante que o valor nao passe de 99
                if(heatmap[i][j] > 99){
                    heatmap[i][j] = 99;
                }
            }
        }
    }
}

/**
 * Registra a pista em que houve colisao no heatmap.
 */
void tJogo_RegistroAtropelamentoHeatmap(int heatmap[][100], tGalinha galinha, tMapa mapa){
    
    int centro_y = galinha.posicao_y - 1;
    int i;
    for(i = centro_y; i <= (centro_y + 1); i++){

        int j;
        // Percorre todas as colunas de uma pista
        // Para registrar uma colisao
        for(j = 0; j < mapa.largura; j++){
            heatmap[i][j] = -1;
        }
    }
}

/**
 * Gera o arquivo heatmap.txt
 */
void CriaHeatmapFile(int heatmap[][100], tMapa mapa, char *argv[]){

    char diretorio[1020];
    sprintf(diretorio,"%s/saida/heatmap.txt",argv[1]);
    FILE *pfile = fopen(diretorio,"w");
    if(pfile == NULL){
        printf("Nao foi possivel criar heatmap no diretorio de saida\n");
        return;
    }

    int alturaMax = CalculaAlturaSemBorda(mapa.qtdPistas);
    int i;
    // OBS: (altura - 1) retira a borda da pista da galinha
    // pois nao eh area jogavel
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

/**
 * Converte o indice de uma pista para a coordenada y
 */
int CalculaPosicao_y(int ordemPistaCimaBaixo){
    // Encontra a posicao da ultima linha da pista
    // Subtrai 2 para chegar ao centro.
    int posicao_y = ordemPistaCimaBaixo*3 - 2;
    return posicao_y;
}

/**
 * Calcula a altura total da area jogavel do mapa.
 * Desconsidera bordas.
 */
int CalculaAlturaSemBorda(int qtdPistas){
    int altura;
    // 2 linhas em branco + 1 linha padrao, por pista
    altura = qtdPistas*2 + qtdPistas;
    return altura;
}

/**
 * Ajusta a altura para que se escale conforma
 * se aproxima do fim do jogo.
 */
int InverteAltura(int altura, int alturaTotal){
    altura = alturaTotal - altura;
    return altura;
}

/**
 * Retorna posicao y da galinha
 */
int tGalinha_Posicao_y(tGalinha galinha){
    return galinha.posicao_y;
}

/**
 * Retorna pontuacao da galinha
 */
int tGalinha_Pontuacao(tGalinha galinha){
    return galinha.pontuacao;
}

/**
 * Retorna vidas restantes da galinha
 */
int tGalinha_Vidas(tGalinha galinha){
    return galinha.vidas;
}

/**
 * Retorna a posicao x de um carro
 */
int tCarro_Posicao_x(tCarro carro){
    return carro.posicao_x;
}

/**
 * Retorna o id de um carro
 */
int tCarro_id(tCarro carro){
    return carro.id;
}

/**
 * Retorna o centro y de uma pista
 */
int tPista_CentroPosicao_y(tPista pista){
    return pista.centro_y;
}

/**
 * Retorna a menor altura registrada de um atropelamento em uma array.
 * Altura ja convertida.
 */
int tAtropelamento_MinimaAltura(tAtropelamento atropelamentos[], int alturaMapa){

    int alturaMinima = 100; 
    int i;
    for(i = 0; tAtropelamento_EhAtropValido(atropelamentos[i]); i++){

        int alturaAtual = InverteAltura(atropelamentos[i].galinha_y, alturaMapa);
        if(alturaMinima > alturaAtual){
            alturaMinima = alturaAtual;
        }
    }

    if(alturaMinima == 100){
        return 0;
    }

    return alturaMinima;
}

/**
 * Retorna a maior altura registrada de um atropelamento em uma array
 * Altura ja convertida.
 */
int tAtropelamento_MaxAltura(tAtropelamento atropelamentos[], int alturaMapa){

    int alturaMax = 0; 
    int i;
    for(i = 0; tAtropelamento_EhAtropValido(atropelamentos[i]); i++){

        int alturaAtual = InverteAltura(atropelamentos[i].galinha_y, alturaMapa);
        if(alturaMax < alturaAtual){
            alturaMax = alturaAtual;
        }
    }
 
    return alturaMax;
}

/**
 * Verifica se a galinha esta colidindo com algum dos carros.
 * Verifica se a area da galinha se sobrepoe a de algum carro.
 * 
 * Retorna o id do carro que causou a colisao (id_valido > 0).
 * Retorna 0 caso nao haja colisao
 */
int tAtropelamento_EstaColidindo(tPista pista, tGalinha galinha){

    // Se nao a carros, nao ha o que analizar
    if(!pista.qtdCarros)
    return 0;

    int galinhaEsquerda = galinha.posicao_x - 1;
    int galinhaDireita = galinha.posicao_x + 1;

    int i;
    for(i = 0; i < pista.qtdCarros; i++){

        int carro_x = tCarro_Posicao_x(pista.carros[i]);
        int carroEsquerda = carro_x - 1;
        int carroDireita = carro_x + 1;

        // caso 1, o centro da galinha esta dentro da area do carro,
        // caso 2, a borda direita da galinha toca a esquerda do carro,
        // caso 3, a borda esquerda da galinha toca a esquerda do carro
        if((galinha.posicao_x >= carroEsquerda && galinha.posicao_x <= carroDireita) ||
            galinhaDireita == carroEsquerda || galinhaEsquerda == carroDireita){

            int carro_id = tCarro_id(pista.carros[i]);
            // retorna o id do carro que causou a colisao
            return carro_id;
        }
    }

    return 0;
}

/**
 * Verifica se um registro de atropelamento eh valido.
 * Baseia-se que um atropelamento nunca ocorrera na iteracao 0.
 */
int tAtropelamento_EhAtropValido(tAtropelamento atropelamento){
    if(atropelamento.iteracao){
        return 1;
    }
    return 0;
}

/**
 * Retorna a pista procurada em um mapa
 */
tPista tMapa_RetornaPista(tMapa mapa, int indicePista){
    return mapa.pistas[indicePista];
}

/**
 * Verifica se ocorre colisao em uma pista especifica no mapa
 */
int tMapa_VerificaPistaColisao(tMapa mapa, int indicePista, tGalinha galinha){

    int tAtropelamento_estaColidindo = tAtropelamento_EstaColidindo(mapa.pistas[indicePista], galinha);
    return tAtropelamento_estaColidindo;
}

/**
 * Retorna o centro de uma determinada pista no mapa
 */
int tMapa_CentroPista_y(tMapa mapa, int pistaIndice){

    int centro_y = tPista_CentroPosicao_y(mapa.pistas[pistaIndice]);
    return centro_y;
}

/**
 * Retorna a quantidade de pistas presentes no mapa
 */
int tMapa_qtdPistas(tMapa mapa){
    return mapa.qtdPistas;
}

/**
 * Verifica se a galinha esta na ultima pista,
 * caso esteja retorna 1 (vitoria)
 * retorna 0 posicao diferente
 */
int tJogo_EhVitoria(tMapa mapa, tGalinha galinha){
    int ultimaPista_y = CalculaPosicao_y(1);
    if(ultimaPista_y == galinha.posicao_y)
    return 1;
    return 0;
}

/**
 * Retorna 0 - jogo nao terminado
 * Retorna 1 - vitoria
 * Retorna 2 - sem vidas
 */
int tJogo_FimDeJogo(tJogo jogo){
    if(tJogo_EhVitoria(jogo.mapa, jogo.galinha)){
        return 1;
    }
    if(!tGalinha_Vidas(jogo.galinha)){
        return 2;
    }
    return 0;
}

/**
 * Implementa a logica da animacao bonus.
 * Caso bonus esteja desativado, retornara 0.
 */
int InicioSkinAnimacao(int iteracao, int ehAnimado){
    int skinInicio = 0;
    if(ehAnimado){
        // Seleciona 1 dentre os 4 tipos de animacao
        int skin_id = iteracao%4;
        
        // Cada skin de carro carro ocupa 6 caracteres
        skinInicio = skin_id * 6; 
    }
    return skinInicio;
}

void DesenhaQualquerEntidade(   char desenhoMapa[][102], int centro_x, int centro_y, 
                                int larguraMapa, char skin[], int incioSkin){
    // informa onde a skin a ser lida comeca
    int j = incioSkin;

    int centroMatriz_x = centro_x - 1;
    int cabecaCentro_y = centro_y - 1;

    int esquerda_x = centroMatriz_x - 1;
    int direita_x = centroMatriz_x + 1;

    // garante que se a entidade estiver pacialmente fora da tela,
    // a parte de fora sera desenhada no lado oposto
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

void DesenhaGalinha(char desenhoMapa[][102], int larguraMapa, tGalinha galinha, char skinGalinha[]){
    int inicioSkin = 0;
    DesenhaQualquerEntidade(desenhoMapa, galinha.posicao_x, galinha.posicao_y, larguraMapa, skinGalinha, inicioSkin);
}

void DesenhaCarros(char desenhoMapa[][102], int larguraMapa, tPista pista, char skinCarro[], int iteracao, int ehAnimado){
    int i;
    for(i = 0; i < pista.qtdCarros; i++){
        int posicao_x = tCarro_Posicao_x(pista.carros[i]);
        int posicao_y = pista.centro_y;
        int inicioSkin = InicioSkinAnimacao(iteracao, ehAnimado);
        DesenhaQualquerEntidade(desenhoMapa, posicao_x, posicao_y, larguraMapa, skinCarro, inicioSkin);
    }
}

/**
 * Desenha o layout base do jogo
 */
tMapa DesenhaCenario(tMapa mapa){

    int i;
    //desenha as pistas exceto a da galinha
    for(i = 0; i < mapa.qtdPistas - 1; i++){

        // calcula a posicao y da pista atual
        int altura = CalculaAlturaSemBorda(i+1);
        int j;

        // itera sobre as 3 linhas da pista
        for(j = (altura - 3); j < altura; j++){

            //desenha 2 linhas vazias
            if(j <= (altura - 2)){
                int k;
                for(k = 0; k < mapa.largura; k++){
                    mapa.desenhoMapa[j][k] = ' ';
                }

            //desenha 1 linha padrao na ultima linha da pista atual
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
tMapa DesenhaPersonagensMapa(tGalinha galinha, tSkin skins, tMapa mapa, int iteracao, int ehAnimado){

    int i;
    for(i = 0; i < mapa.qtdPistas; i++){
        
        int posicaoPista_y = tPista_CentroPosicao_y(mapa.pistas[i]);
        // verifica se a pista da iteracao atual contem a galinha
        if(posicaoPista_y == galinha.posicao_y){
            DesenhaGalinha(mapa.desenhoMapa, mapa.largura, galinha, skins.galinha);
        }

        DesenhaCarros(mapa.desenhoMapa, mapa.largura, mapa.pistas[i], skins.carro, iteracao, ehAnimado);
    }
    return mapa;
}
/**
 * Desenha elementos na array de mapa
 *
 * 1 chamar desenhaMapa
 * 2 chamar ImprimeMapa
 */
tMapa DesenhaMapa(tJogo jogo){
    jogo.mapa = DesenhaCenario(jogo.mapa);
    jogo.mapa = DesenhaPersonagensMapa(jogo.galinha, jogo.skin, jogo.mapa, jogo.iteracao, jogo.ehAnimado);
    return jogo.mapa;
}
void ImprimePlacar(tJogo jogo){
    int pontos = tGalinha_Pontuacao(jogo.galinha);
    int vidas = tGalinha_Vidas(jogo.galinha);
    printf("Pontos: %d | Vidas: %d | Iteracoes: %d\n",pontos, vidas, jogo.iteracao);
}

/**
 * Imprime o mapa com as bordas,
 * Em um destino de saida.
 * 
 * stdout - terminal
 * ou informe um diretorio valido
 */
void ImprimeMapa(tMapa mapa, FILE *saida){
    
    // imprime borda superior
    fprintf(saida,"|");
    int i;
    for(i = 0; i < mapa.largura; i ++){
        fprintf(saida,"-");
    }
    fprintf(saida,"|\n");

    // imprime conteudo do mapa
    for(i = 0; i < mapa.altura-1; i++){

        if(i < (mapa.altura-1)) 
        fprintf(saida,"|");
        
        int j;
        for(j = 0; j < mapa.largura; j++){

            fprintf(saida,"%c",mapa.desenhoMapa[i][j]);
        }
        if(i < (mapa.altura-1)) 
        fprintf(saida,"|");

        // Adiciona uma quebra de linha, exceto apos a ultima linha
        if(i < (mapa.altura-2)) 
        fprintf(saida,"\n");
    }

    // Imprime a borda Inferior do mapa
    fprintf(saida,"\n|");
    for(i = 0; i < mapa.largura; i++){
        fprintf(saida,"-");
    }
    fprintf(saida,"|");

    // Se a saida for o terminal (stdout)
    // adiciona uma quebra de linha
    if(saida == stdout)
        printf("\n");
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

void ResumeRodadaArquivo(int iteracao, tAtropelamento atropelamentos[], char *argv[]){

    int teveAtropelamento = 0;
    int i;
    for(i = 0; tAtropelamento_EhAtropValido(atropelamentos[i]); i++){

        if(atropelamentos[i].iteracao == iteracao){
            teveAtropelamento = 1;
            break;
        }
    }

    if(teveAtropelamento){

        char diretorio[1020];
        sprintf(diretorio,"%s/saida/resumo.txt",argv[1]);

        FILE *pFile = fopen(diretorio,"a");
        if(pFile == NULL){
            printf("Nao foi possivel criar o arquivo resumo.txt na pasta saida\n");
            return;
        }
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
    if(pFile == NULL){
        printf("Nao foi possivel criar o arquivo resumo.txt na pasta saida\n");
        return;
    }
    fprintf(pFile, "[%d] Fim de jogo", iteracao);
    fclose(pFile);
}

void CriaEstatistaFile(tGalinha galinha, tAtropelamento atropelamentos[], tMapa mapa, char *argv[]){

    char diretorio[1020];
    sprintf(diretorio, "%s/saida/estatistica.txt", argv[1]);

    FILE *pFile = fopen(diretorio,"w");
    if(pFile == NULL){
        printf("ERRO: nao foi possivel gerar arquivo de estatistica na pasta saida\n");
        return;
    }

    int alturaMaxAting = InverteAltura(galinha.alturaMaxAtinginda, mapa.altura);
    int alturaMaxAtrop = tAtropelamento_MaxAltura(atropelamentos, mapa.altura);
    int alturaMinAtrop = tAtropelamento_MinimaAltura(atropelamentos, mapa.altura);

    fprintf(pFile,"Numero total de movimentos: %d\n", galinha.qtdMovimentoTotal);
    fprintf(pFile,"Altura maxima que a galinha chegou: %d\n", alturaMaxAting);
    fprintf(pFile,"Altura maxima que a galinha foi atropelada: %d\n", alturaMaxAtrop);
    fprintf(pFile,"Altura minima que a galinha foi atropelada: %d\n", alturaMinAtrop);
    fprintf(pFile,"Numero de movimentos na direcao oposta: %d\n", galinha.qtdMovimentoBaixo);

    fclose(pFile);
}

void CriaRankingFile(tAtropelamento atropelamentos[], char *argv[]){

    char diretorio[1020];
    sprintf(diretorio,"%s/saida/ranking.txt",argv[1]);

    FILE *pfile = fopen(diretorio,"w");
    if(pfile == NULL){
        printf("Nao foi possivel criar o arquivo ranking.txt na pasta saida\n");
        return;
    }
    fprintf(pfile,"id_pista,id_carro,iteracao\n");

    tAtropelamento_sorting(atropelamentos);
    int i;
    for(i = 0; tAtropelamento_EhAtropValido(atropelamentos[i]); i++){

        fprintf(pfile,"%d,%d,%d\n", atropelamentos[i].pista_id,
                                    atropelamentos[i].carro_id,
                                    atropelamentos[i].iteracao);
    }

    fclose(pfile);
}