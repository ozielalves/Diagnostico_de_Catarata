#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int r, g, b;
}pixel;

typedef struct{
    pixel **pixelimagem;
    char codigo[2];
    int max, colunas, linhas;
}imagem, *pont_imagem;

void criarmatriz(pont_imagem Imagem){
    int i;
    Imagem->pixelimagem = (pixel**)malloc(Imagem->linhas * sizeof(pixel*));
    for(i=0; i<Imagem->linhas; i++){
        Imagem->pixelimagem[i] = (pixel*)malloc(Imagem->colunas * sizeof(pixel));
    }
}
void lerimagem(pont_imagem Imagem){
    FILE *imagem;
    int i, j;
    char nomeimagem[250];

    printf("Digite o nome da imagem: ");
    scanf("%s", nomeimagem);
    imagem=fopen(nomeimagem, "r");
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
    criarmatriz(Imagem);
    fscanf(imagem,"%i",&Imagem->max);

    for(i=0; i<Imagem->linhas; i++){
        for(j=0; j<Imagem->colunas; j++){
            fscanf(imagem,"%i",&Imagem->pixelimagem[i][j].r);
            fscanf(imagem,"%i",&Imagem->pixelimagem[i][j].g);
            fscanf(imagem,"%i",&Imagem->pixelimagem[i][j].b);
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
    imagem=fopen(novonome,"w");
    fprintf(imagem,"%s\n",Imagem->codigo);
    fprintf(imagem,"%i ",Imagem->colunas);
    fprintf(imagem,"%i\n",Imagem->linhas);
    fprintf(imagem,"%i\n",Imagem->max);
    for(i=0; i<Imagem->linhas; i++){
        for(j=0; j<Imagem->colunas; j++){
            fprintf(imagem,"%i ",Imagem->pixelimagem[i][j].r);
            fprintf(imagem,"%i ",Imagem->pixelimagem[i][j].g);
            fprintf(imagem,"%i\n",Imagem->pixelimagem[i][j].b);
        }
    }
    fclose(imagem);
    free(Imagem->pixelimagem);
} 
void transformarcinza(pont_imagem Imagem){
    int i, j;
    int combinacao;
    for(i=0; i<Imagem->linhas; i++){
        for(j=0; j<Imagem->colunas; j++){
            combinacao=(Imagem->pixelimagem[i][j].r*0.3 + Imagem->pixelimagem[i][j].g*0.59 + Imagem->pixelimagem[i][j].b*0.11);
            Imagem->pixelimagem[i][j].r = combinacao;
            Imagem->pixelimagem[i][j].g = combinacao;
            Imagem->pixelimagem[i][j].b = combinacao;
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
