#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int r;
    int g;
    int b;
} Pixel;

void menu();
void openImage(char image_name[], int style);
void processImage(FILE *fp, int style);
void imageInfo(FILE *fp, int *coluna, int *linha, int *val);
Pixel** rotateImage90(Pixel **matriz, int linha, int coluna, int *novaLinha, int *novaColuna);

int main() {
    int op; 
    char image_name[100];
    
    printf("\n---------------- EDIT IMAGE MENU ----------------\n\n");
    printf("Enter the name of the image you want to edit: ");
    scanf("%s", image_name);

    while (1) {
        menu();
        scanf("%d", &op);

        if (op == 0) {
            printf("Finishing program.\n");
            break;
        }

        if (op < 1 || op > 5) {
            printf("Invalid option. Please try again.\n");
            continue;
        }

        openImage(image_name, op);
    }

    return 0;
}

void openImage(char image_name[], int style) {
    FILE *fp = fopen(image_name, "r");

    if (fp == NULL) {
        printf("\nError opening the file %s\n", image_name);
        return;
    }

    processImage(fp, style);
}

void processImage(FILE *fp, int style) {
    int coluna, linha, val;
    int r, g, b;

    imageInfo(fp, &coluna, &linha, &val);

    Pixel **matriz = (Pixel **)malloc(linha * sizeof(Pixel *));
    for (int i = 0; i < linha; i++) {
        matriz[i] = (Pixel *)malloc(coluna * sizeof(Pixel));
    }

    for (int i = 0; i < linha; i++) {
        for (int j = 0; j < coluna; j++) {
            fscanf(fp, "%d %d %d", &r, &g, &b);

            switch (style) {
                case 1: // Tons de cinza
                    matriz[i][j].r = matriz[i][j].g = matriz[i][j].b = (r * 0.30) + (g * 0.59) + (b * 0.11);
                    break;

                case 2: // Negativa
                    matriz[i][j].r = 255 - r;
                    matriz[i][j].g = 255 - g;
                    matriz[i][j].b = 255 - b;
                    break;

                case 3: // Efeito envelhecido
                    matriz[i][j].r = (r * 0.393) + (g * 0.769) + (b * 0.189);
                    matriz[i][j].g = (r * 0.349) + (g * 0.686) + (b * 0.168);
                    matriz[i][j].b = (r * 0.272) + (g * 0.534) + (b * 0.131);

                    if (matriz[i][j].r > 255) matriz[i][j].b = 255;
                    if (matriz[i][j].g > 255) matriz[i][j].g = 255;
                    if (matriz[i][j].b > 255) matriz[i][j].r = 255;
                    break;

                case 4: // Raio-X
                    matriz[i][j].r = matriz[i][j].g = matriz[i][j].b = 255 - ((r * 0.30) + (g * 0.59) + (b * 0.11));
                    break;

                case 5: // Rotacionar 90 graus
                    break;
            }
        }
    }

    fclose(fp);
    printf("Image successfully read.\n");

    FILE *fp_saida = fopen("output_image.ppm", "w");

    if (fp_saida == NULL) {
        printf("\nError opening the file\n");
        for (int i = 0; i < linha; i++) {
            free(matriz[i]);
        }
        free(matriz);
        return;
    }

    fprintf(fp_saida, "P3\n");
    fprintf(fp_saida, "%d %d\n", coluna, linha);
    fprintf(fp_saida, "%d\n", val);

    for (int i = 0; i < linha; i++) {
        for (int j = 0; j < coluna; j++) {
            fprintf(fp_saida, "%d %d %d ", matriz[i][j].r, matriz[i][j].g, matriz[i][j].b);
        }
        fprintf(fp_saida, "\n");
    }

    fclose(fp_saida);

    for (int i = 0; i < linha; i++) {
        free(matriz[i]);
    }
    free(matriz);

    printf("Image successfully processed and saved as output_image.ppm\n");
}


void imageInfo(FILE *fp, int *coluna, int *linha, int *val) {
    char tipoImg[3];
    fscanf(fp, "%s", tipoImg);
    fscanf(fp, "%d %d", coluna, linha);
    fscanf(fp, "%d", val);
}

void menu() {
    printf("\n------------ EDIT IMAGE MENU ------------\n\n");
    printf("[1] - Convert image to grey.\n");
    printf("[2] - Convert image to negative.\n");
    printf("[3] - Convert image to aged.\n");
    printf("[4] - Convert image to X-ray.\n");
    printf("[5] - Rotate image 90 degrees.\n");
    printf("[0] - Finish the program.\n");
    printf("Option: ");
}
