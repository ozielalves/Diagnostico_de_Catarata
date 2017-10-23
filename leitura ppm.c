#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
// Recebe inteiros M e N que representam a largura
// e a altura da imagem e aloca memoria para uma
// matriz de dimencoes (N x M)
  
int main()
{
    FILE *image; // Ponteiro para o arquivo que contem a imagem que vamos tratar
    FILE *newImage; // Ponteiro para o arquivo onde vamos salvar a imagem tratada
    char image_name[256];
    char newImage_name[256];
    char key[128];
    unsigned char red, green, blue;
    int i, j, m, n, max;
  
    printf("Digite o nome do arquivo PPM de entrada: ");
    scanf("%s", image_name); // Le o nome do arquivo de entrada
  
    image = fopen(image_name , "r") ; // Abre o arquivo no modo leitura
    if(image == NULL) // Verificase o arquivo existe e foi aberto
    {
        printf("Erro na abertura do arquivo %s\n", image_name);
        return 0 ;
    }
  
    // Le dados do cabeçalho
    fscanf(image, "%s", key);
  
    // Verifica se a imagem esta em PPM
    if(strcmp(key,"P6") != 0)
    {
        printf("Arquivo nao e um PPM\n") ;
        fclose(image) ;
        return 0 ;
    }
  
    // Le os outros dados do cabecalho
    fscanf(image, "%d %d %d", &m, &n, &max) ;
    // m guarda a largura da imagem (colunas da matriz)
    // n guarda a altura da imagem (linhas da matriz)
    // max guarda o valor máximo da escala
    return 0;
}