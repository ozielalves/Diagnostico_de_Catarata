#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    char nomeimagem[260];
    char tipo[3];
    FILE *ptrimagem;
    printf("Digite o nome do arquivo PPM de entrada: ");
    scanf("%s", nomeimagem);                               
    ptrimagem=fopen(nomeimagem , "r") ;                        
    if(ptrimagem == NULL)                                       
    {
        printf("Erro na abertura do arquivo %s\n", nomeimagem);
        return 0;
    }
    fscanf(ptrimagem, "%s", tipo);
    // Verifica se a imagem esta em PPM
    if(strcmp(tipo,"P3") != 0 && strcmp(tipo,"P6") != 0)
    {
        printf("nao eh PPM\n") ;
        fclose(ptrimagem) ;
        return 0;
    }
    else
    {
        printf("eh PPM\n");
    }
    return 0;
}
