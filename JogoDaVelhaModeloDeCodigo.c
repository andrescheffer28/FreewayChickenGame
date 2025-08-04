#include <stdio.h>
#include <stdlib.h>

//Tabuleiro
#define TAM_TABULEIRO 3
#define PECA_1 1
#define PECA_2 2
typedef struct{
    char posicoes[TAM_TABULEIRO][TAM_TABULEIRO];
    char peca1;
    char peca2;
    char pecaVazio;
} tTabuleiro;

tTabuleiro CriaTabuleiro();

tTabuleiro MarcaPosicaoTabuleiro(tTabuleiro tabuleiro, int peca, int x, int y);

int TemPosicaoLivreTabuleiro(tTabuleiro tabuleiro);

int EstaMarcadaPosicaoPecaTabuleiro(tTabuleiro tabuleiro, int x, int y, int peca);

int EstaLivrePosicaoTabuleiro(tTabuleiro tabuleiro, int x, int y);

int EhPosicaoValidaTabuleiro(tTabuleiro tabuleiro, int x, int y);

int ImprimeTabuleiro(tTabuleiro tabuleiro);




//Jogador
#define ID_JOGADOR_1    1
#define ID_JOGADOR_2    2
typedef struct{
    int id;
 //   char nome[1000]; //Para dar nome ao jogador    
} tJogador;

tJogador CriaJogador(int idJogador);

tTabuleiro JogaJogador(tJogador jogador, tTabuleiro tabuleiro);

int VenceuJogador(tJogador jogador, tTabuleiro tabuleiro);




//Jogada
typedef struct{
    int x;
    int y;
    int sucesso;
} tJogada;

tJogada LeJogada();

int ObtemJogadaX(tJogada jogada);

int ObtemJogadaY(tJogada jogada);

int FoiJogadaBemSucedida(tJogada jogada);




//Jogo
typedef struct{
    tTabuleiro tabuleiro;
    tJogador jogador1;
    tJogador jogador2;
    
} tJogo;
tJogo CriaJogo();

void ComecaJogo(tJogo jogo);

int AcabouJogo(tJogo jogo);

int ContinuaJogo();
	



tJogo jogo;
int main(int argc, char** argv) {
    
    do{
        jogo = CriaJogo( );

        ComecaJogo(jogo);
        
    } while( ContinuaJogo() );
 
    return (EXIT_SUCCESS);
}




tTabuleiro CriaTabuleiro(){
    tTabuleiro tab;
    int i, j;
    
    tab.peca1 = 'X';
    tab.peca2 = '0';
    tab.pecaVazio = '-';
    for (i = 0; i < TAM_TABULEIRO; i++){
        for (j = 0; j < TAM_TABULEIRO; j++){
            tab.posicoes[i][j] = tab.pecaVazio;
        }
    }
    return tab;
}

tTabuleiro MarcaPosicaoTabuleiro(tTabuleiro tabuleiro, int peca, int x, int y){
    if (peca == PECA_1){
        tabuleiro.posicoes[y][x] = tabuleiro.peca1;
    } else if (peca == PECA_2){
        tabuleiro.posicoes[y][x] = tabuleiro.peca2;
    } else {
        printf("[ERRO] Jogador invalido!\n");
    }
    return tabuleiro;
}

int TemPosicaoLivreTabuleiro(tTabuleiro tabuleiro){
    int i, j;
    for (i = 0; i < TAM_TABULEIRO; i++){
        for (j = 0; j < TAM_TABULEIRO; j++){
            if ( tabuleiro.posicoes[i][j] == tabuleiro.pecaVazio )
                return 1;
        }
    }
    return 0;
}

int EstaMarcadaPosicaoPecaTabuleiro(tTabuleiro tabuleiro, int x, int y, int peca){
    if (peca == PECA_1)
        return tabuleiro.posicoes[y][x] == tabuleiro.peca1;
    else if (peca == PECA_2)
        return tabuleiro.posicoes[y][x] == tabuleiro.peca2;

    printf("[ERRO] Jogador Invalido!\n");
    return 0;
}

int EstaLivrePosicaoTabuleiro(tTabuleiro tabuleiro, int x, int y){
    return tabuleiro.posicoes[y][x] == tabuleiro.pecaVazio;
}

int EhPosicaoValidaTabuleiro(tTabuleiro tabuleiro, int x, int y){
    return  0 <= y && y < TAM_TABULEIRO &&
            0 <= x && x < TAM_TABULEIRO;
}

int ImprimeTabuleiro(tTabuleiro tabuleiro){
    int i, j;
    char linhaSaida[5] = "\t   ";
    for (i = 0; i < TAM_TABULEIRO; i++){
        for (j = 0; j < TAM_TABULEIRO; j++){
            linhaSaida[j+1] = tabuleiro.posicoes[i][j];
        }
        printf("%s\n", linhaSaida);
    }
}




tJogada LeJogada(){
    tJogada jog;
    jog.sucesso = 1;
    printf("Digite uma posicao (x e y):\n");
    int rtn = scanf("%d %d", &jog.x, &jog.y);
    if ( rtn != 2 ){
        jog.sucesso = 0;
    }
    return jog;
}

