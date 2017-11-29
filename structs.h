#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define PI 3.1415926535
typedef struct{
    int r, g, b;
}pixel;

typedef struct{
    pixel **pixelimagem;//ponteiro que aponta para os pixels da imagem
    char codigo[2];
    int max, largura, altura;
}imagem, *pont_imagem;//ponteiro que aponta para imagem

typedef struct{
  int x,y,r;
} centro;
#endif
