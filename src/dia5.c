#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NumeroPilhas 9

typedef struct stack{
    char *caixas;
    int sp;
    int size;
} *Stack;

Stack initStack (char *c,int N);


void executaLinha (Stack *pilhas,char *linha,int versao);


char pop (Stack s);

void push(Stack s,char c);

void printTopo (Stack *pilhas);

void passaPara (int  N, Stack out,Stack in);


int main (int argc, char *argv[])
{
    if (argc <= 1) return 1;
    FILE *entrada = fopen(argv[1],"r");
    Stack pilhas[NumeroPilhas]; // 9 pilhas de caixas
    char *pilhainicial[NumeroPilhas] = {"GDVZJSB","ZSMGVP","CLBSWTQF","HJGWMRVQ","CLSNFMD","RGCD","HGTRJDSQ","PFV","DRSTJ"};  
    char *linha;

    for (int i = 0;i< NumeroPilhas; i++)
    {
        linha =pilhainicial[i];

        pilhas[i] = initStack (linha, strlen(linha));
    }


    char input [100];

    while (fgets(input,100,entrada))
    {
        executaLinha (pilhas,input,2);
    }
    printTopo (pilhas);

}

void executaLinha (Stack *pilhas,char *linha,int versao)
{
    int nr=-1,out = -1,in = -1;
    sscanf(linha,"move %d from %d to %d ",&nr,&out,&in);
    out--;in--;// passar para indice
    Stack saida,entrada;
    if (nr < 1 || out < 0 || out > NumeroPilhas ||in < 0 || in > NumeroPilhas )
    {
        printf("Deu merda\n");
        return;
    }
    // pilhas onde guardar e ir buscar as caixas
    saida = pilhas[out];
    entrada = pilhas[in];

    if (versao == 1)
    {
        char caixa;
        while (saida->sp > 0 && nr > 0)
        {
            caixa = pop(saida);
            push (entrada,caixa);
            nr --;
        }
    }
    else passaPara(nr,saida,entrada);
}

Stack initStack (char *c,int N)
{
    Stack s = malloc(sizeof(struct stack));
    s->sp =N;
    s->size = N;
    s->caixas = malloc (s->size);
    strcpy(s->caixas,c);
    return s;
}

char pop (Stack s)
{
    if (s->sp == 0) return 0;
    s->sp--;
    return s->caixas[s->sp];
}

void push(Stack s,char c)
{
    if (s->sp == s->size)
    {
        s->size += 5;
        s->caixas = realloc (s->caixas,s->size);
    }
    s->caixas[s->sp] = c;
    s->sp++;
}

void printTopo (Stack *pilhas)
{
    int i;
    char c;
    for (i = 0; i< NumeroPilhas ; i++)
    {
        Stack s = pilhas[i];
        if (s->sp > 0)
        {
            c = s->caixas[s->sp-1];
            putchar (c);
        }
    }
    putchar ('\n');
}


void passaPara (int  N, Stack out,Stack in)
{
    int i = out->sp-N;
    if (i<0) i =0;

    while (i<out->sp)
    {
        push(in,out->caixas[i]);
        i++;
    }
    if (N > out->sp) out->sp = 0;
    else out->sp -= N;

}