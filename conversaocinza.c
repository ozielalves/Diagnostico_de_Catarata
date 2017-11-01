#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned char** aloca(int m, int n)
{
    int i;
    unsigned char **M;
    M = (unsigned char**) malloc(n * sizeof(unsigned char*));
    for(i = 0; i < n; i++)
    {
        M[i] = (unsigned char*) malloc(m * sizeof(unsigned char));
    }
    return M;
}
int conversaocinza(unsigned char r, unsigned char g, unsigned char b)
{
    int new;
    new = 0.3*r + 0.59*g + 0.11*b;
    return new;
} 
int main()
{
    FILE *ptrimagem; 
    FILE *ptrnovaimagem;
    char nomeimagem[260];
    char nomenovaimagem[260];
    char key[2];
    unsigned char red, green, blue;
    int i, j, m, n, max;
  
    printf("Digite o nome do arquivo PPM de entrada: ");
    scanf("%s", nomeimagem);
    printf("Digite o nome do arquivo PGM de saida: ");
    scanf("%s", nomenovaimagem);
  
    ptrimagem = fopen(nomeimagem , "r") ;
    if(ptrimagem == NULL) // Verificase o arquivo existe e foi aberto
    {
        printf("Erro na abertura do arquivo %s\n", nomeimagem);
        return 0 ;
    }
    fscanf(ptrimagem, "%s", key);
    if(strcmp(key,"P3") != 0)
    {
        printf("Arquivo nao e um PPM\n") ;
        fclose(ptrimagem) ;
        return 0 ;
    }
    fscanf(ptrimagem, "%d %d %d", &m, &n, &max) ;
    /* m guarda a largura da imagem (colunas da matriz)
     n guarda a altura da imagem (linhas da matriz)
    max guarda o valor máximo da escala
    Matriz para guardar a imagem em tons de cinza*/
    unsigned char **matrizcinza;
    matrizcinza = aloca(m, n);
    /* Lê os canais de cores de cada pixel ij da imagem
     e salva na matriz matrizcinza o seu correspondente
     em cinza*/
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            fscanf(ptrimagem, "%c%c%c", &red, &green, &blue);
            matrizcinza[i][j] = conversaocinza(red, green, blue);
        }
    }
  
    ptrnovaimagem = fopen(nomenovaimagem , "w"); 
    fprintf(ptrnovaimagem,"P5\n"); 
    fprintf(ptrnovaimagem, "%d %d\n %d\n", m, n, max); 
  
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            // Escreve os valores de cada pixel no arquivo
            fprintf(ptrnovaimagem, "%c", (char) matrizcinza[i][j]);
        }
    }
    fclose(ptrimagem);
    fclose(ptrnovaimagem);
    for(i = 0; i < n; i++)
    {
        free(matrizcinza[i]);
    }
    free(matrizcinza);
    return 0;
}
