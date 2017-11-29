#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define PI 3.1415926535
/*Struct que armazena os pixels da imagem, em r, g, b.*/
typedef struct{
    int r, g, b;
}pixel;
/*Struct que armazena a imagem como um todo.*/
typedef struct{
    pixel **pixelimagem;//ponteiro que aponta para os pixels da imagem.
    char codigo[2];//codigo da imagem ppm(P3).
    int max, largura, altura;//altura, largura e tonalidade máxima da imagem.
}imagem, *pont_imagem;//ponteiro que aponta para imagem
/*Struct que contém as coordenadas do centro da imagem e seu raio*/
typedef struct{
  int x,y,r;
} centro;
#endif
