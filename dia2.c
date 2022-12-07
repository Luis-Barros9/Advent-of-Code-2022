#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int scorev1 (char *linha);

int scorev2 (char *linha);


int socreMao (char c);

int socreResult (char c);

int main (int argc,char *argv[])
{
    if (argc <= 1) return 1;
    FILE *in = fopen (argv[1], "r");
    char linha[8];
    int total = 0;
    while (fgets (linha,8,in))
    {
        total += scorev2 (linha);
    }
    printf("%d",total);
}


int scorev1 (char *linha)
{
    char op,eu;
    int total = 0;
    sscanf (linha , "%c %c",&op,&eu);
    op += 'X' -'A';
    
    if (eu == 'X') 
    {
        total++;
        if (op == 'Z') total += 6;
    }
    else if (eu == 'Y') 
    {
        total += 2;
        if (op == 'X') total += 6;

    }
    else 
    {
        total += 3;
        if (op == 'Y') total += 6;

    }
    if (eu == op) total +=3;
    return total;

}

int scorev2 (char *linha)
{
    char op,eu;
    int total = 0;
    sscanf (linha , "%c %c",&op,&eu);
    total += socreResult (eu);
    char jogo;
    if (eu == 'X') 
    {
        if (op == 'A') jogo ='C';
        else if (op == 'B') jogo = 'A';
        else jogo = 'B';
    }
    else if (eu == 'Y') 
    {
        jogo = op;
    }
    else 
    {
        if (op == 'A') jogo ='B';
        else if (op == 'B') jogo = 'C';
        else jogo = 'A';

    }
    total += socreMao(jogo);
    return total;
}


int socreMao (char c)
{
    int x;
    if (c == 'A') x= 1;
    else if (c == 'B') x = 2;
    else x = 3;
    return x;
}

int socreResult (char c)
{
    int x;
    if (c == 'X') x= 0;
    else if (c == 'Y') x = 3;
    else x = 6;
    return x;
}