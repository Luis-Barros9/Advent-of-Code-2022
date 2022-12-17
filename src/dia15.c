#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


typedef struct posicao
{
    int x;
    int y;    
}POS;

typedef struct sensor
{
    POS sensor;
    POS beacon; // beacon mais próximo
} SENS;

typedef struct stack
{
    int size;
    int sp;
    void **valores;
}STACK;

typedef struct intrevalo
{
    int inicio;
    int fim;
}INT;


void libertaStack (STACK *st, void (*funcao) (void *));


INT *criaIntrevalo (int i,int f);

STACK *initStack();

int distancia(POS a, POS b);


int distBeacon(SENS *sensor);


SENS *criaSensor(int xS,int ys,int xB,int yB);

void insereStack(STACK *s, void *valor);

bool naoExisteNaStack(STACK *stInt,void *valor);


STACK *simplificaStIntrevalos (STACK *stnt);


long int naoContemBeacon (STACK *stSensores, int y);

long int contaTamanho (INT *i);

bool contido (int x, INT *i);



int main (int argc, char *argv[])
{
    if (argc <= 4) return 1;
    FILE *entrada = fopen(argv[1],"r");
    
    
    int y = atoi(argv[2]);

    int xMin = atoi (argv[3]);
    int yMin = atoi (argv[4]);

    STACK *sensores = initStack();

    size_t size;
    char *linha;

    int xSensor,ySensor,xBeacon,yBeacon;
    SENS *sensor;
    while (getline(&linha,&size,entrada) != -1)
    {
        sscanf(linha,"Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",&xSensor,&ySensor,&xBeacon,&yBeacon);

        sensor = criaSensor(xSensor,ySensor,xBeacon,yBeacon);
        insereStack(sensores,sensor);
    }

    int livresLinhaY = naoContemBeacon(sensores,y);

    printf("Na linha %d há %d posições onde não pode existir beacon\n",y,livresLinhaY);


    libertaStack(sensores,free);
}


STACK *initStack()
{
    STACK *nova =malloc (sizeof (STACK));
    nova->size =4;
    nova->sp =0;
    nova->valores = malloc (4 * sizeof(void *));
    return nova;
}

void insereStack(STACK *s, void *valor)
{
    if (s->sp == s->size)
    {
        s->size *=2;
        s->valores = realloc(s->valores,s->size * sizeof(void *));
    }
    s->valores[s->sp] = valor;
    s->sp++;
}

void libertaStack (STACK *st, void (*funcao) (void *))
{
    if (funcao != NULL)
    {
        for (int i =0;i<st->sp;i++)
            funcao(st->valores[i]);
    }
    free(st->valores);
    free(st);
}

bool naoExisteNaStack(STACK *stInt,void *valor)
{
    int i,aux,v;
    v = *(int *)valor;
    for (i =0; i<stInt->sp;i++)
    {   
        aux = *(int *) stInt->valores[i];
        if (v == aux) return false;
    }

    return true;
}

SENS *criaSensor(int xS,int yS,int xB,int yB)
{
    SENS *novo = malloc (sizeof(SENS));
    
    POS s,b;
    
    s.x = xS; s.y =yS;
    b.x = xB; b.y =yB;

    novo->sensor = s;
    novo->beacon = b;

    return novo;
}

long int naoContemBeacon (STACK *stSensores,int y)
{
    STACK *ocupados = initStack();
    STACK *beacons = initStack();
    SENS *sensor;


    int *s,i,j,dist,xInicial,dist2;
    POS inicio,beacon;
    for (i =0 ;i<stSensores->sp;i++)
    {
        sensor = stSensores->valores[i];
        beacon = sensor->beacon;
        if (beacon.y == y)
        {
            xInicial =sensor->beacon.x;
            if (naoExisteNaStack(beacons,&xInicial))
            {
                s = malloc(sizeof(int));
                *s = xInicial;
                insereStack(beacons,s);
            }
        }

        dist = distBeacon(sensor);
        xInicial = sensor->sensor.x;
        inicio.x = xInicial;
        inicio.y = y;
        dist2 = distancia(inicio,sensor->sensor);
        if (dist2 <= dist)
        {
            j = dist-dist2;
            INT *intrevalo = criaIntrevalo(xInicial-j,xInicial+j);
            insereStack(ocupados,intrevalo);
        }
    }

    ocupados = simplificaStIntrevalos(ocupados);
    
    long int resultado = 0;
    for (i =0;i<ocupados->sp;i++)
    {
        resultado += contaTamanho (ocupados->valores[i]);
    }
    
    for (j =0; j<beacons->sp;j++)
    {
        int x = *(int *) beacons->valores[j];
        for (i =0;i<ocupados->sp;i++)
        {
            if (contido(x,ocupados->valores[i])) resultado--;
        }
    }

    return resultado;
}

