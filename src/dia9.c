#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




typedef struct pos
{
    int linha;
    int coluna;
}POS;

typedef struct ponta
{
    int size;
    int nPos;
    POS *poisicoes; // arrray de todas as posições ocupadas
    POS atual;   // posição atual
}CAUDA;

CAUDA *initCauda(POS *inicial);

void mover (POS *p, char dir);

POS corrigir (POS *cabeca, POS atual);

void inserePos (POS atual, CAUDA *t);

int posIguais (POS x, POS y);


int main (int argc, char *argv[])
{
    if (argc <= 2) return 1;
    FILE *entrada = fopen(argv[1],"r");


    if (entrada == NULL) return 1;

    int i,nCaudas = atoi(argv[2]);

    POS *cabeca = malloc (nCaudas*sizeof(POS));
    // todas as caudas exceto a ponta
    for (i = 0 ;i<nCaudas;i++)
    {
        cabeca[0].coluna =0;
        cabeca[0].linha =0;
    }
    
    CAUDA *cauda = initCauda(cabeca);
    char linha[1000];
    char dir;
    int n,j;
    POS nova;
    while (fgets(linha,1000,entrada))
    {
        if (sscanf (linha,"%c %d",&dir,&n) == 2);
        {
            for (i =0; i < n;i++)
            {
                mover(cabeca,dir);
                for (j = 1;j<nCaudas;j++)
                {
                    cabeca[j] =corrigir (&cabeca[j-1],cabeca[j]); // corrigir a posição de acordo com a ponta anterior
                }
                nova = corrigir(&cabeca[nCaudas-1],cauda->atual);
                inserePos(nova,cauda);
            }
        }
    }
    printf ("A cauda ocupou %d posições\n", cauda->nPos);
}

CAUDA *initCauda(POS *inicial)
{
    CAUDA *new = malloc (sizeof(CAUDA));
    new->atual = *inicial;
    new->nPos =1;
    new->size =4;
    new->poisicoes = malloc (4 * sizeof(POS));
    new->poisicoes[0] = *inicial;
    return new;
}

void mover (POS *p, char dir)
{
    if (dir == 'U') p->linha--;
    else if (dir == 'D') p->linha++;
    else if (dir  == 'L') p->coluna--;
    else p->coluna++;
}

POS corrigir (POS *cabeca, POS atual)
{
    int difHorizontal, difVertical;
    difHorizontal = cabeca->coluna- atual.coluna;
    difVertical = cabeca->linha - atual.linha;
    if (-2 < difVertical  && difVertical < 2 &&  -2 < difHorizontal && difHorizontal < 2) 
    {
        return atual; // a cabeça está junta à corda
    }

    if (difHorizontal)
    {
        if (difHorizontal > 0) atual.coluna ++;
        else  atual.coluna--;
    }
    if (difVertical)
    {
        if (difVertical > 0) atual.linha ++;
        else atual.linha--;
    }
    return atual;
}

int posIguais (POS x, POS y)
{
    return (x.coluna == y.coluna && x.linha == y.linha);
}

void inserePos (POS new, CAUDA *t)
{
    if (posIguais (new,t->atual)) return;
    t->atual = new;
    int i;
    for (i =0;i<t->nPos && !posIguais(new,t->poisicoes[i]) ;i++);
    if (i == t-> nPos)
    {
        if (t->size == i)
        {
            t->size *=2;
            t->poisicoes = realloc (t->poisicoes, t->size * sizeof(POS));
        }
        t->poisicoes[i] = new;
        t->nPos ++;
    }
}