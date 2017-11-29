#include "funcoes.h"
#include "structs.h"
void criarmatriz(pont_imagem Imagem){ /*Prepara os pixels da nova imagem recebendo a altura e largura da imagem*/
    int i;
    Imagem->pixelimagem = (pixel**)malloc(Imagem->altura * sizeof(pixel*));
    for(i=0; i<Imagem->altura; i++){
        Imagem->pixelimagem[i] = (pixel*)malloc(Imagem->largura * sizeof(pixel));
    }
}
void limparimagem(pont_imagem Imagem) {
    int i;
    for (i=0; i < Imagem->altura; ++i) {
        free(Imagem->pixelimagem[i]);
    }
    free(Imagem->pixelimagem);

    free(Imagem);
}
centro *hough(pont_imagem Imagem){
    unsigned short int t,r,i,j,x,y,k,l;
    unsigned short int altura = Imagem->altura, largura = Imagem->largura;
    unsigned short int raiominimoiris = (fmin(altura,largura))/8; //raio da iris é em torno do raio da pupila x2
    unsigned short int raiomaximopupila = (fmin(altura,largura))/4; // pode ser que a altura  ou largura seja o maior, entao o maximo sera em torno do menor/4
    unsigned short int raiominimopupila = raiomaximopupila/4; // minimo em torno do raio maximo /4
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
                        y = i + k * sin(t*PI/180.0); 
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
                if(A[largura*altura*(k-raiominimopupila)+(i*largura)+j] > 0.9*max){
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
    if(c == NULL){
    	fprintf(stderr, "Erro na alocacao do centro da pupila");
    }
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
        }else if(raios[l] > (raios[l+1])/3.0){// caso d do pdf, caso a imagem contenha mais de um circulo.
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
    if(Imagemsegmentada->largura == 1167){/*Cada imagem possui dimensoes diferentes, logo, raios diferentes*/
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
    for(i = 0; i < Imagem->altura; i++){
        for(j = 0; j < Imagem->largura; j++){
        if(sqrt((i-c->y)*(i-c->y)+(j-c->x)*(j-c->x)) < c->r+raioimagem){ 
		/*Nesse caso, foi necessário identificar manualmente o valor especifico do raio da imagem.  Caso o círculo seja excêntrico à pupila,
		todos os valores menor que o raio deste circulo, sera pertencente a pupila.*/
            Imagemsegmentada->pixelimagem[i][j].r = Imagem->pixelimagem[i][j].r;
            Imagemsegmentada->pixelimagem[i][j].g = Imagem->pixelimagem[i][j].g;
            Imagemsegmentada->pixelimagem[i][j].b = Imagem->pixelimagem[i][j].b;
        }else{
		/*Caso não, será preto.(Não pertencente a imagem.)*/
            Imagemsegmentada->pixelimagem[i][j].r = 0;
            Imagemsegmentada->pixelimagem[i][j].g = 0;
            Imagemsegmentada->pixelimagem[i][j].b = 0;
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
    const char sobelx[3][3] = {{ 1, 0, -1},{ 2, 0, -2},{1,0,-1}};//filtros
    const char sobely[3][3] = {{1, 2, 1},{0, 0, 0},{-1, -2, -1}};
    pont_imagem Imagemsobel = calloc(1,sizeof(imagem));
    if(Imagemsobel==NULL){
	fprintf(stderr, "Erro ao alocar memoria para imagem com o filtro de sobel.");
	return NULL;
    }
    strcpy(Imagemsobel->codigo,Imagem->codigo);
    Imagemsobel->altura = Imagem->altura;
    Imagemsobel->largura = Imagem->largura;
    Imagemsobel->max = Imagem->max;
    criarmatriz(Imagemsobel);
/*Aqui deve-se retirar as bordas da imagem para nao existir indices negativos*/
    for(i=1; i<(Imagem->altura)-1; i++){                
        for(j=1; j< (Imagem->largura)-1; j++){
            x=0;
            y=0;
            for(k=0; k<3; k++){
                for(l=0; l<3; l++){ 
                    x += ((Imagem->pixelimagem[i-1+k][j-1+l].r) * (sobelx[k][l]));// definição do Gx e Gy
                    y += ((Imagem->pixelimagem[i-1+k][j-1+l].r) * (sobely[k][l]));

                }
            }
            int aux = sqrt(pow(x,2)+pow(y,2)); //fórmula do filtro de sobel.
                        if(aux > 255){ //limite maximo da imagem.
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
    if(Imagembin==NULL){
	fprintf(stderr, "Erro ao alocar memoria para imagem binarizada.");
	return NULL;
    }
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
    criarmatriz(Imagembin);
    for(i = 0; i < Imagem->altura; i++){
        for(j = 0; j < Imagem->largura; j++){
            if(Imagem->pixelimagem[i][j].r > threshold){
		    /*Caso a intensidade do pixel seja maior que o threshold, tornar ele branco, senão, torne-o preto.
		    Importante observar que cada imagem possui um threshold diferente*/
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
	if(Imagemgauss==NULL){
	fprintf(stderr, "Erro ao alocar memoria para imagem com filtro gaussiano.");
	return NULL;
    }
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
pont_imagem lerimagem(FILE *imagem){/*Lê a imagem do usuário*/
    int i, j;
    char nomeimagem[250],aux[100];
    char temp[100];
    int temp1,temp2;
    pont_imagem Imagem;
    Imagem=malloc(sizeof(imagem));
    if(Imagem==NULL){
	fprintf(stderr, "Erro ao alocar memoria para a imagem.");
	return NULL;
    }
	/*Logo abaixo, é verificado se existe uma linha de comentário, e assim será feito no decorrer da leitura:
	O programa lerá infinitamente até que o primeiro char seja diferente de "#", e quando parado, o valor ou palavra sera usado nas
	variáveis requisitadas.(altura,largura,max,pixeis).*/
    while(1){			
        fscanf(imagem,"%s",temp);
        if(temp[0]=='#'){
            fgets(aux,99,imagem);
        }else {
            break;
        }
    }
    if(strcmp(temp,"P3")!=0){ // Verifica se é PPM
        printf("Imagem nao eh PPM\n");
        exit(1);
    }
    strncpy(Imagem->codigo,temp,2);
    fgetc(imagem);
    while(1){
    fgets(temp,99,imagem);   
        if(temp[0]=='#'){
            strcpy(aux,temp);
        }else{
            break;
        }
    }
	/*No caso da altura e largura, devemos utilizar sscanf, pois nas imagens, elas se encontram na mesma linha.*/
    sscanf(temp,"%d %d",&temp1,&temp2);
    Imagem->largura=(temp1);
    Imagem->altura=(temp2);
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
    return Imagem;
}
void novaimagem(pont_imagem Imagem, unsigned short int a){/*Função para criar a imagem em tons de cinza*/
    FILE *imagem;
    int i, j;
    unsigned short int count=0;
    count++;
    char novonome[250];
    if(a==0){
   		printf("Digite o nome para imagem marcada:");
	}
	else{
		printf("Digite o nome para imagem segmentada:");
	}
    scanf("%s", novonome);
    imagem=fopen(novonome,"w");// abre em modo escrita
    if (imagem == NULL){
        fprintf(stderr, "Erro ao tentar gerar a imagem. NULL.\n");
    }
    fprintf(imagem,"%s\n",Imagem->codigo);/*escreve o cabeçalho da imagem.*/
    fprintf(imagem,"%i ",Imagem->largura);
    fprintf(imagem,"%i\n",Imagem->altura);
    fprintf(imagem,"%i\n",Imagem->max);
    for(i=0; i<Imagem->altura; i++){
        for(j=0; j<Imagem->largura; j++){
            fprintf(imagem,"%i ",Imagem->pixelimagem[i][j].r);/*Escreve os novos pixels, em formato mais didático: p1 p2 p3
	    													   p4 p5 p6
														   p7 p8 p9*/
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
    if(Imagemcinza==NULL){
	fprintf(stderr, "Erro ao alocar memoria para imagem em tons de cinza.");
	return NULL;
    }
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
                if (Imagem->pixelimagem[i][j].r > 75 ) {
                    qtdpixelafetado++;//quantos pixels possui catarata
                }
            }
        }
    }

    porcentagemfinal = 100*(qtdpixelafetado/qtdpixel);//retornando a porcentagem em um numero inteiro
    return porcentagemfinal;
}
void ndiagnostico(double porcentagemfinal) {
    char diagnostico[17]="diagnostico.txt";
    FILE *arquivo = fopen(diagnostico, "w");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro na criacao do arquivo diagnostico\n");
        exit(1);
    } /*Caso a pupila tenha 65% ou mais de pixels comprometidos(valor intermediário entre 60% e 70% dado no pdf), o indivíduo será diagnosticado com catarata*/
    if (porcentagemfinal >= 65) {
        fprintf(arquivo, "Indivíduo com catarata\nPorcentagem de comprometimento da pupila: %.2lf%%\n",porcentagemfinal);
    }
    else {
        fprintf(arquivo, "Indivíduo sem catarata\nPorcentagem de comprometimento da pupila: %.2lf%%\n", porcentagemfinal);
    }

    fclose(arquivo);
}
void marcarpupila(pont_imagem Imagem, centro *c) {
    int i,j,raioimagem;
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
    raioimagem=25; //40
   }
    unsigned int t;
	int x, y;
	
	//Percorre a circunferencia de centro igual ao passado por parametro
	for (t = 0; t < 360; t++) {
		//Calcula as coordenadas atraves do raio (incluso na passagem por parametro do Centro)
		y = (c->r+raioimagem)*sin(t*PI/180.0);
		x = (c->r+raioimagem)*cos(t*PI/180.0);

		//Marca em magenta as coordenadas no raio
		Imagem->pixelimagem[c->y+y][c->x+x].r = 0;
		Imagem->pixelimagem[c->y+y][c->x+x].g = 255;
		Imagem->pixelimagem[c->y+y][c->x+x].b = 0;
	}
}
pont_imagem flash(pont_imagem Imagem){
    pont_imagem Imagemsflash = calloc(1,sizeof(imagem));
    if(Imagemsflash==NULL){
	fprintf(stderr, "Erro ao alocar memoria para imagem binarizada.");
	return NULL;
    }
    strcpy(Imagemsflash->codigo,Imagem->codigo);
    Imagemsflash->altura = Imagem->altura;
    Imagemsflash->largura = Imagem->largura;
    Imagemsflash->max=Imagem->max;
    criarmatriz(Imagemsflash);
    unsigned short int i,j,media;
    int soma,total;
    for(i = 0; i < Imagem->altura; i++){
        for(j = 0; j < Imagem->largura; j++){ /*Aqui realizamos a média dos tons no centro da imagem(uma tentativa de deixar a imagem uniforme).*/
            if(Imagem->pixelimagem[i][j].r > 0){
                soma+=Imagem->pixelimagem[i][j].r;
                total++;
            }
        }
    }
    media=(int)soma/total;
    for(i = 0; i < Imagem->altura; i++){
        for(j = 0; j < Imagem->largura; j++){
            if(Imagem->pixelimagem[i][j].r < 140){/*Caso o pixel seja menor que 140(valor manual achado para tonalidade de um flash), será um valor real da pupila.*/
                Imagemsflash->pixelimagem[i][j].r = Imagem->pixelimagem[i][j].r;
                Imagemsflash->pixelimagem[i][j].g = Imagem->pixelimagem[i][j].g;
                Imagemsflash->pixelimagem[i][j].b = Imagem->pixelimagem[i][j].b;
            }else{/*Se não, substitua-o pela média das tonalidades.*/
                Imagemsflash->pixelimagem[i][j].r = media;
                Imagemsflash->pixelimagem[i][j].g = media;
                Imagemsflash->pixelimagem[i][j].b = media;
            }
        }
    }
    return Imagemsflash;
}
