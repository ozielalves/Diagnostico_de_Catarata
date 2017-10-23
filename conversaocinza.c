#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
// Recebe inteiros M e N que representam a largura
// e a altura da imagem e aloca memoria para uma
// matriz de dimencoes (N x M)
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
  
// Metodo da combinacao linear
int combLinear(unsigned char r, unsigned char g, unsigned char b)
{
    int new;
    new = 0.3*r + 0.59*g + 0.11*b;
    return new;
}
  
// Metodo da media
int media(unsigned char r, unsigned char g, unsigned char b)
{
    int new;
    new = (r + g + b) / 3;
    return new;
}
  
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
    printf("Digite o nome do arquivo PGM de saida: ");
    scanf("%s", newImage_name);
  
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
  
    // Matriz para guardar a imagem em tons de cinza
    unsigned char **grayImage;
    grayImage = aloca(m, n);
  
    // Lê os canais de cores de cada pixel ij da imagem
    // e salva na matriz grayImage o seu correspondente
    // em cinza
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            fscanf(image, "%c%c%c", &red, &green, &blue);
            grayImage[i][j] = combLinear(red, green, blue);
            //grayImage[i][j] = media(red, green, blue);
        }
    }
  
    newImage = fopen(newImage_name , "w"); // Abre o arquivo no modo escrita
    fprintf(newImage,"P5\n"); // Escreve o cabecalho arquivo PGM
    fprintf(newImage, "%d %d\n %d\n", m, n, max); // Escreve o cabecalho
  
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            // Escreve os valores de cada pixel no arquivo
            fprintf(newImage, "%c", (char) grayImage[i][j]);
        }
    }
  
    fclose(image); // Fecha o arquivo da imagem original
    fclose(newImage); // Fecha o arquivo da nova imagem
  
    // Libera a memória alocada para a matriz grayImage
    for(i = 0; i < n; i++)
    {
        free(grayImage[i]);
    }
    free(grayImage);
    return 0;
}