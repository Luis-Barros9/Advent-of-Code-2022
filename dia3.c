#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define Divisoes 3


char *interseta (char *s1 , char *s2);

int prioridadeComum (char **linha,int nPartes);

int priorities( char n); 

char comum (char *s1,int n1,char *s2,int n2);

void tiraQuebraLinha (char *linha)
{
    int t = strlen (linha);
        if (t > 0 && linha [t-1] == '\n') linha [t-1] = '\0';
}

int main (int argc, char *argv[])
{
    if (argc <= 1) return 1;
    FILE *entrada = fopen(argv[1],"r");  

    int  t = Divisoes * (sizeof (char *));
    char **linha = malloc (t);
    for (int x =0;x <Divisoes;x++)
        linha[x] = malloc (100);

    t = 0;
    int sum =0;
    int i = 1;
    while (fgets(linha[0],100,entrada))
    {
        tiraQuebraLinha(linha[0]);
        while (i<Divisoes && fgets(linha[i],100,entrada) )
        {
            tiraQuebraLinha(linha[i]);            
            i++;
        }
        sum += prioridadeComum (linha,i);
        i = 1;     
    }
    
    for (int x =0;x <Divisoes;x++)
        free(linha[x]);
    free(linha);
    printf ("Total : %d\n",sum);
}

char *interseta (char *s1 , char *s2)
{
    if (s1 == NULL || s2 == NULL) return NULL;
    char prev [1000],*resultado;
    int i =0,x,y,aux;
    char c;
    
    for ( x= 0;s1[x] != '\0';x++)
    {
        aux =0;
        c = s1[x];
        y = 0;
        while (s2[y] != '\0' && s2[y] != c)
        {
            y++;
        }
        if (s2[y] == c)
        {
            while (aux <i && prev[aux] != c)
                aux ++;
            if (aux == i)
            {
                prev [i] = c;
                i++;
            }          
        } 
    }
    prev[i] = '\0';
    resultado = strdup(prev);
    return resultado;
}

int maiorPrioridade(char *linha)
{
    if (linha == NULL) return 0;
    int i,max = 0;
    for (i =0;linha[i] != '\0';i++)
    {
        int x = priorities (linha[i]);
        if (x > max) max = x;
    }
    return max;
}

char comum (char *s1,int n1,char *s2,int n2)
{
    char c = '\0',c1;
    for (int i =0;i<n1 ;i++)
    {
        c1 = s1[i];
        for (int x =0; x < n2;x++)
        {
            if (c1 == s2[x])
            {
                c = c1;
                return c;
            }
        }
    }
    return c;
}

int prioridadeComum (char **linhas, int nParte)
{
    if (nParte < 2) return 0;
    int r =0;
    int particao;
    // partir a string em subpartes devidamente espaçadas
    int i =2;
    char *nova = interseta (linhas[0],linhas[1]);
    while (i<nParte)
    {
        char *axu = nova;
        nova = interseta(nova, linhas[i]);
        if (axu != NULL)free (axu);
        i++;
    }
    r = maiorPrioridade(nova);
    if (nova != NULL )free (nova);
    return r; 
}


int priorities( char n)
{
    int r ;
    if (islower(n)) r = n - 'a';
    else r = n +26 -'A';
    r++;
    return r; 
}