int ObtemJogadaX(tJogada jogada){
    return jogada.x;
}

int ObtemJogadaY(tJogada jogada){
    return jogada.y;
}

int FoiJogadaBemSucedida(tJogada jogada){
    return jogada.sucesso;
}




tJogador CriaJogador(int idJogador){
    tJogador jogador;
    jogador.id = idJogador;
    
//    printf("Digite o nome do jogador %d:", jogador.id); //Para dar nome ao jogador
//    scanf("%s", jogador.nome); //Para dar nome ao jogador
    return jogador;
}

tTabuleiro JogaJogador(tJogador jogador, tTabuleiro tabuleiro){
    int x, y, livre = 0, valida = 0;
    do {
        printf("Jogador %d\n", jogador.id);
        tJogada jogada = LeJogada();
        if ( !FoiJogadaBemSucedida( jogada ) ){
            scanf("%*[^\n]");
            scanf("%*c");
            printf("Formato invalido!\n");
            continue;
        }
        
        x = ObtemJogadaX(jogada);
        y = ObtemJogadaY(jogada);
        
        valida = EhPosicaoValidaTabuleiro(tabuleiro, x, y);
        if ( valida ){
            livre = EstaLivrePosicaoTabuleiro(tabuleiro, x, y);
            if ( !livre ){
                printf("Posicao invalida (OCUPADA - [%d,%d] )!\n", x, y);
            }
        } else
            printf("Posicao invalida (FORA DO TABULEIRO - [%d,%d] )!\n", x, y);
    } while (!valida || !livre);
    int peca = (jogador.id == ID_JOGADOR_1 ? PECA_1: PECA_2);
    printf("Jogada [%d,%d]!\n", x, y);
    tabuleiro = MarcaPosicaoTabuleiro(tabuleiro, peca, x, y);
    return tabuleiro;
}

int VenceuJogador(tJogador jogador, tTabuleiro tabuleiro){
    int peca = (jogador.id == ID_JOGADOR_1 ? PECA_1: PECA_2);
    if (    EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 0, 0, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 0, 1, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 0, 2, peca ) ){
        return 1;
    }
    if (    EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 1, 0, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 1, 1, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 1, 2, peca ) ){
        return 1;
    }
    if (    EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 2, 0, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 2, 1, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 2, 2, peca ) ){
        return 1;
    }
    if (    EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 0, 0, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 1, 0, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 2, 0, peca ) ){
        return 1;
    }
    if (    EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 0, 1, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 1, 1, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 2, 1, peca ) ){
        return 1;
    }
    if (    EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 0, 2, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 1, 2, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 2, 2, peca ) ){
        return 1;
    }
    if (    EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 0, 0, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 1, 1, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 2, 2, peca ) ){
        return 1;
    }
    if (    EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 0, 2, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 1, 1, peca ) &&
            EstaMarcadaPosicaoPecaTabuleiro( tabuleiro, 2, 0, peca ) ){
        return 1;
    }
    return 0;
}




tJogo CriaJogo(){
    tJogo jogo;
    
    jogo.tabuleiro = CriaTabuleiro();
    jogo.jogador1 = CriaJogador(ID_JOGADOR_1);
    jogo.jogador2 = CriaJogador(ID_JOGADOR_2);
    return jogo;
}

void ComecaJogo(tJogo jogo){
    int jogador1Venceu = 0, jogador2Venceu = 0;
    while( 1 ){

        jogo.tabuleiro = JogaJogador(jogo.jogador1, jogo.tabuleiro); 
        jogador1Venceu = VenceuJogador(jogo.jogador1, jogo.tabuleiro);
        ImprimeTabuleiro(jogo.tabuleiro);
        if (jogador1Venceu || AcabouJogo(jogo))
            break;
        jogo.tabuleiro = JogaJogador(jogo.jogador2, jogo.tabuleiro); 
        jogador2Venceu = VenceuJogador(jogo.jogador2, jogo.tabuleiro);
        ImprimeTabuleiro(jogo.tabuleiro);
        if (jogador2Venceu || AcabouJogo(jogo))
            break;
    }
    
    if (jogador1Venceu)
        printf("JOGADOR 1 Venceu!\n");
    else if (jogador2Venceu)
        printf("JOGADOR 2 Venceu!\n");
    else 
        printf("Sem vencedor!\n");
}

int AcabouJogo(tJogo jogo){
    return !TemPosicaoLivreTabuleiro(jogo.tabuleiro);
}

int ContinuaJogo(){
    char simounao = '\0';
    while(simounao != 's' && simounao != 'n'){
        printf("Jogar novamente? (s,n)\n");
        
        scanf("%*[^sn]");
        if ( scanf("%c", &simounao) == EOF)
          simounao = 'n';
        scanf("%*[^\n]");
        if ( scanf("%*c") == EOF)
          simounao = 'n';
    }
    if (simounao == 's')
        return 1;
    else
        return 0;
}


