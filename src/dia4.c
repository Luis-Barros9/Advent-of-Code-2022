#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>



bool overlap (int y1,int y2, int x1 ,int x2)
{
    return !(y2 < x1 || y1 > x2);
}


bool contemEntre (int y1,int y2, int x1 ,int x2)
{
    return ((y1 <= x1 && y2 >= x2 ) || (x1 <= y1 && x2 >= y2 ));
}
int main (int argc, char *argv[])
{
    if (argc <= 1) return 1;
    FILE *entrada = fopen(argv[1],"r");
    char linha [30];
    int fully = 0,overlaps =0;
    int inicio1,fim1,inicio2,fim2;
    while (fgets(linha,30,entrada))
    {
        sscanf (linha,"%d-%d,%d-%d",&inicio1,&fim1,&inicio2,&fim2);
        if (contemEntre(inicio1,fim1,inicio2,fim2)) 
        {
            fully++;
            overlaps++;
        }
        else if (overlap(inicio1,fim1,inicio2,fim2))   overlaps++;        
    }
    printf ("Fully : %d\n Overlaps : %d",fully,overlaps);
    
}
