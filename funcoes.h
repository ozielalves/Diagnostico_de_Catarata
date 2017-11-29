#ifndef FUNCOES_H
#define FUNCOES_H
#include "structs.h"

/*A função abaixo aloca o espaço, quando chamada.*/
void criarmatriz(pont_imagem Imagem);
/*Essa função libera e evita o vazamento de memória, inclusive nos pixels(free).*/
void limparimagem(pont_imagem Imagem);
/*A função abaixo tem como retorno as coordenadas do centro c, e o própio, da imagem e o raio, é utilizado o método da transformada de Hough.*/
centro *hough(pont_imagem Imagem);
/*Nessa função, segmenta-se a imagem exatamente na pupila. A segmentação é fundamental para a analise dos pixels defeituosos no futuro.*/
pont_imagem segmentacao(pont_imagem Imagem, centro *c);
/*Essa função aplica o filtro de sobel, para realçar arestas a serem trabalhadas no futuro na binarização e transformada de Hough .*/
pont_imagem filtrosobel(pont_imagem Imagem);
/*A função abaixo retorna a imagem binarizada (ou em preto, ou em branco), baseada num threshold(diferente à cada imagem).*/
pont_imagem binarizacao(pont_imagem Imagem);
/*Função que aplica o filtro gaussiano, para eliminar ruidos, deixando a imagem mais "suave" para ser trabalhada no sobel.*/
pont_imagem filtrogaussiano(pont_imagem Imagem);
/*Usada  para fazer a leitura da imagem.*/
pont_imagem lerimagem(FILE *imagem);
/*Usada para criar uma nova imagem.*/
void novaimagem(pont_imagem Imagem, unsigned short int a);
/*Função para transformar a imagem em tons de cinza, baseada no método de combinação linear.*/
pont_imagem transformarcinza(pont_imagem Imagem);
/*Aqui calcula-se a porcentagem de pixels afetados pela catarata.*/
double porcentagem(pont_imagem Imagem);
/*Aqui emite-se o diagnóstico, tendo como resultado se a imagem possui ou não catarata e a porcentagens de pixels comprometidos.*/
void ndiagnostico(double porcentagemfinal);
/*Função para marcar o contorno da pupila em RGB.*/
void marcarpupila(pont_imagem Imagem, centro *c);
pont_imagem flash(pont_imagem Imagem);
#endif
