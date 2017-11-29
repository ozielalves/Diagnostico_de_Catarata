#include <stdio.h>
#include "funcoes.h"
#include "structs.h"
int main(int argc, char *argv[])
{   
    char resposta[7];
    int a=0,b=1,d=2;// valores para se passado como parametro para escrita de nova imagem
    char nomeimagem[50];
    strcpy(nomeimagem,argv[2]);// Cópia do nome do argv[2] para uma variável.
    FILE *file=fopen(nomeimagem,"r"); /*Aqui se abre a imagem para leitura, com o nome dado no argv[2].*/
    if (file == NULL){
        fprintf(stderr, "Erro ao tentar abrir a imagem. NULL.\n");
    }
    imagem Imagem;

    pont_imagem Imagemoriginal;
    Imagemoriginal=lerimagem(file); //Imagem sem nenhum filtro aplicado


    pont_imagem Imagemcinza;
    Imagemcinza = transformarcinza(Imagemoriginal);// Aplicação da escala de cinza.

    pont_imagem Imagemgauss;
    Imagemgauss=filtrogaussiano(Imagemcinza);// Aplicação do filtro de gauss.

    pont_imagem Imagemsobel;
    Imagemsobel=filtrosobel(Imagemgauss);//Aplicação do filtro de sobel.

    pont_imagem Imagembin;
    Imagembin=binarizacao(Imagemsobel);//Aplicação da binarização.

    centro *c = hough(Imagembin);// definição do centro da pupila e seu respectivo raio.

    pont_imagem Imagemsegmentada;
    Imagemsegmentada=segmentacao(Imagemcinza, c);// Segmentação da imagem em tons de cinza.

    double porcentagemfinal = porcentagem(Imagemsegmentada); // Porcentagem de pixels comprometidos.
    ndiagnostico(porcentagemfinal);//Criação do diagnóstico

    printf("Deseja gerar imagem sem flash?(sim/nao): ");
    scanf("%s",resposta);
    if(strcmp(resposta,"sim")==0){ // Criação de imagem com o flash borrado.
        pont_imagem Imagemsflash;
        Imagemsflash=flash(Imagemsegmentada);
        novaimagem(Imagemsflash,d);
    }

    printf("Deseja gerar imagem com pupila marcada?(sim/nao): ");
    scanf("%s",resposta);
    if(strcmp(resposta,"sim")==0){ //Criação da imagem com a pupila em RGB marcada.
        marcarpupila(Imagemoriginal,c);
        novaimagem(Imagemoriginal,a);
    }
    novaimagem(Imagemsegmentada,b);
  return 0;
}
