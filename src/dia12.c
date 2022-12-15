#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PARTIDA 'a'
#define CHEGADA 'z'


typedef struct aresta {
int dest;
struct aresta *prox;
} *LAdj;


int contaPassos (int ant[],int d,int o);


LAdj criaAresta(int destino,LAdj restantes);

void preecncheCaminhos(char *str,int nLinhas,int nColunas, LAdj *caminhos);


int travessiaBF (int nDigitos, LAdj *caminhos ,int o,int destino,int visitados[]);


int posicao (int nColunas,int indL,int indC);


void tiraQuebraLinha (char *linha);

void inicioFim (char *mapa,int *inicio, int *fim);

int passo (char atual,char dest);

int melhorInicio(int nDigitos,char *str,LAdj *caminhos,int destino, int ant[],int melhor);



int main (int argc, char *argv[])
{
    if (argc <= 1) return 1;
    FILE *entrada = fopen(argv[1],"r");


    int t = 1,aux=0,nLinhas =0,nColunas =0;
    // ler a primeira linha para determinar o tamanho de uma coluna
    char *mapa = NULL; 

    size_t size;
    char *linha;
    getline(&linha,&size,entrada);
    nLinhas ++;
    tiraQuebraLinha(linha);
    aux = strlen(linha); // numero de carateres
    nColunas = aux;
    t += aux;
    mapa = malloc(t);
    strcpy(mapa,linha);

    while (getline(&linha,&size,entrada) != -1)
    {
        tiraQuebraLinha(linha);
        nLinhas++;
        t+=nColunas;
        mapa = realloc(mapa,t);
        strcpy(mapa+aux,linha);
        aux += nColunas;

    }
    free (linha);
    int inicio,fim;
    inicioFim(mapa,&inicio,&fim);
    LAdj *caminhos = malloc(aux * sizeof(LAdj));
    int *ant = malloc(aux*sizeof(int));

    preecncheCaminhos(mapa,nLinhas,nColunas,caminhos);

    int x = travessiaBF(aux,caminhos,inicio,fim,ant);

    int passos = contaPassos(ant,fim,inicio);



    printf("(%d)Travessia completa em %d passos\n",x,passos);


   int i = melhorInicio(aux,mapa,caminhos,fim,ant,passos);
  printf("Melhor caminho em %d\n",i);


}



int melhorInicio(int nDigitos,char *str,LAdj *caminhos,int destino, int ant[],int melhor)
{
    int i,aux;
    for (i=0; i<nDigitos; i++)
    {
        if (str[i] == 'a')
        {
            travessiaBF(nDigitos,caminhos,i,destino,ant);
            aux = contaPassos(ant,destino,i);
            if (aux>-1 && aux < melhor) melhor = aux;
        }
    }


    return melhor;

}

int passo (char atual,char dest)
{
    return (dest - atual <= 1 );
}

void tiraQuebraLinha (char *linha)
{
    int t = strlen (linha);
        if (t > 0 && linha [t-1] == '\n') linha [t-1] = '\0';
}

int posicao (int nColunas,int indL,int indC)
{
    int i =0;
    i = nColunas *indL+ indC;
    return i;
}

void inicioFim (char *mapa,int *inicio, int *fim)
{
    for (int i =0;mapa[i] != '\0';i++)
    {
        if (mapa[i] == 'S') 
        {
            *inicio = i;
            mapa[i] = PARTIDA;
        }
        else if (mapa[i] == 'E') 
        {
            mapa[i] = CHEGADA;
            *fim = i;
        }
    }
}

int contaPassos (int ant[],int d,int o)
{
    int passos =0;
    while ( d!= o && ant[d] >=0)
    {
        d = ant[d];
        passos ++;
    }
    if (d != o) passos = -1;
    return passos;
}


int travessiaBF (int nDigitos, LAdj *caminhos ,int o,int destino,int visitados[])
{
    for (int i = 0; i<nDigitos;i++)
        visitados[i] = -2;

    int *q = malloc(nDigitos* sizeof(int));

    int inicio = 0,fim =0 ,count =0;
    LAdj it;


    q[fim++] = o; // enqueue ( q , o ) ;
    visitados[o] = -1;

    while ( inicio < fim ) 
    { // ! empty ( q )
        o = q[inicio++]; // o = dequeue ( q ) ;
        count++;

        for( it = caminhos[o] ; it != NULL; it= it->prox )
        {
            if ( visitados[it->dest] == -2)
            {
                q [fim++] = it->dest;
                visitados[it->dest] = o;
            }
            
            if (it->dest == destino) 
            {
                return count;
            }
        }


    }
    free(q);
    return count;
}



LAdj criaAresta(int destino,LAdj restantes)
{
    LAdj x= malloc(sizeof(struct aresta));
    x->dest = destino;
    x->prox = restantes;
    return x;
}

void libertaAresta(LAdj aresta)
{
    if (aresta == NULL) return;

    libertaAresta(aresta->prox);
    free (aresta);
}


void preecncheCaminhos(char *str,int nLinhas,int nColunas, LAdj *caminhos)
{
    int nDigitos = nLinhas *nColunas;
    int i,j;
    for (i =0; i<nDigitos;i++)
    {
        caminhos[i] = NULL;
    }
    int pos,dest;
    char atual,proximo;
    
    for ( i =0;i<nLinhas;i++)
    {
        for (j =0;j<nColunas;j++)
        {
            pos =posicao(nColunas,i,j);
            atual = str[pos];

            if (i)
            {
                dest = posicao(nColunas,i-1,j);
                proximo = str[dest];
                if (passo(atual,proximo)) caminhos[pos] = criaAresta(dest,caminhos[pos]);
            }

            if (i!= nLinhas-1)
            {
                dest = posicao(nColunas,i+1,j);
                proximo = str[dest];
                if (passo(atual,proximo)) caminhos[pos] = criaAresta(dest,caminhos[pos]);
            }


            if (j)
            {
                dest = posicao(nColunas,i,j-1);
                proximo = str[dest];
                if (passo(atual,proximo)) caminhos[pos] = criaAresta(dest,caminhos[pos]);
            }
            if (j != nColunas-1)
            {
                dest =posicao(nColunas,i,j+1);
                proximo = str[dest];
                if (passo(atual,proximo)) caminhos[pos] = criaAresta(dest,caminhos[pos]);
            }
        }
    }
}