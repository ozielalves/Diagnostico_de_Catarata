#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int r, g, b;
}pixel;

typedef struct{
    pixel **pixelimagem;//ponteiro que aponta para os pixels da imagem
    char codigo[2];
    int max, largura, altura;
}imagem, *pont_imagem;//ponteiro que aponta para imagem

void criarmatriz(pont_imagem Imagem){ /*Prepara os pixels da nova imagem recebendo a altura e largura da imagem original*/
    int i;
    Imagem->pixelimagem = (pixel**)malloc(Imagem->altura * sizeof(pixel*));
    for(i=0; i<Imagem->altura; i++){
        Imagem->pixelimagem[i] = (pixel*)malloc(Imagem->largura * sizeof(pixel));
    }
}
void lerimagem(pont_imagem Imagem){/*Lê a imagem do usuário*/
    FILE *imagem;// ponteiro que aponta pro arquivo
    int i, j;
    char nomeimagem[250];
    char temp[100];
    printf("Digite o nome da imagem: ");
    scanf("%s", nomeimagem);
    imagem=fopen(nomeimagem, "r");// abre em modo leitura
        if(imagem == NULL){
            printf("Houve um erro ao abrir a imagem\n");
            exit(1);
        }
    while(1){
    fscanf(imagem,"%s",temp);
        if(temp[0]=='#'){
            continue;
        }else{
            break;
        }
      if(strcmp(temp,"P3")!=0){ // Verifica se é PPM(incompleto)
            printf("Imagem nao eh PPM\n");
            exit(1);
        }
    }
    strcpy(Imagem->codigo,temp);
    while(1){
    fgets(temp,100,imagem);
        if(temp[0]=='#'){
            fscanf(imagem,"%s",temp);
            continue;
        }else{
            break;
        }
    }
    Imagem->largura=atoi(temp);
    while(1){
    fscanf(imagem,"%s",temp);
        if(temp[0]=='#'){
            continue;
        }else{
            break;
        }
    }
    Imagem->altura=atoi(temp);
    criarmatriz(Imagem);
    while(1){
    fscanf(imagem,"%s",temp);
        if(temp[0]=='#'){
            continue;
        }else{
            break;
        }
    }
    Imagem->max=atoi(temp);
    for(i=0; i<Imagem->altura; i++){
        for(j=0; j<Imagem->largura; j++){
            fscanf(imagem,"%i",&Imagem->pixelimagem[i][j].r);/*aqui se lê todos os pixels da imagem e armazena em r=red, g=gree,b=blue*/
            fscanf(imagem,"%i",&Imagem->pixelimagem[i][j].g);
            fscanf(imagem,"%i",&Imagem->pixelimagem[i][j].b);
        }
    }
    fclose(imagem);
} 
void novaimagem(pont_imagem Imagem){/*Função para criar a imagem em tons de cinza*/
    FILE *imagem;
    int i, j;
    char novonome[250];
    printf("Digite o novo nome para imagem em cinza: ");
    scanf("%s", novonome);
    imagem=fopen(novonome,"w");// abre em modo escrita
    fprintf(imagem,"%s\n",Imagem->codigo);/*escreve o cabeçalho da imagem*/
    fprintf(imagem,"%i ",Imagem->largura);
    fprintf(imagem,"%i\n",Imagem->altura);
    fprintf(imagem,"%i\n",Imagem->max);
    for(i=0; i<Imagem->altura; i++){
        for(j=0; j<Imagem->largura; j++){
            fprintf(imagem,"%i ",Imagem->pixelimagem[i][j].r);/*Escreve os novos pixels, ja alterados na conversão*/
            fprintf(imagem,"%i ",Imagem->pixelimagem[i][j].g);
            fprintf(imagem,"%i\n",Imagem->pixelimagem[i][j].b);
        }
    }
    fclose(imagem);
    free(Imagem->pixelimagem);//libera o espaço
} 
void transformarcinza(pont_imagem Imagem){
    int i, j;
    int combinacao;
    for(i=0; i<Imagem->altura; i++){
        for(j=0; j<Imagem->largura; j++){
            combinacao=(Imagem->pixelimagem[i][j].r*0.3 + Imagem->pixelimagem[i][j].g*0.59 + Imagem->pixelimagem[i][j].b*0.11);/*Formula para conversao de cinza dada no pdf*/
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
