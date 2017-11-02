#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int r, g, b;
}pixel;


typedef struct{
    pixel **pix_imagem;
    char codigo[3];
    int qualidade, colunas, linhas;
}imagem, *pont_imagem;


void criarMatrizPixeis(pont_imagem Imagem){
    int i;
    Imagem->pix_imagem = (pixel**)malloc(Imagem->linhas * sizeof(pixel*));
    for(i = 0; i < Imagem->linhas; i++){
        Imagem->pix_imagem[i] = (pixel*)malloc(Imagem->colunas * sizeof(pixel));
    }
}
void lerimagem(pont_imagem Imagem){
    FILE *imagem;
    int i, j;
    char nomeimagem[250];

    printf("Digite o nome da imagem: ");
    scanf("%s", nomeimagem);
    imagem = fopen(nomeimagem, "r");
    if(imagem == NULL){
        printf("Houve um erro ao abrir a imagem\n");
        exit(1);
    }
    
    fscanf(imagem,"%s",Imagem->codigo);
    if(strcmp(Imagem->codigo,"P3")!=0){
        printf("Imagem nao eh PPM\n");
        fclose(imagem);
    }
    fscanf(imagem,"%i",&Imagem->colunas);
    fscanf(imagem,"%i",&Imagem->linhas);
    criarMatrizPixeis(Imagem);
    fscanf(imagem,"%i",&Imagem->qualidade);

    for(i = 0; i < Imagem->linhas; i++){
        for(j = 0; j < Imagem->colunas; j++){
            fscanf(imagem,"%i",&Imagem->pix_imagem[i][j].r);
            fscanf(imagem,"%i",&Imagem->pix_imagem[i][j].g);
            fscanf(imagem,"%i",&Imagem->pix_imagem[i][j].b);
        }
    }

    fclose(imagem);
}
  
void novaimagem(pont_imagem Imagem){
    FILE *imagem;
    int i, j;
    char novonome[250];
    printf("Digite o novo nome para imagem em cinza: ");
    scanf("%s", novonome);

    imagem = fopen(novonome, "w");

    fprintf(imagem,"%s\n",Imagem->codigo);
    fprintf(imagem,"%i ",Imagem->colunas);
    fprintf(imagem,"%i\n",Imagem->linhas);
    fprintf(imagem,"%i\n",Imagem->qualidade);

    for(i = 0; i < Imagem->linhas; i++){
        for(j = 0; j < Imagem->colunas; j++){
            fprintf(imagem,"%i ",Imagem->pix_imagem[i][j].r);
            fprintf(imagem,"%i ",Imagem->pix_imagem[i][j].g);
            fprintf(imagem,"%i\n",Imagem->pix_imagem[i][j].b);
        }
    }
    fclose(imagem);
    free(Imagem->pix_imagem);
} 
void transformarcinza(pont_imagem Imagem){
    int i, j;
    int combinacao;
    for(i = 0; i < Imagem->linhas; i++){
        for(j = 0; j < Imagem->colunas; j++){
            combinacao = (Imagem->pix_imagem[i][j].r*0.3 + Imagem->pix_imagem[i][j].g*0.59 + Imagem->pix_imagem[i][j].b*0.11);
            Imagem->pix_imagem[i][j].r = combinacao;
            Imagem->pix_imagem[i][j].g = combinacao;
            Imagem->pix_imagem[i][j].b = combinacao;
        }
    }
}
int main()
{   
    imagem Imagem;
    lerimagem(&Imagem);
    transformarcinza(&Imagem);
    novaimagem(&Imagem);
  return 0;
}