bool juntaIntrevalos (INT *dest,INT *prev)
{
    // retorna false se os intrevalos intersetam se e true se não, se intersetarem-se , copiar a união para dest
    bool status =false;
    if (dest->inicio <= prev->inicio)
    {
        if (dest->fim >= prev->inicio)
        {
            status = true;
            if (prev->fim > dest->fim) dest->fim = prev->fim;
        }

    }
    else
    {
        if (dest->inicio <= prev->fim)
        {
            dest->inicio = prev->inicio;
            status = true;
            if (prev->fim > dest->fim) dest->fim = prev->fim;
        }

    }
    
    return status;
}

STACK *simplificaStIntrevalos (STACK *stnt)
{
    if (stnt->sp < 2) return stnt;
    STACK *naoRemovidos = initStack();
    int i,j,cond;
    INT *temp,*original;
    for (i = stnt->sp -1; i>0;i--)
    {
        temp = stnt->valores[i];
        cond = 1;
        for (j = 0;j<i && cond;j++)
        {
            original = stnt->valores[j];
            if (juntaIntrevalos(original,temp))
            {
                cond =0;
                free(temp);
            }
        }
        if (cond) insereStack(naoRemovidos,temp);
    }
    insereStack(naoRemovidos,stnt->valores[0]);   
    
    free (stnt->valores);
    free(stnt);

    return naoRemovidos;
}

long int contaTamanho (INT *i)
{
    long int res =1 + i->fim - i->inicio;
    return res;
}

bool contido (int x, INT *i)
{
    return (x <= i->fim && x >= i->inicio);
}

/*
    função funciona para inputs com valores pequenos mas para valores grandes não termina
int naoContemBeacon (STACK *stSensores, int y)
{

    STACK *ocupados = initStack();
    STACK *beacons = initStack();
    int *s,i,dist,xInicial,dist2;
    POS inicio,beacon;
    SENS *sensor;
    for (i =0 ;i<stSensores->sp;i++)
    {
        sensor = stSensores->valores[i];
        beacon = sensor->beacon;
        if (beacon.y == y)
        {
            xInicial =sensor->beacon.x;
            if (naoExisteNaStack(beacons,&xInicial))
            {
                s = malloc(sizeof(int));
                *s = xInicial;
                insereStack(beacons,s);
            }
        }



        dist = distBeacon(sensor);
        xInicial = sensor->sensor.x;
        inicio.x = xInicial;
        inicio.y = y;
        dist2 = distancia(inicio,sensor->sensor);
        if (dist2 <= dist)
        {
            s = malloc (sizeof(int));
            *s =xInicial;
            if(naoExisteNaStack(beacons,s)&& naoExisteNaStack(ocupados,s)) insereStack(ocupados,s);
            else free(s);
            dist2++;
            for (int i = 1; dist2 <= dist;dist2++,i++ )
            {
                s = malloc (sizeof(int));
                *s =xInicial+i;
                if(naoExisteNaStack(beacons,s)&& naoExisteNaStack(ocupados,s)) insereStack(ocupados,s);
                else free(s);
                s = malloc (sizeof(int));
                *s =xInicial-i;
                if(naoExisteNaStack(beacons,s)&& naoExisteNaStack(ocupados,s)) insereStack(ocupados,s);
                else free(s);
            }

        }
    }

    int resultado = ocupados->sp;

    libertaStack(ocupados,free);
    libertaStack(beacons,free);

    return resultado;
}
*/

INT *criaIntrevalo (int i,int f)
{
    INT *intrevalo = malloc(sizeof(INT));
    intrevalo->inicio =i;
    intrevalo->fim =f;
    return intrevalo;
}


int distancia(POS a, POS b)
{
    int distX,distY;
    distX = a.x - b.x;
    distY = a.y - b.y;
    if (distX < 0) distX *= -1;
    if (distY < 0) distY *= -1;
    return distX + distY;
}

int distBeacon(SENS *sensor)
{
    int dist;
    POS s,b;
    s = sensor->sensor;
    b = sensor->beacon;

    dist = distancia (s,b);
    
    return dist;

}