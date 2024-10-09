#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char inputFilename[100], outputFilename[100];
    printf("Digite o nome do arquivo P3 (ex: imagem.p3): ");
    scanf("%s", inputFilename);
    
    printf("Digite o nome do arquivo de saída P6 (ex: imagem.ppm): ");
    scanf("%s", outputFilename);

    FILE* inputFile = fopen(inputFilename, "r");
    if (!inputFile) {
        printf("Erro ao abrir o arquivo '%s'.\n", inputFilename);
        return 1;
    }

    char format[3];
    int width, height, maxVal;

    // Lê o formato do arquivo
    fscanf(inputFile, "%s", format);
    if (strcmp(format, "P3") != 0) {
        printf("Formato de entrada não é P3.\n");
        fclose(inputFile);
        return 1;
    }

    // Ignorar comentários no cabeçalho
    char c = fgetc(inputFile);
    while (c == '#') {
        while (fgetc(inputFile) != '\n');
        c = fgetc(inputFile);
    }
    ungetc(c, inputFile);

    // Lê dimensões e valor máximo
    fscanf(inputFile, "%d %d %d", &width, &height, &maxVal);

    // Verifica se a profundidade de cor é 255 (máximo para PPM)
    if (maxVal != 255) {
        printf("Valor máximo não é 255. O programa suporta apenas imagens com profundidade de 255.\n");
        fclose(inputFile);
        return 1;
    }

    // Abre o arquivo de saída
    FILE* outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        printf("Erro ao criar o arquivo '%s'.\n", outputFilename);
        fclose(inputFile);
        return 1;
    }

    // Escreve cabeçalho do P6
    fprintf(outputFile, "P6\n%d %d\n%d\n", width, height, maxVal);

    // Lê pixels e escreve em binário no novo arquivo P6
    for (int i = 0; i < height * width; i++) {
        int r, g, b;
        fscanf(inputFile, "%d %d %d", &r, &g, &b);
        fputc((unsigned char)r, outputFile);
        fputc((unsigned char)g, outputFile);
        fputc((unsigned char)b, outputFile);
    }

    printf("Conversão concluída: '%s' criado com sucesso.\n", outputFilename);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
