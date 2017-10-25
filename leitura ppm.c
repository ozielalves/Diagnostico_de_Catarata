#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    FILE *ptrimagem;
    char nomeimagem[260];
    char key[3];
    printf("Digite o nome do arquivo PPM de entrada: ");
    scanf("%s", nomeimagem);                                // Le o nome do arquivo de entrada
  
    ptrimagem=fopen(nomeimagem , "r") ;                         // Abre o arquivo no modo leitura
    if(ptrimagem == NULL)                                       // Verifica se o arquivo existe e foi aberto
    {
        printf("Erro na abertura do arquivo %s\n", nomeimagem);
        return 0;
    }
  
    // Le dados do cabeçalho
    fscanf(ptrimagem, "%s", key);
  
    // Verifica se a imagem esta em PPM
    if(strcmp(key,"P3") != 0 && strcmp(key,"P6") != 0)
    {
        printf("nao eh PPM\n") ;
        fclose(ptrimagem) ;
        return 0 ;
    }
    else{
        printf("eh PPM\n");
    }
    return 0;
}
