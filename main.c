#include <stdio.h>
#include "funcoes.h"
#include "structs.h"
int main(int argc, char *argv[])
{   
    char resposta[7];
    int a=0,b=1,d=2;// valores para se passado como parametro para escrita de nova imagem
    char nomeimagem[50];
    strcpy(nomeimagem,argv[2]);
    FILE *file=fopen(nomeimagem,"r");
    if (file == NULL){
        fprintf(stderr, "Erro ao tentar abrir a imagem. NULL.\n");
    }
    imagem Imagem;

    pont_imagem Imagemoriginal;
    Imagemoriginal=lerimagem(file);


    pont_imagem Imagemcinza;
    Imagemcinza = transformarcinza(Imagemoriginal);

    pont_imagem Imagemgauss;
    Imagemgauss=filtrogaussiano(Imagemcinza);

    pont_imagem Imagemsobel;
    Imagemsobel=filtrosobel(Imagemgauss);

    pont_imagem Imagembin;
    Imagembin=binarizacao(Imagemsobel);

    centro *c = hough(Imagembin);

    pont_imagem Imagemsegmentada;
    Imagemsegmentada=segmentacao(Imagemcinza, c);

    double porcentagemfinal = porcentagem(Imagemsegmentada);
    ndiagnostico(porcentagemfinal);

    printf("Deseja gerar imagem sem flash?(sim/nao): ");
    scanf("%s",resposta);
    if(strcmp(resposta,"sim")==0){
        pont_imagem Imagemsflash;
        Imagemsflash=flash(Imagemsegmentada);
        novaimagem(Imagemsflash,d);
    }

    printf("Deseja gerar imagem com pupila marcada?(sim/nao): ");
    scanf("%s",resposta);
    if(strcmp(resposta,"sim")==0){
        marcarpupila(Imagemoriginal,c);
        novaimagem(Imagemoriginal,a);
    }
    novaimagem(Imagemsegmentada,b);
  return 0;
}
