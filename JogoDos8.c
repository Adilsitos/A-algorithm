#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include<stdbool.h>

typedef struct Lista{
    int heuristica;
    int **matriz;
    struct Lista *prox;
}Lista;



int **criaMatrizSolucao(int **MS){ //Cria a matriz final, ou seja, a matriz a qual o programa deve chegar
    MS=(int**)malloc(3* sizeof(int*)); //alocando memoria para  a matriz solucao
    int x,y,cont;
    cont=0;
    for(x=0;x<3;x++){//Criando matriz
        MS[x]=(int*)malloc(3*sizeof(int)); //alocando memoria para  a matriz solucao
    }
    for(x=0;x<3;x++){ //Inserindo valores na matriz

        for(y=0;y<3;y++){
            MS[x][y]=cont;
            cont++; //cont mudando de valor ate 8, para chegar a matriz desejada
        }
    }
    return MS; //retornando a matriz
}


void imprimeMatriz(int **C){//imprime matriz
    printf("\n \n");
    int x,y;
    for(x=0;x<3;x++){//x  linha
        for(y=0;y<3;y++){//y coluna
            printf("%d ", C[x][y]);//print numero que esta nessa posicao
        }
        printf("\n");
    }
}

int verifica(int num, int **M){//Verifica se o numero ja existe na matriz
    int x, y;
   for(x = 0; x < 3; x++){
        for(y = 0; y < 3; y++){ //percorrendo os valores da matriz
            if(num == M[x][y]){
               return 0; //caso encontre o valor na matriz, retorna 0
            }
        }
    }
    return 1; // caso nao encontre o valor na matriz, retorna 1
}

int **criaMatrizRandom(int **M){//Gera matriz randomica(Matriz a ser solucionada)
    M=(int**)malloc(3* sizeof(int*)); //alocando espaço de memoria na matriz
    int x,y,z=0; // variaveis utilizadas para percorrer a matriz
    for(x=0;x<3;x++){
        M[x]=(int*)malloc(3*sizeof(int)); //alocando espaço de memoria na matriz
    }

    int num,num2 ;
    srand(time(NULL)); //função para a raiz nunca ser igual

    for(x=0;x<3;x++){
        for(y=0;y<3;y++){
            num = rand()%8;  //gerando um numero aleatorio de 0 a 8
            while(verifica(num,M) == 0 ){//Chama a funcao verifica, para saber se o numero ja existe, se existir,gera outro numero
                num = rand()%10;

               
            }   
             // atribui o numero aleatorio na matriz
            M[x][y] = num;
        }
    }

    for(x=0;x<3;x++){
        for(y=0;y<3;y++){
            if(M[x][y] == 9){
                M[x][y] = 0;

            }
        }
    }


    return M; // retorna a matriz randomica
}

int calcAestrela(int **MR, int **MS){ //Calcula a distancia da peca para sua posicao final em linha reta
    int x,y,z=0;
    int manhattan = 0,g=0,soma=0,i,j,k,l;

    
    while(z != 9){
        for(x=0;x<3;x++){
            for(y=0;y<3;y++){//Pega as coordenadas da peca na matriz a ser solucionada
                if(MR[x][y] == z){
                    i = x; //guarda a posica x em i
                    j = y; // guarda a posicao y em j
                }
            }
        }
   
        for(x=0;x<3;x++){
            for(y=0;y<3;y++){//Pega as coordenadas da peca na matriz solucao
                if(MS[x][y] == z){
                    k = x; //guarda a posicao x em k
                    l = y; //guarda a posica y em l
                }
            }
        }   
        manhattan = fabs(k-i)+ fabs(l-j);
        if(manhattan != 0){
            g++;
        }
        soma += manhattan;
        //printf(" numero %d Valor da dist %d \n",z,manhattan); 
        z++;
    }
    
    soma += g;
     //Calcula a distancia da peca em linha reta, pegando a coordenada x da matriz solucao e randomica
                                // e pegando a coordenada y da matriz soluca e randomica, elevando ao quadrado as duas
                                // pela funcao "fabs" e assim gerando a distancia
    
    return soma; // retornando a distancia

}

Lista *insereInicio(int heuristicaEstrela, Lista *L, int **M){
   if(L == NULL){
       L = (Lista*) malloc(sizeof(Lista));
       L->heuristica = heuristicaEstrela;
       L->matriz = M;
       L->prox =NULL;
       return L;
    }else{
        
        Lista *novo;
        novo = (Lista*) malloc(sizeof(Lista));
        novo->heuristica = heuristicaEstrela;
        novo->matriz = M;
        novo->prox = L;
       
        return novo;
    }
}


