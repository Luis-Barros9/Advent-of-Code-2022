#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PARTIDA 'a'
#define CHEGADA 'z'


typedef struct pos
{
    int indiceL;
    int indiceC;
} POS;

typedef struct grid
{
    char **dados;
    int nLinhas;
    int nColunas;
    int size;
    POS inicio;
    POS fim;
}GRID;



int fimAoInicio(GRID *g);


int diferentes (POS x, POS y);


GRID *initGrid (int colunas,char *l);

void insereLinha (GRID *g ,char *linha);


void tiraQuebraLinha (char *linha);

void inicioEFim (GRID *g);


int passo (char atual,char dest);

int main (int argc, char *argv[])
{
    if (argc <= 1) return 1;
    FILE *entrada = fopen(argv[1],"r");


    int t;
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

    inicioEFim(g);

    int melhorc = fimAoInicio (g);
    printf ("%d\n",melhorc);

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

void inicioEFim (GRID *g)
{
    POS pos;
    int i,j;
    char *linha;
    for (j =0; j< g->nLinhas ;j++)
    {
        linha = g->dados[j];
        for (i =0; i< g->nColunas ;i++)
        {
            if (linha[i] == 'S')
            {
                pos.indiceC = i;
                pos.indiceL =j;
                g->inicio = pos;
                linha[i] = PARTIDA;
            }
            else if (linha[i] == 'E')
            {
                pos.indiceC = i;
                pos.indiceL =j;
                g->fim = pos;
                linha[i] = CHEGADA;

            }
        }
    }
}


int diferentes (POS x, POS y)
{
    return (x.indiceC != y.indiceC || x.indiceL != y.indiceL);
}


int fimAoInicio(GRID *g)
{
    int passos = 0;
    POS atual = g->fim;
    POS objetivo = g->inicio;
    int melhor,passo;
    char dir,altura,novo,anterior = '\0'; // e-> esquerd ->d ->dir c->cima b ->baixo;
    while (diferentes (objetivo,atual))
    {
        passo = '\0';
        melhor = -26;
        altura = g->dados[atual.indiceL][atual.indiceC];

        if (anterior != 'd' && atual.indiceC > 0) // possivel andar para a esquerda
        {
            novo = g->dados[atual.indiceL][atual.indiceC-1];
            passo = altura -novo;
            if (passo <= 1)
            {
                if (passo > melhor)
                    dir = 'e';
            }
        }
        if (anterior != 'e' && atual.indiceC < g->nColunas-1) // possivel andar para a direita
        {
            novo = g->dados[atual.indiceL][atual.indiceC+1];
            passo = altura -novo;
            if (passo <= 1)
            {
                if (passo > melhor)
                    dir = 'd';
            }
        }
        if (anterior != 'b' && atual.indiceL > 0) // possivel andar para cima
        {
            novo = g->dados[atual.indiceL-1][atual.indiceC];
            passo = altura -novo;
            if (passo <= 1)
            {
                if (passo > melhor)
                    dir = 'c';
            }
        }
        if (anterior != 'c' && atual.indiceL < g->nLinhas-1) // possivel andar para baixo
        {
            novo = g->dados[atual.indiceL+1][atual.indiceC];
            passo = altura -novo;
            if (passo <= 1)
            {
                if (passo > melhor)
                    dir = 'b';
            }
        }

        if (passo == '\0')
        {
            printf ("Erro no caminho\n");
            return 0;
        }
        if (passo == 'e') atual.indiceC--;
        else if (passo == 'd') atual.indiceC++;
        else if (passo == 'c') atual.indiceL --;
        else atual.indiceL++;

        anterior = passo;
        passos++;
        
    }
    return passos;
    
}