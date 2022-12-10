#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int correrciclos (char *comando,int *cicloInicial,int *atual,int *registo, int salto);

void escrever(int registo, int salto, int atual);

int main (int argc, char *argv[])
{
    if (argc <= 3) return 1;
    FILE *entrada = fopen(argv[1],"r");


    int cicloInicial, salto,atual,registo,sum;

    cicloInicial = atoi (argv[2]);
    salto = atoi (argv[3]);
    atual =1;
    registo = 1;
    sum =0;

    int i;
    char linha[100];
    while(fgets(linha,100,entrada))
    {
        escrever (registo,salto,atual);
        if (atual == cicloInicial ||atual == cicloInicial-1)
        {
            sum += cicloInicial*registo;
            cicloInicial += salto;
        }
        atual++;
        if (sscanf(linha,"addx %d",&i)==1)
        {
            // trata-se de uma adição
            escrever(registo,salto,atual);

            registo += i;
            atual ++;
        }

    }

    printf ("Soma total: %d\n",sum);
    
}

void escrever(int registo, int salto, int atual)
{
    int pos = (atual-1) % salto;
    if (pos >= registo-1 && pos <= registo+1) putchar ('#');
    else putchar('.');
    if (pos == salto -1) putchar('\n');
}

int correrciclos (char *comando,int *cicloInicial,int *atual,int *registo, int salto)
{

    int sum=0;
    escrever(*registo,salto,*atual);
    int x = *cicloInicial,r = *registo,a = *atual;
    if (*atual == x || *atual == x-1)
    {
        *cicloInicial = x + salto;
        sum = x * r;
        putchar('\n');
    }
    int i;
    if (sscanf(comando,"addx %d",&i)==1)
    {
            // trata-se de uma adição
            *atual = a+1;
            escrever(*registo,salto,*atual);
            *registo = r * i;
            *atual = a+1;
    }
    else
    {
        *atual = a+1;
    }

    return sum;
}