void imprime(Lista *L){
    Lista *aux =L;
    int **MS;
    MS = criaMatrizSolucao(MS);
    while(aux != NULL){
        imprimeMatriz(aux->matriz);
        printf("Heuristica %d\n",calcAestrela(aux->matriz,MS));
        aux = aux->prox;
    }
    
}
int comparaMatriz(int **M1, int **M2){
    int x=0,y=0,cont=0;
    for(x=0;x<3;x++){
        for(y=0;y<3;y++){
            if(M1[x][y] == M2[x][y]){
                cont++;
            }
        }
    }
    return cont;
}


int procuraListaAbertos(Lista *Abertos, int **MR){
    Lista *aux = Abertos;
    int x =0, y=0;
    
    while(aux != NULL){
     
        if(comparaMatriz(aux->matriz,MR) == 9){
            //printf("Valor ja aberto");
            return 1;

        }
        
        aux = aux->prox;
    }
    return 0;
}


Lista *excluiListaFechados(Lista *Fechados, int **MR){
    Lista *aux = Fechados;
    Lista *ant = NULL;
    
    if(aux == NULL){
        return NULL;
    }else{
        while(comparaMatriz(aux->matriz,MR) != 9 && aux != NULL){
            ant = aux;
            aux = aux->prox;
        }
        if(aux == NULL){
            return Fechados;
        }else if(ant == NULL){
            Fechados = aux->prox;
            free(aux);
            return Fechados;
        }else{
            ant->prox = aux->prox;
            free(aux);
            return Fechados;
        }
    }
}

