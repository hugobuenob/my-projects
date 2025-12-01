#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Predict_SVM.h"
#include "LBPFeatures.h"

#define BMP_HEADER_SIZE 54
#define PIXELS 128

/*Funcionamiento: se abre el archivo test.txt, que contiene el nombre de la imagen a probar y su clase.
* Mediante strtok() se separa el nombre (imagen.bmp) y se concatena con la ruta que lleva a las imagenes para leerla.
* La siguiente llamada a strtok() lee la clase a la que pertenece. imread() lee el .bmp y lo transforma en una matriz de 128x128,
* que se pasa a LBPfeatures() o como se termine llamando. Despues va a Predict_svm(). Por ultimo se compara la clasificacion obtenida con
* la clase real de la imagen, obtenida de test.txt. Los aciertos se suman a True y los fallos a False, y finalmente se imprime el porcentaje
* de aciertos, que segun dice Andres en el video de moodle deberia estar entorno al 50%.
*/

void imread(const char *filename, unsigned char image[PIXELS][PIXELS]);
//void LBPFeatures_custom(const unsigned char *image, int width, int height, double *features);
//int Predict_SVM(const double *features, const void *Model_SVM_Custom);

// Function to read a 128x128 grayscale BMP image file
void imread(const char *filename, unsigned char image[PIXELS][PIXELS]) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    // BMP Header is 54 bytes
    unsigned char bmpHeader[BMP_HEADER_SIZE];
    fread(bmpHeader, sizeof(unsigned char), BMP_HEADER_SIZE, file);

    // Extract image height and width from BMP header
    int width = *(int*)&bmpHeader[18];
    int height = *(int*)&bmpHeader[22];

    // Ensure the file is 128x128
    if (width != PIXELS || height != PIXELS) {
        fprintf(stderr, "Unsupported image size: %dx%d\n", width, height);
        fclose(file);
        exit(1);
    }

    unsigned char *row = (unsigned char *)malloc(width);

    for (int i = 0; i < height; i++) {
        fread(row, sizeof(unsigned char), width, file);
        for (int j = 0; j < width; j++) {
            // Grayscale image: directly read the intensity value
            image[height - i - 1][j] = row[j];
        }
    }

    free(row);
    fclose(file);
}

int main() {

    FILE *fid = fopen("Outex-TC-00010/000/test.txt", "r");
    if (fid == NULL) {
        perror("Error opening file");
        return 1;
    }

    int True = 0, False = 0;
    char tline[HISTOGRAM_WIDTH];
    int K_ant = -1;

    while (fgets(tline, sizeof(tline), fid)) {  //Para cada linea (i.e. cada imagen)
        char *imageName = strtok(tline, " \t\n");
        char FullName[HISTOGRAM_WIDTH];
        snprintf(FullName, sizeof(FullName), "Outex-TC-00010/images/%s", imageName);

        char* imageClass = strtok(NULL, " \t\n");
        int classLabel = atoi(imageClass);

        // A new set of images is announced when the classification type changes.
        if (classLabel != K_ant) {
            printf("------------------------- \n");
            printf("New Set: %d \n", classLabel);
            printf("True: %d \n", True);
            printf("False: %d \n", False);
        }

        K_ant = classLabel;

        unsigned char Im[PIXELS][PIXELS];
        int width, height;
        imread(FullName, Im);

        double Infer_Features[HISTOGRAM_WIDTH]; // Assuming 256 features for LBP

        // ESTA LLAMADA HAY QUE ADAPTARLA
        //LBPFeatures_custom(Im, Infer_Features);
        //ESTA LLAMADA HAY QUE ADAPTARLA TAMBIEN
        //int Predict_Label = Predict_SVM(Infer_Features, NULL); // Model_SVM_Custom needs to be defined

        if (Predict_Label == classLabel) {
            True = True + 1;  //does True++ save time?
        } else {
            False = False +1; //does False++ save time?
        }
    }

    fclose(fid);

    printf("True: %d \n", True);
    printf("False: %d \n", False);
    printf("Percentage: %.2f \n", 100.0 * True / (True + False));

    return 0;
}
//#include <stdio.h>
//#include <stdlib.h>
