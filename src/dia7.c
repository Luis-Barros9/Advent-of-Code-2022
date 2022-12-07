#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define Origem "/"

typedef struct diretoria DIR;

struct diretoria
{
    DIR *raiz;
    char *nome;
    int tamanhoSemDir;
    DIR **diretorias;
    int nDir;
    int sizeDir;
};

void tiraQuebraLinha (char *linha);

int tamanhoTotal (DIR *d,int *x,int max);

void imprimeDir( DIR *d);

DIR *initDir (char *nome);

void insereDir(DIR *root,DIR *add);

void libertaDir (DIR *d);

int preencherDir (DIR *d,char *linha,FILE *f);

DIR *buscarSubpasta (DIR *aux, char *comando);

int apagarDir(DIR *d,int min, int *smallest);

int main (int argc, char *argv[])
{
    // 4 arguments: ./prog input.txt MaxSizeFolder TotalSpace UnusedNeeded
    if (argc <= 4) return 1;
    FILE *entrada = fopen(argv[1],"r");


    int maxSize,totalSpace,minUnused;
    maxSize = atoi(argv[2]);
    totalSpace = atoi(argv[3]);
    minUnused = atoi(argv[4]);



    DIR *root = initDir(Origem);
    DIR *aux = root;
    int continuacao = 0;


    char linha[1000];
    while (continuacao || fgets(linha,1000,entrada))
    {
        continuacao =0;

        tiraQuebraLinha(linha);
        // trata-se de um comando
        char comando[40];
        comando[0] = '\0';
        int x = sscanf(linha,("$ cd %s"),comando);
        if (x == 1)
        {
            if (strcmp(Origem,comando) == 0)
            {
                aux = root; // passar para a origem
            }
            else if ((strcmp("..",comando) == 0))
            {
                if (aux != NULL) aux = aux->raiz;

            }
            else
            {

                DIR *nova = buscarSubpasta(aux,comando);
                aux = nova;

            }
        }
        else
        {
            // trata-se de um ls logo vamos obter linhas nao comandos
            DIR *inserir = aux;
            if (sscanf(linha,("$ ls %s"),comando) == 1)
            {
                inserir = buscarSubpasta(aux,comando);
                if (inserir == aux) 
                {
                    return 1;
                }

            }
            continuacao = preencherDir (inserir,linha,entrada);
            
        }
    }
    int sum = 0,used;
    used = tamanhoTotal (root,&sum,maxSize);
    printf("Tamanho : %d\n",used,sum);
    int libertar = (minUnused) - (totalSpace-used);
    (void) apagarDir(root,libertar,&minUnused);
    printf ("Apagar diretoria com tamanho %d\n", minUnused);
}

void imprimeDir( DIR *d)
{
    if (d == NULL)
    {
        return;
    }
    printf ("Pasta: %s %d\t",d->nome,d->tamanhoSemDir);
    printf("Subpastas:");
    for (int i = 0; i<d->nDir;i++)
    {
        printf("%s ",d->diretorias[i]->nome);
    }
    putchar('\t');
    if (d->raiz != NULL) printf("Raiz: %s\n",d->raiz->nome);

}

    

DIR *initDir (char *nome)
{
    DIR *d = malloc (sizeof(DIR));
    d->nome = strdup (nome);
    d->raiz = NULL;
    d->tamanhoSemDir = 0;
    d->nDir =0;
    d->sizeDir = 5; // começa com 5 capacidade para 5 subpastas
    d->diretorias = malloc(5 * sizeof(DIR *));
    return d;
}

void insereDir(DIR *root,DIR *add)
{
    if (root->sizeDir == root->nDir)
    {
        root->sizeDir += 5;
        root->diretorias = realloc(root->diretorias,root->sizeDir * (sizeof (DIR)));
    }
    root->diretorias[root->nDir] = add;
    root->nDir++;
    add->raiz = root;
}

void libertaDir (DIR *d)
{
    if (d == NULL) return;
    for (int i =0;i<d->nDir;i++)
    {
        libertaDir(d->diretorias[i]);
    }
    free(d->nome);
    free(d->diretorias);
    free(d);
    
}

int preencherDir (DIR *d,char *linha,FILE *f)
{
    char *token;
    char *x = fgets(linha,1000,f);
    while (x)
    {
        tiraQuebraLinha(linha);
        if (linha[0] == '$') return 1;
        token = strtok (linha, " ");
        if (strcmp("dir", token))
        {
            //não temos uma diretoria
            int t = atoi(token);
            d->tamanhoSemDir += t;
        }
        else
        {
            // obter nome do fihciero
            token = strtok(NULL," ");
            DIR *novo = initDir(token);
            insereDir(d,novo);
        }
        x = fgets(linha,1000,f);
    }
    return 0;
}

int apagarDir(DIR *d,int min, int *smallest)
{
    int total = d->tamanhoSemDir;
    int i,x,encontrei;
    for (i = 0,encontrei = 1; i< d->nDir;i++)
    {
        DIR *prox = d->diretorias[i];
        total += apagarDir (prox,min,smallest);
    }
    if (encontrei && total >= min && total <= *smallest ) *smallest = total;
    return total;
}


int tamanhoTotal (DIR *d,int *x, int max)
{
    // retorna o tamanho de uma pasta
    if (d == NULL) return 0;
    int t = d->tamanhoSemDir;
    for (int i = 0; i<d->nDir;i++)
    {
        DIR *subFolder =d->diretorias[i];
        t += (tamanhoTotal(subFolder,x,max));
    }
    if (x != NULL && t<max) *x = *x + t;
    return t;
}

void tiraQuebraLinha (char *linha)
{
    int t = strlen (linha);
        if (t > 0 && linha [t-1] == '\n') linha [t-1] = '\0';
}

DIR *buscarSubpasta (DIR *aux, char *comando)
{

    int i=0;
    while (i<aux->nDir && strcmp(aux->diretorias[i]->nome,comando))
    {
        i++;
    }
    if (i != aux->nDir) aux = aux->diretorias[i];
    return aux;
}