Lista *move(int **MR,int **MS, Lista *Fechados, Lista *Abertos){
    
    int i=0,j=0,x=0,y=0,aux = 0;
    int **NM1, **NM2, **NM3, **NM4;   //NM = New matriz 
    NM1 = (int**)malloc(3*sizeof(int)); //alocando uma matriz
    NM2 = (int**)malloc(3*sizeof(int));//alocando uma matriz
    NM3 = (int**)malloc(3*sizeof(int));//alocando uma matriz
    NM4 = (int**)malloc(3*sizeof(int));
        for(i=0;i<3;i++){
            NM1[i] = (int*) malloc(3*sizeof(int)); //alocando uma matriz
            NM2[i] = (int*) malloc(3*sizeof(int));//alocando uma matriz
            NM3[i] = (int*) malloc(3*sizeof(int));
            NM4[i] = (int*) malloc(3*sizeof(int));
        }

         for(i=0;i<3;i++){
            for(j=0;j<3;j++){
                NM1[i][j] = MR[i][j]; //deixando a matriz alocada com mesmo valor de M
                NM2[i][j] = MR[i][j];
                NM3[i][j] = MR[i][j];
                NM4[i][j] = MR[i][j];

                if(MR[i][j] == 0){
                    x = i;
                    y = j;
                }               
            }
        }
    aux = NM1[x][y];
   
    if(x == 0 && y == 0 ){ 
       
        NM1[x][y] = NM1[x+1][y];
        NM1[x+1][y] = aux;

        NM2[x][y] = NM2[x][y+1];
        NM2[x][y+1] = aux;

        if(procuraListaAbertos(Abertos,NM1) != 1){
            Fechados = insereInicio(calcAestrela(NM1,MS),Fechados,NM1);
        }
        if(procuraListaAbertos(Abertos,NM2) != 1){
            Fechados = insereInicio(calcAestrela(NM2,MS),Fechados,NM2);
        }
    }

    if(x == 0 && y==1){
        

        NM1[x][y] = NM1[x+1][y];
        NM1[x+1][y] = aux;

        NM2[x][y] = NM2[x][y+1];
        NM2[x][y+1] = aux;

        NM3[x][y] = NM3[x][y-1];
        NM3[x][y-1] = aux;

        if(procuraListaAbertos(Abertos,NM1) != 1){
            Fechados = insereInicio(calcAestrela(NM1,MS),Fechados,NM1);
        }
        if(procuraListaAbertos(Abertos,NM2) != 1){
            Fechados = insereInicio(calcAestrela(NM2,MS),Fechados,NM2);
        }
        if(procuraListaAbertos(Abertos,NM3) != 1){
            Fechados = insereInicio(calcAestrela(NM3,MS),Fechados,NM3);
        }
        
    }

    if(x == 0 && y==2){
        

        NM1[x][y] = NM1[x+1][y];
        NM1[x+1][y] = aux;

        NM2[x][y] = NM2[x][y-1];
        NM2[x][y-1] = aux;

        if(procuraListaAbertos(Abertos,NM1) != 1){
            Fechados = insereInicio(calcAestrela(NM1,MS),Fechados,NM1);
        }
        if(procuraListaAbertos(Abertos,NM2) != 1){
            Fechados = insereInicio(calcAestrela(NM2,MS),Fechados,NM2);
        }
    }

    if(x==1 && y==0){
        

        NM1[x][y] = NM1[x+1][y];
        NM1[x+1][y] = aux;

        NM2[x][y] = NM2[x-1][y];
        NM2[x-1][y] = aux;

        NM3[x][y] = NM3[x][y+1];
        NM3[x][y+1] = aux;

        if(procuraListaAbertos(Abertos,NM1) != 1){
            Fechados = insereInicio(calcAestrela(NM1,MS),Fechados,NM1);
        }
        if(procuraListaAbertos(Abertos,NM2) != 1){
            Fechados = insereInicio(calcAestrela(NM2,MS),Fechados,NM2);
        }
        if(procuraListaAbertos(Abertos,NM3) != 1){
            Fechados = insereInicio(calcAestrela(NM3,MS),Fechados,NM3);
        }
        
    }

    if(x==1 && y==1){
        

        NM1[x][y] = NM1[x+1][y];
        NM1[x+1][y] = aux;
       
        NM2[x][y] = NM2[x][y+1];
        NM2[x][y+1] = aux;
        
        NM3[x][y] = NM3[x-1][y];
        NM3[x-1][y] = aux;

        NM4[x][y] = NM4[x][y-1];
        NM4[x][y-1] = aux;

        if(procuraListaAbertos(Abertos,NM1) != 1){
            Fechados = insereInicio(calcAestrela(NM1,MS),Fechados,NM1);
        }
        if(procuraListaAbertos(Abertos,NM2) != 1){
            Fechados = insereInicio(calcAestrela(NM2,MS),Fechados,NM2);
        }
        if(procuraListaAbertos(Abertos,NM3) != 1){
            Fechados = insereInicio(calcAestrela(NM3,MS),Fechados,NM3);
        }
        if(procuraListaAbertos(Abertos,NM4) != 1){
            Fechados = insereInicio(calcAestrela(NM4,MS),Fechados,NM4);
        }

    }

    if(x== 1 && y==2){
        NM1[x][y] = NM1[x+1][y];
        NM1[x+1][y] = aux;
       
        NM2[x][y] = NM2[x][y-1];
        NM2[x][y-1] = aux;
        
        NM3[x][y] = NM3[x-1][y];
        NM3[x-1][y] = aux;

        if(procuraListaAbertos(Abertos,NM1) != 1){
            Fechados = insereInicio(calcAestrela(NM1,MS),Fechados,NM1);
        }
        if(procuraListaAbertos(Abertos,NM2) != 1){
            Fechados = insereInicio(calcAestrela(NM2,MS),Fechados,NM2);
        }
        if(procuraListaAbertos(Abertos,NM3) != 1){
            Fechados = insereInicio(calcAestrela(NM3,MS),Fechados,NM3);
        }

    }

    if(x == 2 && y == 0){
        NM1[x][y] = NM1[x-1][y];
        NM1[x-1][y] = aux;
       
        NM2[x][y] = NM2[x][y+1];
        NM2[x][y+1] = aux; 

        if(procuraListaAbertos(Abertos,NM1) != 1){
            Fechados = insereInicio(calcAestrela(NM1,MS),Fechados,NM1);
        }
        if(procuraListaAbertos(Abertos,NM2) != 1){
            Fechados = insereInicio(calcAestrela(NM2,MS),Fechados,NM2);
        }
    }

    if(x == 2 && y==1){
        NM1[x][y] = NM1[x-1][y];
        NM1[x-1][y] = aux;
       
        NM2[x][y] = NM2[x][y+1];
        NM2[x][y+1] = aux;

        NM3[x][y] = NM3[x][y-1];
        NM3[x][y-1] = aux;

        if(procuraListaAbertos(Abertos,NM1) != 1){
            Fechados = insereInicio(calcAestrela(NM1,MS),Fechados,NM1);
        }
        if(procuraListaAbertos(Abertos,NM2) != 1){
            Fechados = insereInicio(calcAestrela(NM2,MS),Fechados,NM2);
        }
        if(procuraListaAbertos(Abertos,NM3) != 1){
            Fechados = insereInicio(calcAestrela(NM3,MS),Fechados,NM3);
        }

    }
    if(x == 2 && y == 2){
        aux = NM1[x][y];

        NM1[x][y] = NM1[x-1][y];
        NM1[x-1][y] = aux;

        NM2[x][y] = NM2[x][y-1];
        NM2[x][y-1] = aux;

        if(procuraListaAbertos(Abertos,NM1) != 1){
            Fechados = insereInicio(calcAestrela(NM1,MS),Fechados,NM1);
        }
        if(procuraListaAbertos(Abertos,NM2) != 1){
            Fechados = insereInicio(calcAestrela(NM2,MS),Fechados,NM2);
        }
    }
    
    //Fechados = excluiListaFechados(Fechados,MR);
    
    return Fechados;
}

