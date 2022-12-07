#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define topN 3
void insereTop (int rank[] , int N,int x)
{
    int i =N-1;
    while ( i>= 0 && x > rank[i])
    {
        if (i != N-1) rank [i+1] = rank[i];
        rank[i] = x;
        i--;
    }
}

int main (int argc,char *argv[])
{
    if (argc <= 1) return 1;
    FILE *in = fopen (argv[1], "r");
    int i, atual = 0,x,top[topN];
    
    for (i = 0;i<topN;i++)
        top[i] = 0;

    char linha [100];
    while (fgets(linha,100,in))
    {
        if (sscanf (linha,"%d",&x) == 1)
        {
            atual += x;
        }
        else
        {
            insereTop(top,topN,atual);
            atual = 0;
        }
    }
    insereTop(top,topN,atual);

    int total;
    for (i =0;i<topN;i++)
    {
        int y = top[i];
        printf ("%dº:%d",i+1,y);
        total += y;
    }

    printf ("max: %d" , total);
}