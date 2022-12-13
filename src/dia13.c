#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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


LISTA *initLISTA ();


void imprimeLista(LISTA *c);

void insereLista(LISTA *l, ITEM x);


LISTA *criaLista(char *str);


int main (int argc, char *argv[])
{
    if (argc <= 1) return 1;
    FILE *entrada = fopen(argv[1],"r");

    LISTA *part1,*part2;
    size_t size;
    char *linha;
    while (getline(&linha,&size,entrada) != -1)
    {
        part1 = criaLista(linha);
        getline(&linha,&size,entrada);
        part2 = criaLista(linha);
        getline(&linha,&size,entrada); // ignorar linha em branco que separa 2 pares

    }
    free(linha);
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