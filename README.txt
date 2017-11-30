# Projeto-de-ITP (2017.2)
Projeto de ITP - Diagnóstico de Catarata
Autores: Victor Hugo Freire Ramalho
         Oziel Alves do Nascimento Junior

Nesse projeto fizemos um programa em C para analisar 4 imagens (duas com catarata e duas sem catarata) e emitir o diagnóstico final de cada. Para isso realizamos algumas etapas: 

-Transformação da imagem em tons de cinza.
-Aplicação do filtro gaussiano.
-Aplicação do filtro de sobel.
-Binarização.
-Transformada de Hough.
-Detecção de pixels defeituosos.
-Diagnóstico se o olho analisado possui ou não catarata e o comprometimento.

Funcionalidades básicas: Todos funcionalidas básicas foram atendidas.

Funcionalidas Extras: 
1. Imagem em RGB com contorno da pupila; 
2. O progama ignora os comentarios em toda a leitura;
3. Free em imagens, otimizando a memória e funcionamento.
4. Segmentação do flash.
5. Opção de gerar a imagem com o flash borrado(obtendo uma maior precisão no diagnóstico).
6. Opção na criação das imagens em cada etapa.
7. Opção na escolha do nome do diagnostico.


Para compilar:
Digite no terminal: gcc main.c funcoes.c -o catarata -W -lm

Para executar:
               (nomedoexecutavel) (nomeimagem) (formato) (nomediagnostico)
Digite no terminal: ./catarata -i Catarata.ppm -f ppm -o diagnostico.txt 
