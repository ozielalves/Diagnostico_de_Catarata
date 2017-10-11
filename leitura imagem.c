#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int r;
    int g;
    int b;
} pixel;

pixel imagem[300][300];


void filtro_negativo(pixel im[300][300], int alt, int larg, int max);

int main() {
    int alt, larg;
    char tipo[3];
    char coment[100];
    int max;
    int i, j;

    scanf("%s%d%d%d", tipo, &larg, &alt, &max);

    for (i = 0; i < alt; i++) {
        for (j = 0; j < larg; j++) {
            scanf("%d%d%d", &imagem[i][j].r, &imagem[i][j].g, &imagem[i][j].b);
        }
    }

    filtro_negativo(imagem, alt, larg, max);

    printf("%s\n", tipo);
    printf("%d %d\n", larg, alt);
    printf("%d\n", max);

    for (i = 0; i < alt; i++) {
        for (j = 0; j < larg; j++) {
            printf("%d %d %d\n", 255 - imagem[i][j].r, 255 - imagem[i][j].g, 255 - imagem[i][j].b);
        }
    }
    return (EXIT_SUCCESS);
}

void filtro_negativo(pixel im[300][300], int alt, int larg, int max) {
    int i, j;

    for (i = 0; i < alt; i++) {
        for (j = 0; j < larg; j++) {
            im[i][j].r = max - im[i][j].r;
            im[i][j].g = max - im[i][j].g;
            im[i][j].b = max - im[i][j].b;
        }
    }
}
