#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define DELIMITADOR1 "[[2]]"
#define DELIMITADOR2 "[[6]]"


typedef struct valor
{
    bool integer;
    void *valor;
}ITEM;


typedef struct linha
{
    int size;
    int sp;
    ITEM *valores;
} LISTA;

typedef struct stackLista
{
    int size;
    int sp;
    LISTA **valores;
}STACK;


STACK *initStack();

void insereStack(STACK *st,LISTA *valor);


LISTA *initLISTA ();


void imprimeLista(LISTA *c);

void insereLista(LISTA *l, ITEM x);

int inOrder(LISTA *primeiro, LISTA *segundo);

void libertaLista (LISTA *l);

void libertaStack (STACK *s);

void imprimeStack(STACK *s);

LISTA *criaLista(char *str);


int main (int argc, char *argv[])
{
    if (argc <= 1) return 1;
    FILE *entrada = fopen(argv[1],"r");
    LISTA *delim1,*delim2,*part1,*part2;
    char *aux = malloc(1000);
    strcpy(aux,DELIMITADOR1);

    delim1 = criaLista (aux);

    strcpy(aux,DELIMITADOR2);
    delim2 = criaLista (aux);
    free(aux);



    STACK *infDelim1,*entreDelims, *dpsDelim2;
    infDelim1 = initStack();
    entreDelims = initStack();
    dpsDelim2 = initStack();


    size_t size;
    char *linha;
    int i =1,total =0;
    while (getline(&linha,&size,entrada) != -1)
    {
        part1 = criaLista(linha);
        getline(&linha,&size,entrada);
        part2 = criaLista(linha);
        getline(&linha,&size,entrada); // ignorar linha em branco que separa 2 pares

        if (inOrder(part1,part2) == 1) total += i;
        

        if (inOrder(part1,delim1) == 1) insereStack(infDelim1,part1);
        else if (inOrder(part1,delim2) == 1) insereStack(entreDelims,part1);
        else insereStack(dpsDelim2,part1);

        if (inOrder(part2,delim1) == 1) insereStack(infDelim1,part2);
        else if (inOrder(part2,delim2) == 1) insereStack(entreDelims,part2);
        else insereStack(dpsDelim2,part2);
        //libertaLista (part1);
        //libertaLista(part2);
        i++;

    }
    /* Display the lissts in the correct order
    free(linha);
    imprimeStack(infDelim1);
    imprimeLista(delim1);
    putchar('\n');
    imprimeStack(entreDelims);
    imprimeLista(delim2);
    putchar('\n');
    imprimeStack(dpsDelim2);
    */
    int i1 =infDelim1->sp +1;
    int valor = (infDelim1->sp +1) * (i1+entreDelims->sp+1);
    printf("Total na ordem correta %d\n",total);
    printf("%d\n",valor);

    libertaStack(infDelim1);
   libertaStack(entreDelims);
   libertaStack(dpsDelim2);
   libertaLista(delim1);
   libertaLista(delim2);
}

int inOrder(LISTA *primeiro, LISTA *segundo)
{
    int  i = 0,res = 0,valorX,valorY;
    ITEM x,y;
    while (primeiro->sp > i && segundo ->sp >i)
    {
        x = primeiro->valores[i];
        y = segundo->valores[i];

        if (x.integer && y.integer)
        {
            valorX = *(int *) x.valor;
            valorY = *(int *) y.valor;


            //se forem os 2 inteiros
            if (valorX < valorY) res =1;
            else if (valorX > valorY) res =-1;
        }
        else
        {
            LISTA *aux = NULL;
            if (x.integer)
            {
                aux = initLISTA();
                insereLista(aux,x);
                x.integer =false;
                x.valor = aux;                
            }
            else if (y.integer)
            {
                aux = initLISTA();
                insereLista(aux,y);
                y.integer =false;
                y.valor = aux;
            }
            res = inOrder(x.valor,y.valor);
            if (aux != NULL)
            {
                free(aux->valores);
                free(aux);
            }
        }
        if (res ) return res;
        i++;
    }
    if (primeiro->sp == segundo->sp) res = 0;
    else if (primeiro->sp < segundo->sp) res = 1;
    else res = -1;
    return res;
}


LISTA *criaLista(char *str)
{
    for (; str[0] != '\0' && str[0] != '[';str++);
    LISTA *nova = initLISTA();
    ITEM aInserir;
    int parentesis = 1,*x;
    
    str++;

    while (parentesis)
    {
        if (str[0] == ']')
        {
            parentesis  = 0;
        }
        else if (str[0] == '[')
        {
            aInserir.integer = false;

            aInserir.valor = criaLista(str);
            insereLista(nova,aInserir);
            parentesis++;
            str++;
            //avançar ate fim da nova lista
            while (parentesis > 1)
            {
                if (str[0] == ']') parentesis--;
                else if (str[0] == '[') parentesis++;
                str++;
            }           

        }
        else if (str[0] == ',') str++;
        else
        {
            x = malloc (sizeof (int));
            sscanf(str,"%d%s",x,str);
            // ver tamanho do salto por inteiro

            aInserir.integer = true;
            aInserir.valor  =x;
            insereLista(nova,aInserir);
        }
    }
    return nova;
}

void libertaLista (LISTA *l)
{
    if (l == NULL) return;
    int i;
    ITEM x;
    for (i =0;i< l->sp;i++)
    {
        x = l->valores[i];
        if (x.integer)  free (x.valor);
        else libertaLista(x.valor);
    }
    free(l->valores);
    free(l);
}


LISTA *initLISTA ()
{
    LISTA *l = malloc(sizeof(LISTA));
    l->size =1;
    l->sp =0;
    l->valores = malloc(sizeof(ITEM));
    return l;
}

void insereLista(LISTA *l, ITEM x)
{
    if (l->size == l->sp)
    {
        l->size *=2;
        l->valores = realloc(l->valores,l->size * sizeof(ITEM));
    }
    l->valores[l->sp] = x;
    l->sp++;
}

void imprimeLista(LISTA *c)
{


    if (c == NULL) return;
    ITEM x;
    int y;
    putchar('{');
    for (int i =0; i< c->sp;i++)
    {
        x = c->valores[i]; 
        if (x.integer)
        {
            y = *(int *) x.valor;
            printf(" %d ",y );
        }
        else
        {
            imprimeLista ( x.valor);            
        }
    }
    putchar('}');
}


STACK *initStack()
{
    STACK *nova = malloc (sizeof (STACK));
    nova->size = 1000;
    nova->sp = 0;
    nova->valores = malloc (1000 *(sizeof(LISTA *)));
    return nova;
}

void insereStack(STACK *st,LISTA *valor)
{
    if (st->size == st->sp)
    {
        st->size *=2;
        st->valores = realloc(st->valores, st->size * sizeof(LISTA *));
    }
    int i;
    for (i = st->sp-1; i>=0 && inOrder(st->valores[i],valor) == -1 ; i--)
        st->valores[i+1] = st->valores[i];
    
    //if (i < 0) i =0;
    i++;
    st->valores[i] = valor;
    st->sp++;
}

void imprimeStack(STACK *s)
{
    for (int i = 0; i< s->sp; i++)
    {
        imprimeLista(s->valores[i]);
        putchar('\n');
    }
}

void libertaStack (STACK *s)
{
    for (int i = 0; i< s->sp;i++)
    {
        libertaLista(s->valores[i]);
    }
    free(s->valores);
    free (s);
}
