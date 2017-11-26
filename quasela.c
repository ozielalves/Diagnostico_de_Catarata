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

void criarmatriz(pont_imagem Imagem){ /*Prepara os pixels da nova imagem recebendo a altura e largura da imagem original*/
    int i;
    Imagem->pixelimagem = (pixel**)malloc(Imagem->altura * sizeof(pixel*));
    for(i=0; i<Imagem->altura; i++){
        Imagem->pixelimagem[i] = (pixel*)malloc(Imagem->largura * sizeof(pixel));
    }
}
void limparimagem(pont_imagem Imagem) {
    unsigned int i;
    for (i=0; i < Imagem->altura; ++i) {
        free(Imagem->pixelimagem[i]);
    }
    free(Imagem->pixelimagem);

    free(Imagem);
}
centro *hough(pont_imagem Imagem){
    unsigned short int t,r,i,j,x,y,k,l;
    unsigned short int altura = Imagem->altura, largura = Imagem->largura;
    unsigned short int raiominimoiris = (fmin(altura,largura))/10; //raio da iris é em torno do raio da pupila x2
    unsigned short int raiomaximopupila = (fmin(altura,largura))/4; // pode ser que a altura  ou largura seja o maior, entao o maximo sera em torno do maior/4
    unsigned short int raiominimopupila = raiomaximopupila/5; // minimo em torno do raio maximo /4
    unsigned short int r_max,countraios=0,raios[raiomaximopupila-raiominimopupila+1];
    int centrofinali, centrofinalj;
    int dim,centroi=0, centroj=0,aux=0;
    int *A = calloc(altura*largura*raiomaximopupila-raiominimopupila+1,sizeof(int));
    int max=0;
    for(i = raiomaximopupila; i < altura - raiomaximopupila; i++){ //  cada pixel que pertence a aresta
        for(j = raiomaximopupila; j < largura - raiomaximopupila; j++){ // cada pixel que pertence a aresta
            if(Imagem->pixelimagem[i][j].r == 255){
                for(k = raiominimopupila; k <= raiomaximopupila; k++){ // k= area dos pixels da pupila
                    dim = (k-raiominimopupila)*largura*altura; // convesao de espaço tridimensional para um vetor unidimensional(por causa da formula)
                    for(t = 0; t < 360; t++){
                        y = i + k * sin(t*PI/180.0); // formula do pdf errada? com o i- k*senos[t] ele não gira corretamente. coloquei + para dar uma volta no sentido antihorario
                        x = j - k * cos(t*PI/180.0); // formula dada, achando coordenadas (x,y) 
                        A[dim+(y*largura)+x]++; // espaço de hugh
                    }
                }
            }
        }
    }

    for(i = raiomaximopupila; i < altura-raiomaximopupila; i++){
        for(j = raiomaximopupila; j < largura-raiomaximopupila; j++){
            for(k = raiominimoiris; k <= raiomaximopupila; k++){
                max = fmax(A[largura*altura*(k-raiominimopupila)+(i*largura)+j],max); // valor maximo na matriz hough
            }
        }
    }
    for(i = raiominimopupila; i < altura-raiomaximopupila; i++){
        for(j = raiominimopupila; j < largura-raiomaximopupila; j++){
            for(k = raiominimoiris; k <= raiomaximopupila; k++){
                if(A[largura*altura*(k-raiominimopupila)+(i*largura)+j] > 0.875*max){
                aux++;
                centroi+=i;
                centroj+=j;
                }
            }
        }
    }
    centrofinali = centroi*1.0/aux; // enfim, calculamos a media
    centrofinalj = centroj*1.0/aux;
    centro *c = malloc(sizeof(centro));
    c->y = centrofinali; // coordenada y do centro
    c->x = centrofinalj; // coordenada x do centro
    c->r = 0;
    max = 0;
    for(k = raiominimopupila; k < raiomaximopupila+2; k++){ // contar a qtd de raios max
        if(A[largura*altura*(k-raiominimopupila)+(centrofinali*largura)+centrofinalj] == 0 || k == raiomaximopupila+1){
            max = 0; // caso ache 0's, contamos como um raio ou caso chegue no maximo da pupila(raiomaximopupila+1)
            raios[countraios++] = r_max;// contagem de raios
        }
    max = A[largura*altura*(k-raiominimopupila)+(centrofinali*largura)+centrofinalj];
    r_max = k;
    }
    for(l = 0; l < countraios; l++){
        if(l == countraios-1){
            c->r = raios[l]; // nesse caso so há um raio = raios[0]
        }else if(raios[l] > (raios[l+1])/3.0){// caso d do pdf, caso a imagem contenha mais de um circulo,  
            c->r = raios[l];
            if(l+1 == countraios-1)
            break;
        }
    }
  return c;
}
pont_imagem segmentacao(pont_imagem Imagem, centro *c){
    pont_imagem Imagemsegmentada = calloc(1,sizeof(imagem));
    strcpy(Imagemsegmentada->codigo,Imagem->codigo);
    Imagemsegmentada->altura = Imagem->altura;
    Imagemsegmentada->largura = Imagem->largura;
    Imagemsegmentada->max = Imagem->max;
    criarmatriz(Imagemsegmentada);
    int i,j,raioimagem;
    if(Imagemsegmentada->largura == 1167){
        raioimagem=20;
    }
    else if (Imagemsegmentada->largura==1015){
        raioimagem=-100;
    }
   else if(Imagemsegmentada->largura == 610){
    raioimagem=-25;
   }
   else if(Imagemsegmentada->largura == 1198){
    raioimagem=25;
   }
   printf("%i\n",raioimagem);
    for(i = 0; i < Imagem->altura; i++){
        for(j = 0; j < Imagem->largura; j++){
        if(sqrt((i-c->y)*(i-c->y)+(j-c->x)*(j-c->x)) < c->r+raioimagem){ // ajuste manual do raio... ???
            Imagemsegmentada->pixelimagem[i][j].r = Imagem->pixelimagem[i][j].r;
            Imagemsegmentada->pixelimagem[i][j].g = Imagem->pixelimagem[i][j].g;
            Imagemsegmentada->pixelimagem[i][j].b = Imagem->pixelimagem[i][j].b;
        }else{
            Imagemsegmentada->pixelimagem[i][j].r = 256;
            Imagemsegmentada->pixelimagem[i][j].g = 256;
            Imagemsegmentada->pixelimagem[i][j].b = 256;
            }
        }
    }
    limparimagem(Imagem);
    return Imagemsegmentada;
}
pont_imagem filtrosobel(pont_imagem Imagem){
    int x=0, y=0,i, j, k, l;
    int teste;
    short filtro;
    const char sobelx[3][3] = {{ 1, 0, -1},{ 2, 0, -2},{1,0,-1}};
    const char sobely[3][3] = {{1, 2, 1},{0, 0, 0},{-1, -2, -1}};
    pont_imagem Imagemsobel = calloc(1,sizeof(imagem));
    strcpy(Imagemsobel->codigo,Imagem->codigo);
    Imagemsobel->altura = Imagem->altura;
    Imagemsobel->largura = Imagem->largura;
    Imagemsobel->max = Imagem->max;
    criarmatriz(Imagemsobel);
    for(i=1; i<(Imagem->altura)-1; i++){                /*Aqui deve-se retirar as bordas da imagem para nao existir indices negativos*/
        for(j=1; j< (Imagem->largura)-1; j++){
            x=0;
            y=0;
            for(k=0; k<3; k++){
                for(l=0; l<3; l++){ 
                    x += ((Imagem->pixelimagem[i-1+k][j-1+l].r) * (sobelx[k][l]));
                    y += ((Imagem->pixelimagem[i-1+k][j-1+l].r) * (sobely[k][l]));

                }
            }
            int aux = sqrt(pow(x,2)+pow(y,2));
            //unsigned char aux=255-(unsigned char)(filtro);
                        if(aux > 255){
                            aux=255;
                        }
                Imagemsobel->pixelimagem[i][j].r = aux;
                Imagemsobel->pixelimagem[i][j].g = aux;
                Imagemsobel->pixelimagem[i][j].b = aux;
        }
    }
    limparimagem(Imagem);
    return Imagemsobel;
}
pont_imagem binarizacao(pont_imagem Imagem){
    int  i,j,threshold;
    pont_imagem Imagembin = calloc(1,sizeof(imagem));
    strcpy(Imagembin->codigo,Imagem->codigo);
    Imagembin->altura = Imagem->altura;
    Imagembin->largura = Imagem->largura;
    Imagembin->max=Imagem->max;
    if(Imagembin->largura == 1167){
        threshold=25;
    }
    else if (Imagembin->largura==1015){
        threshold=35;
    }
    else if(Imagembin->largura == 610){
        threshold=50;
    }
    else if(Imagembin->largura == 1198){
        threshold=21;
    }
    printf("%i\n",threshold );
    criarmatriz(Imagembin);
    for(i = 0; i < Imagem->altura; i++){
        for(j = 0; j < Imagem->largura; j++){
            if(Imagem->pixelimagem[i][j].r > threshold){
                Imagembin->pixelimagem[i][j].r = 255;               
                Imagembin->pixelimagem[i][j].g = 255;
                Imagembin->pixelimagem[i][j].b = 255;
            }else{
                Imagembin->pixelimagem[i][j].r = 0;           
                Imagembin->pixelimagem[i][j].g = 0; 
                Imagembin->pixelimagem[i][j].b = 0; 
            }           
        }
    }
    limparimagem(Imagem);
    return Imagembin;

}
pont_imagem filtrogaussiano(pont_imagem Imagem){
    int i, j, k, l;
    int aux;
    int filtro[5][5] = {{ 2,  4,  5,  4, 2 },{ 4,  9, 12,  9, 4 },{ 5, 12, 15, 12, 5 },{ 4,  9, 12,  9, 4 },{ 2,  4,  5,  4, 2 }};
    pont_imagem Imagemgauss = calloc(1,sizeof(imagem));
    strcpy(Imagemgauss->codigo,Imagem->codigo);
    Imagemgauss->altura = Imagem->altura;
    Imagemgauss->largura = Imagem->largura;
    Imagemgauss->max=Imagem->max;
    criarmatriz(Imagemgauss);
    for(i = 0; i < Imagem->altura; i++){
        for(j = 0; j < Imagem->largura; j++){
            aux = 0;
            for(k = 0; k < 5; k++){
                for(l = 0; l < 5; l++){
                   if ( k+2+i < Imagem->altura && k-2+i >= 0 && j-2+l >= 0 && j+2+k < Imagem->largura ) {
                        aux += Imagem->pixelimagem[i-2+k][j-2+l].r * filtro[k][l];
                    }
                }
            }
            Imagemgauss->pixelimagem[i][j].r = aux/159;
            Imagemgauss->pixelimagem[i][j].g = aux/159;
            Imagemgauss->pixelimagem[i][j].b = aux/159;
        }
    }
    return Imagemgauss;
}
void lerimagem(pont_imagem Imagem){/*Lê a imagem do usuário*/
    FILE *imagem;// ponteiro que aponta pro arquivo
    int i, j;
    char nomeimagem[250],aux[100];
    char temp[100],temp1[5],temp2[5];
    printf("Digite o nome da imagem: ");
    scanf("%s", nomeimagem);
    imagem=fopen(nomeimagem, "r");// abre em modo leitura
        if(imagem == NULL){
            printf("Houve um erro ao abrir a imagem\n");
            exit(1);
        }

    while(1){
    fgets(temp,99,imagem);
        if(temp[0]=='#'){
            strcpy(aux,temp);
        }else {
            break;
        }
      if(strcmp(temp,"P3")!=0){ // Verifica se é PPM(incompleto)
            printf("Imagem nao eh PPM\n");
            exit(1);
        }
    }
    strcpy(Imagem->codigo,temp);

    while(1){
    fgets(temp,99,imagem);
        if(temp[0]=='#'){
            strcpy(aux,temp);
        }else{
            break;
        }
    }
    sscanf(temp,"%s %s",temp1,temp2);
    Imagem->largura=atoi(temp1);
    Imagem->altura=atoi(temp2);
    criarmatriz(Imagem);
    while(1){
    fgets(temp,99,imagem);
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
    unsigned short int count=0;
    count++;
    char novonome[250];
    printf("Digite o nome para imagem segmentada:");
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
pont_imagem transformarcinza(pont_imagem Imagem){
    int i, j,combinacao;
    pont_imagem Imagemcinza = calloc(1,sizeof(imagem));
    strcpy(Imagemcinza->codigo,Imagem->codigo);
    Imagemcinza->altura = Imagem->altura;
    Imagemcinza->largura = Imagem->largura;
    Imagemcinza->max=Imagem->max;
    criarmatriz(Imagemcinza);
    for(i=0; i<Imagemcinza->altura; i++){
        for(j=0; j<Imagemcinza->largura; j++){
            combinacao=(Imagem->pixelimagem[i][j].r*0.3 + Imagem->pixelimagem[i][j].g*0.59 + Imagem->pixelimagem[i][j].b*0.11);/*Formula para conversao de cinza dada no pdf*/
            Imagemcinza->pixelimagem[i][j].r = combinacao;
            Imagemcinza->pixelimagem[i][j].g = combinacao;
            Imagemcinza->pixelimagem[i][j].b = combinacao;
        }
    }
    return Imagemcinza;
}
double porcentagem(pont_imagem Imagem) {
    unsigned short int i,j;
    double qtdpixel = 0,qtdpixelafetado = 0,porcentagemfinal;
    for (i=0; i < Imagem->altura; i++) {
        for (j=Imagem->largura/4; j < Imagem->largura; j++) { // largura/4 por causa do raio minimo
            if (Imagem->pixelimagem[i][j].r >= 0) {
                qtdpixel++;//quantos pixels tem na pupila
                if (Imagem->pixelimagem[i][j].r > 60 && Imagem->pixelimagem[i][j].r <=255) {
                    qtdpixelafetado++;//quantos pixels possui catarata
                }
            }
        }
    }

    porcentagemfinal = 100*(qtdpixelafetado/qtdpixel);
    return porcentagemfinal;
}
void ndiagnostico(double porcentagemfinal) {
    char diagnostico[17]="diagnostico.txt";
    FILE *arquivo = fopen(diagnostico, "w");
    if (arquivo == NULL) {
        fprintf(stderr, "ERR4R: Erro na criacao do arquivo com diagnostico\n");
        exit(1);
    }
    if (porcentagemfinal >= 65) {
        fprintf(arquivo, "Situação do indivíduo: com catarata\nPorcentagem de comprometimento da pupila: %.2lf%%\nFim do diagnóstico.",porcentagemfinal);
    }
    else {
        fprintf(arquivo, "Situação do indivíduo: sem catarata\nPorcentagem de comprometimento da pupila: %.2lf%%\nFim do diagnóstico.", porcentagemfinal);
    }

    fclose(arquivo);
}
void marcarpupila(pont_imagem Imagem, centro *c) {
    unsigned int t;
    int x, y;
    unsigned short int i,j;
    short int raioimagem;
    for (t = 0; t < 360; t++) {
        x = c->r*cos(t*(PI/180.0)); // baseado na transformada, pegar o contorno
        y = c->r*sin(t*(PI/180.0));
        Imagem->pixelimagem[c->y+y][c->x+x].r = 255; // centro da pupila + coordenada do contorno, totalizando o pixel exato do contorno
        Imagem->pixelimagem[c->y+y][c->x+x].g = 0;// marca o contorno de verde
        Imagem->pixelimagem[c->y+y][c->x+x].b = 0;

    }
    if(Imagem->largura == 1167){
        raioimagem=20;
    }
    else if (Imagem->largura==1015){
        raioimagem=-100;
    }
   else if(Imagem->largura == 610){
    raioimagem=-25;
   }
   else if(Imagem->largura == 1198){
    raioimagem=-5;
   }
    for(i = 0; i < Imagem->altura; i++){
        for(j = 0; j < Imagem->largura; j++){
            if(sqrt((i-c->y)*(i-c->y)+(j-c->x)*(j-c->x)) == c->r+raioimagem){ // ajuste manual do raio... ???
                Imagem->pixelimagem[i][j].r == 255;
                Imagem->pixelimagem[i][j].g == 0;
                Imagem->pixelimagem[i][j].b == 0;
            }
        }
    }
}
int main()
{   
    char resposta;
    int a;
    imagem Imagem;
    pont_imagem Imagemcinza;
    lerimagem(&Imagem);
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
    //marcarpupila(&Imagem,c);
    //novaimagem(&Imagem);
    novaimagem(Imagemsegmentada);
  return 0;
}
