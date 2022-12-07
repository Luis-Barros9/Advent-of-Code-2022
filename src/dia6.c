#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TamanhoTodosDiferentes 14

void printAteN(char *s,int N)
{
    for (int i =0; i<N;i++)
        putchar(s[i]);
}


int main (int argc, char *argv[])
{
    if (argc <= 1) return 1;
    FILE *entrada = fopen(argv[1],"r");
    char marker [TamanhoTodosDiferentes+1];// espaço para o marker e \0
    int i =0,x = 0,j;
    char c;
    while (i < TamanhoTodosDiferentes && (c = getc(entrada)) != EOF)
    {
        x++;
        int insere = 1;
        for (j = 0; insere && j < i;j++)
        {
            if (c ==  marker[j])
            {
                insere = 0;
            }
        }
        if (insere==1)
        {
            marker[i] = c;
            i++;
        }
        else
        {
            int n,newi;
            for (n = j,newi = 0;n<i;n++)
            {
                marker[newi] = marker [n];
                newi++;
            }
            i = newi;
            marker[i] = c;
            i++;           
        }
    }
    
    printf ("Marker pos : %d", x);
}