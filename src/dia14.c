#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define Origem {500,0}
#define direcao "->"
#define dimensao 1000



typedef struct posicao
{
    int x;
    int y;    
}POS;


void strParaPos (char *str, POS *p);

void printZonaMapa(int initL,int fimL,int initC,int fimC,bool mapa[][dimensao]);



bool quedaAreia(POS origem,bool mapa[][dimensao],int numeroLinhas);


void preencheCaminho (POS inicio, POS fim , bool  mapa[][dimensao],int *maxy);

void bloqueiaLinha(char *linha,bool mapa[][dimensao],int *maxy);



int main (int argc, char *argv[])
{

    if (argc <= 1) return 1;


    FILE *entrada = fopen(argv[1],"r");

    bool mapa[dimensao][dimensao]; // mapa das posicoes em que esta bloqueado ou nao
    int i,j;

    for (i =0;i<dimensao;i++)
        for (j =0; j<dimensao; j++)
            mapa[i][j] =false;
    

    int maxy = 0;
    POS areia = Origem;
    size_t size;
    char *linha;
    while (getline(&linha,&size,entrada) != -1)
    {
        bloqueiaLinha(linha,mapa,&maxy);
    }
    free (linha);

    bool mapaPart2[dimensao][dimensao];

    for (i =0;i<maxy+2;i++)
        for (j =0; j<dimensao; j++)
            mapaPart2[i][j] =mapa[i][j];

    for (j =0; j<dimensao; j++)
            mapaPart2[maxy+2][j] =true;

    i =0;
  
    while (quedaAreia(areia,mapa,maxy+1))
    {
        i++;
    }



    printf("Parou de cair apos %d\n",i);
    i =1;
     while (quedaAreia(areia,mapaPart2,maxy+2))
    {
        i++;
    }
    printf("Com chão, parou de cair apos %d\n",i);
    
}



void bloqueiaLinha(char *linha,bool mapa[][dimensao],int *maxy)
{
    POS inicio,fim;
    char * aux,*found;
    aux = strdup(linha);

    // primeiro campinho
    found = strtok(aux,direcao);
    if (found == NULL) return;
    strParaPos(found,&inicio);
    found = strtok(NULL,direcao);
    if (found == NULL) return;
    strParaPos(found,&fim);
    if (*maxy < inicio.y) * maxy = inicio.y;
    preencheCaminho(inicio,fim,mapa,maxy);
    
    int i =1;
    while( (found = strtok(NULL,direcao)) != NULL )
    {
        inicio = fim;
        strParaPos(found,&fim);
        preencheCaminho(inicio,fim,mapa,maxy);
        i++;
    }
    free (aux);
}

void preencheCaminho (POS inicio, POS fim , bool  mapa[][dimensao],int *maxy)
{
    if (*maxy < fim.y) *maxy = fim.y;

    int i;
    for (i = inicio.x;  i <= fim.x;i++)
        mapa[inicio.y][i] = true;

    for (i = inicio.x;  i >= fim.x;i--)
        mapa[inicio.y][i] = true;

    for (i = inicio.y;  i <= fim.y;i++)
        mapa[i][inicio.x] = true;

    for (i = inicio.y;  i >= fim.y;i--)
        mapa[i][inicio.x] = true;
    

}
void printZonaMapa(int initL,int fimL,int initC,int fimC,bool mapa[][dimensao])
{
    int i,j;
    for(i =initL;i <= fimL;i++)
    {
        for (j = initC;j<= fimC;j++)
        {
            if (mapa[i][j]) putchar('#');
            else putchar('.');
        }
        putchar('\n');
    }
}
void strParaPos (char *str, POS *p)
{
    int x,y;
    sscanf(str,"%d,%d",&x,&y);
    p->x = x;
    p->y = y;
    
}

bool quedaAreia(POS origem,bool mapa[][dimensao],int numeroLinhas)
{
    POS aux = origem;
    while (origem.y <= numeroLinhas)
    {
        origem.y++;
        if (mapa[origem.y][origem.x])// verificar se em baixo esta ocupado
        {
            if (!mapa[origem.y][origem.x-1]) origem.x--;
            else if (!mapa[origem.y][origem.x+1]) origem.x++;
            else
            {
                origem.y--;
                mapa[origem.y][origem.x] = true;
                if (aux.y == origem.y && aux.x == origem.x)
                {
                    return false;
                }
                

                return true;
            }
        }
    }
    
    return false;
}
