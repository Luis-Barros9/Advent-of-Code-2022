#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct monkey
{
    int *worrylevels;
    int items;
    int inspections;
    char *operation;
    int divisivel;
    int ifTure;
    int ifFalse;
    int size;
} MONKEY;


long long int monkeybuisness (MONKEY *macacos, int numeroMacacos);

void correrRonda (MONKEY *macacos,int numeroMacacos ,int aborrecimento, int minMultComum);

void atirarItens (MONKEY *macacos,int nMacaco ,int aborrecimento,int minMultComum);

MONKEY initMonkey ();

void insereItem (MONKEY *m,int novo);


char *avancaAtePontos (char *str);

void inicializaItens (MONKEY *m,char *linha);

int operacao (char *operacao, int old,int divisao, int minMultComum);





int main (int argc, char *argv[])
{
    // ./prog <inputfile> <divideby> <numberofrounds>
    if (argc <= 3) return 1;
    FILE *entrada = fopen(argv[1],"r");
    
    int divisao = atoi(argv[2]);
    int rounds = atoi(argv[3]);
    int numeroMacacos,size,t,x,minMultCom;
    
    minMultCom = 1;
    t = sizeof(MONKEY);
    numeroMacacos =0;
    size = 4;

    MONKEY *monkey = malloc (t*size);
    
    
    char linha[1000],*aux;

    while (fgets(linha,1000,entrada))
    {
        if (sscanf(linha,"Monkey %d:",&x) ==1)
        {
            if (numeroMacacos ==size)
            {
                size *=4;
                monkey =realloc(monkey, t *size );
            }
            monkey[numeroMacacos] = initMonkey();

            
            fgets (linha,1000,entrada);
            aux = avancaAtePontos(linha);
            inicializaItens(monkey+numeroMacacos,aux);

            fgets (linha,1000,entrada);
            aux = avancaAtePontos(linha);
            monkey[numeroMacacos].operation = strdup(aux);
    
            fgets (linha,1000,entrada);
            aux = avancaAtePontos(linha);
            sscanf (aux, "divisible by %d",&x);
            if (minMultCom % x) minMultCom *= x; 
            monkey[numeroMacacos].divisivel = x;

            fgets (linha,1000,entrada);
            aux = avancaAtePontos(linha);
            sscanf(aux,"throw to monkey %d",&x);
            monkey[numeroMacacos].ifTure = x;

            fgets (linha,1000,entrada);
            aux = avancaAtePontos(linha);
            sscanf(aux,"throw to monkey %d",&x);
            monkey[numeroMacacos].ifFalse = x;

            numeroMacacos++;
        }
    }
    printf ("Minimo mult comum: %d\n",minMultCom);
    while (rounds > 0)
    {
        correrRonda (monkey,numeroMacacos,divisao,minMultCom);
        rounds--;
    }
    long long level = monkeybuisness(monkey,numeroMacacos);

    printf("Monkey buisness leves: %ld\n", level);
    

}

char *avancaAtePontos (char *str)
{
    while (*str != '\0' && *str != ':')
    {
        str++;
    }
    if (*str == ':') str += 2;
    return str;
    
}

MONKEY initMonkey ()
{
    MONKEY novo;
    novo.items =0;
    novo.size =4;
    novo.operation = NULL;
    novo.inspections =0;
    novo.worrylevels = malloc (sizeof(int)*4);
    return novo;
}



void inicializaItens (MONKEY *m,char *linha)
{
    char *token;
    int x;
    token = strtok(linha,", ");
    while (token != NULL)
    {
        sscanf(token,"%d",&x);
        insereItem(m,x);
        token = strtok(NULL,", ");
    }
    
}


void insereItem (MONKEY *m,int novo)
{
    if (m->size == m->items)
    {
        m->size *=2;
        m->worrylevels = realloc(m->worrylevels, (sizeof(int) * m->size));
    }
    m->worrylevels[m->items] =novo;
    m->items++;
}

int operacao (char *operacao, int old,int divisao, int minMultComum)
{
    long long int new=0;
    char op;
    char aux[10];
    int valor;

    sscanf (operacao,"new = old %c %s",&op,aux);
    
    if (sscanf (aux,"%d",&valor) != 1) valor = old;
    
    if (op == '*') new = ((long long int)old * (long long int)valor);
    else new = old + valor;
    new /= divisao;
    
    return (new % minMultComum);
}


void atirarItens (MONKEY *macacos,int nMacaco,int aborrecimento, int minMultComum)
{
    MONKEY atirador = macacos[nMacaco];
    int x =  macacos[nMacaco].items;
    int i,recetor,atual,valor;
    for (i =0;i < x;i++)
    {
        valor =atirador.worrylevels[i];
        
        
        atual = operacao(atirador.operation ,valor,aborrecimento,minMultComum);
        

        if (atual % atirador.divisivel == 0) 
        {
            recetor = atirador.ifTure;
        }
        else 
        {
            recetor = atirador.ifFalse;
        }

        insereItem(&macacos[recetor],atual);
    }

    macacos[nMacaco].inspections += x;

    macacos[nMacaco].items = 0;
}


void correrRonda (MONKEY *macacos,int numeroMacacos,int aborrecimento, int minMultomum)
{
    int i;
    for (i = 0;i < numeroMacacos ; i++)
    {
        atirarItens (macacos ,i,aborrecimento,minMultomum);
    }

}

long long int monkeybuisness (MONKEY *macacos, int numeroMacacos)
{
    long long int max1 , max2,inspecoes;
    max1 = max2 = 0;


    for (int i =0;i<numeroMacacos; i++)
    {
        inspecoes = macacos[i].inspections;
        if (inspecoes > max1)
        {
            max2 = max1;
            max1 = inspecoes;

        }
        else if (inspecoes > max2)
            max2 = inspecoes;
        
    }

    inspecoes = max1 * max2;

    return inspecoes;
    
}
