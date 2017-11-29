#include <stdio.h>
#include "funcoes.h"
#include "structs.h"
int main()
{   
    char resposta[7];
    int a=0,b=1;
    imagem Imagem;
    lerimagem(&Imagem);


    pont_imagem Imagemcinza;
    Imagemcinza = transformarcinza(&Imagem);

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

    printf("Deseja gerar imagem com pupila marcada?(sim/nao): ");
    scanf("%s",resposta);
    if(strcmp(resposta,"sim")==0){
    	marcarpupila(&Imagem,c);
    	novaimagem(&Imagem,a);
    }

    novaimagem(Imagemsegmentada,b);
  return 0;
}
