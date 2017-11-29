#ifndef FUNCOES_H
#define FUNCOES_H
#include "structs.h"

void criarmatriz(pont_imagem Imagem);
void limparimagem(pont_imagem Imagem);
centro *hough(pont_imagem Imagem);
pont_imagem segmentacao(pont_imagem Imagem, centro *c);
pont_imagem filtrosobel(pont_imagem Imagem);
pont_imagem binarizacao(pont_imagem Imagem);
pont_imagem filtrogaussiano(pont_imagem Imagem);
void lerimagem(pont_imagem Imagem);
void novaimagem(pont_imagem Imagem, unsigned short int a);
pont_imagem transformarcinza(pont_imagem Imagem);
double porcentagem(pont_imagem Imagem);
void ndiagnostico(double porcentagemfinal);
void marcarpupila(pont_imagem Imagem, centro *c);
#endif