int **procuraMenor(Lista *Fechados){
    
    Lista *aux = Fechados;

    Lista *menor = Fechados;
    while(aux != NULL){
        if(menor->heuristica > aux->heuristica){
            menor = aux;
        }

        aux = aux->prox;
    }

    return menor->matriz;

}

int Resolve(Lista *Fechados, Lista *Abertos, int **MR, int **MS){
    int i =0;
    if(Fechados == NULL){
        Abertos = insereInicio(calcAestrela(MR,MS),Abertos,MR);
        Fechados = move(MR,MS,Fechados,Abertos);
        i = Resolve(Fechados,Abertos,procuraMenor(Fechados),MS);
    }
    int **Menor, x= 0;
    Menor = (int**)malloc(3*sizeof(int));
    for(x=0;x<3;x++){
        Menor[x] = (int*) malloc(3*sizeof(int));
    }
    Menor = (procuraMenor(Fechados));
    if(calcAestrela (Menor,MS) != 0){
        
        Abertos = insereInicio(calcAestrela(MR,MS),Abertos,Menor);
        Fechados = move(Menor,MS,Fechados,Abertos);
        Fechados = excluiListaFechados(Fechados,Menor);
        printf("Menor valor");
        imprimeMatriz(Menor);
        //printf("Matriz Random original");
       
        printf("Heuristica %d\n",calcAestrela(Menor,MS));
        
        //printf("Fechados \n");
        //imprime(Fechados);
        //printf("Abertos \n");
        //imprime(Abertos);
        //getchar();
        
        i = Resolve(Fechados,Abertos,Menor,MS);
    }else{
        printf("Resolvido com sucesso");
        imprimeMatriz(Menor);
        return 1;
    }

       
        

  
}


int main(){
    int **MS;
    int **MR;
    int **MT;
    int x=0;
    MS = criaMatrizSolucao(MS);
    MR =  criaMatrizRandom(MR);
    
    //imprimeMatriz(MR);
    imprimeMatriz(MS);

    
    Lista *Abertos = NULL; 
    Lista *Fechados = NULL;


    MT=(int**)malloc(3* sizeof(int*)); //alocando espaço de memoria na matriz
    int y,z=0; // variaveis utilizadas para percorrer a matriz
    for(x=0;x<3;x++){
        MT[x]=(int*)malloc(3*sizeof(int)); //alocando espaço de memoria na matriz
    }

    MT[0][0]=4;
    MT[0][1] = 5;
    MT[0][2] = 6;
    MT[1][0] = 1;
    MT[1][1] = 2;
    MT[1][2] = 0;
    MT[2][0] = 3;
    MT[2][1] = 7;
    MT[2][2] = 8;
    imprimeMatriz(MT);

    Resolve(Fechados,Abertos,MR,MS);
    //Fechados = insereInicio(calcAestrela(MR,MS),Fechados,MR);
    //Fechados = insereInicio(calcAestrela(MR,MS),Fechados,MS);
    
    //Fechados = excluiListaFechados(Fechados,MS);
    //imprime(Fechados);
    /*Abertos = insereInicio(calcAestrela(MR,MS),Abertos,MR);
    printf("valores abertos\n");
    imprime(Abertos);
    Fechados = move(MR,MS,Fechados,Abertos);
    printf("valores fechados\n");
    imprime(Fechados);*/
}