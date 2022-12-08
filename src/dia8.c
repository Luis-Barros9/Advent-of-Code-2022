#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



typedef struct grid
{
    char **dados;
    int nLinhas;
    int nColunas;
    int size;
}GRID;


GRID *initGrid (int colunas,char *l);

void insereLinha (GRID *g ,char *linha);

int peRimetro(int colunas, int linhas);

int visiveis (GRID *g,int *acumulador);


void tiraQuebraLinha (char *linha)
{
    int t = strlen (linha);
        if (t > 0 && linha [t-1] == '\n') linha [t-1] = '\0';
}


int visivel (char valor,int colunaPos, int linhaPos, GRID *g);

int visibilidade (char valor,int colunaPos, int linhaPos, GRID *g);



int main (int argc, char *argv[])
{
    if (argc <= 1) return 1;
    FILE *entrada = fopen(argv[1],"r");
    if (entrada == NULL) return 2;
    int i = 0,t;
    // ler a primeira linha para determinar o tamanho de uma coluna
    char linha[1000];
    fgets(linha,1000,entrada);
    tiraQuebraLinha(linha);
    t = strlen(linha);
    GRID *g = initGrid(t,linha);    
    while (fgets(linha,1000,entrada))
    {
        tiraQuebraLinha (linha);
        insereLinha (g,linha);
    }
    int vis;
    int v = visiveis (g,&vis);
    printf ("Há %d  arvores visiveis, com visibilidade maxima %d\n",v,vis);
}

int peRimetro(int colunas, int linhas)
{
    return 2* (colunas+linhas-2);
}

int visiveis (GRID *g,int *acumulador)
{
    int c = g->nColunas,l= g->nLinhas;
    int total = peRimetro(c,l);
    
    int i,j,max = 0, atual = 0;
    for (i = 1; i<l-1;i++)
    {//PERCORRER  LINHAS INTERIORES
        char *linha = g->dados[i];
        for (j = 1;j < c-1;j++)
        { // PERCORRER COLUNAS INTERIORES
            char pos = linha[j];
            total += (visivel(pos,j,i,g));
            atual = visibilidade (pos,j,i,g);
            if (atual > max) max = atual;
        }
    }


    *acumulador = max;  
    return total;
}


int visibilidade (char valor,int colunaPos, int linhaPos, GRID *g)
{
    int res,esq,dir,cima,baixo,i;
    char *linha = g->dados[linhaPos];

    for (i = colunaPos -1; i > 0 && linha[i] < valor;i--);
    esq = colunaPos - i;
    for (i = colunaPos +1; i < g->nColunas-1 && linha[i] < valor;i++);
    dir = i - colunaPos;

    for (i = linhaPos-1;i > 0 && (g->dados[i])[colunaPos] < valor;i--);
    cima = linhaPos -i;

    for (i = linhaPos +1; i < g->nLinhas-1 && (g->dados[i])[colunaPos] < valor;i++);
    baixo = i - linhaPos;


    res = esq * dir * cima * baixo;

    return res;

}



int visivel (char valor,int colunaPos, int linhaPos, GRID *g)
{
    // começamos por pensar que não é visível
    int i;
    char compararcao;
    char *linha = g->dados[linhaPos];

    
    for (i = 0;i<colunaPos && linha[i] < valor;i++); // percorrer a linha em que ele esta de inicio ate a sua posicao
    if  (i == colunaPos)
    {   
        return 1;
    }
    
    
    for (i = g->nColunas-1; i >colunaPos && linha[i] < valor;i--); // percorrer a linha em que ele esta do fim ate a sua posicao
    if  (i == colunaPos)
    {   
        return 1;
    }
    for (i = 0;i<linhaPos && (g->dados[i])[colunaPos] < valor;i++);
    if (i == linhaPos)
    {   
        return 1;
    }

    for (i = g->nLinhas -1; i > linhaPos && g->dados[i][colunaPos] < valor;i--);
    if (i == linhaPos)
    {
        return 1;
    }
    return 0;    
}


GRID *initGrid (int colunas,char *l)
{
    GRID *g = malloc (sizeof(GRID));
    g->size = 0;
    g->nColunas = colunas;
    g->size = 4;
    g->nLinhas = 1;
    g->dados = malloc (g->size * sizeof(char *));
    g->dados[0] = malloc (g->nColunas);
    strcpy (g->dados[0], l);
    return g;

}

void insereLinha (GRID *g ,char *linha)
{
    if (g->size == g->nLinhas)
    {
        g->size *=2;
        g->dados = realloc(g->dados,g->size * sizeof (char *));
    }
    char *new = malloc (g->nColunas);
    strcpy(new,linha);
    g->dados[g->nLinhas] = new;
    g->nLinhas++;

}