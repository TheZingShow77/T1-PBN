#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char r, g, b;
} Pixel;

Pixel** loadPPM(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo '%s'. Verifique se o arquivo existe e se o nome está correto.\n", filename);
        exit(1);
    }

    char format[3];
    fscanf(file, "%s\n", format);
    if (strcmp(format, "P6") != 0) {
        printf("Formato de arquivo '%s' não suportado. O programa suporta apenas arquivos PPM no formato P6.\n", filename);
        fclose(file);
        exit(1);
    }

    fscanf(file, "%d %d\n255\n", width, height);

    Pixel** image = (Pixel**)malloc(*height * sizeof(Pixel*));
    for (int i = 0; i < *height; i++) {
        image[i] = (Pixel*)malloc(*width * sizeof(Pixel));
    }

    for (int i = 0; i < *height; i++) {
        if (fread(image[i], sizeof(Pixel), *width, file) != *width) {
            printf("Erro ao ler os dados da imagem. O arquivo pode estar corrompido ou não é um PPM válido.\n");
            fclose(file);
            exit(1);
        }
    }

    fclose(file);
    return image;
}

void savePPM(const char* filename, Pixel** image, int width, int height) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Erro ao salvar o arquivo '%s'. Verifique as permissões e o nome do arquivo.\n", filename);
        exit(1);
    }
    fprintf(file, "P6\n%d %d\n255\n", width, height);
    for (int i = 0; i < height; i++) {
        fwrite(image[i], sizeof(Pixel), width, file);
    }
    fclose(file);
}

void toGrayscale(Pixel** image, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char gray = 0.3 * image[i][j].r + 0.59 * image[i][j].g + 0.11 * image[i][j].b;
            image[i][j].r = image[i][j].g = image[i][j].b = gray;
        }
    }
}

void toNegative(Pixel** image, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j].r = 255 - image[i][j].r;
            image[i][j].g = 255 - image[i][j].g;
            image[i][j].b = 255 - image[i][j].b;
        }
    }
}

void toXRay(Pixel** image, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char temp = image[i][j].r;
            image[i][j].r = image[i][j].b;
            image[i][j].b = temp;
        }
    }
}

Pixel** rotate90(Pixel** image, int* width, int* height) {
    int newWidth = *height;
    int newHeight = *width;
    Pixel** rotated = (Pixel**)malloc(newHeight * sizeof(Pixel*));
    for (int i = 0; i < newHeight; i++) {
        rotated[i] = (Pixel*)malloc(newWidth * sizeof(Pixel));
    }

    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            rotated[j][newHeight - 1 - i] = image[i][j];
        }
    }

    *width = newWidth;
    *height = newHeight;
    return rotated;
}

void toOldStyle(Pixel** image, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char r = image[i][j].r;
            unsigned char g = image[i][j].g;
            unsigned char b = image[i][j].b;
            image[i][j].r = (r + g + b) / 3;
            image[i][j].g = (r + g + b) / 3 * 0.9;
            image[i][j].b = (r + g + b) / 3 * 0.7;
        }
    }
}

int main() {
    char filename[100];
    printf("Digite o nome da imagem PPM (ex: imagem.ppm): ");
    scanf("%s", filename);

    int width, height;
    Pixel** image = loadPPM(filename, &width, &height);

    if (width < 400 || height < 400) {
        printf("A imagem deve ter no mínimo 400x400 pixels.\n");
        return 1;
    }

    int option;
    printf("\nEscolha uma das opções abaixo:\n");
    printf("1. Tons de Cinza\n");
    printf("2. Negativa\n");
    printf("3. Raio-X\n");
    printf("4. Rotacionar 90 graus\n");
    printf("5. Envelhecida\n");
    printf("Opção: ");
    scanf("%d", &option);

    Pixel** processedImage = image;
    char outputFilename[100];

    switch (option) {
        case 1:
            toGrayscale(image, width, height);
            strcpy(outputFilename, "tons_de_cinza.ppm");
            break;
        case 2:
            toNegative(image, width, height);
            strcpy(outputFilename, "negativa.ppm");
            break;
        case 3:
            toXRay(image, width, height);
            strcpy(outputFilename, "raio_x.ppm");
            break;
        case 4:
            processedImage = rotate90(image, &width, &height);
            strcpy(outputFilename, "rotacionada_90.ppm");
            break;
        case 5:
            toOldStyle(image, width, height);
            strcpy(outputFilename, "envelhecida.ppm");
            break;
        default:
            printf("Opção inválida.\n");
            return 1;
    }

    savePPM(outputFilename, processedImage, width, height);
    printf("Imagem processada salva como: %s\n", outputFilename);

    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);
    if (processedImage != image) {
        for (int i = 0; i < height; i++) {
            free(processedImage[i]);
        }
        free(processedImage);
    }

    return 0;
}
