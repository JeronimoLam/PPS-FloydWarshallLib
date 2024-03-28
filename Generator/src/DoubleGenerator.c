#include "matrix_operations.h"

void generarMatrizDouble(int n, double ***matriz, double maxValue) {
    *matriz = (double **)malloc(n * sizeof(double *));
    if (*matriz == NULL)
    {
        printf("No se pudo reservar memoria para la matriz.\n");
        exit(1);
    }
    
    for (int i = 0; i < n; i++) {
        (*matriz)[i] = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            if (i == j)
                (*matriz)[i][j] = 0;
            else
            {
                // Generate a random float between 0 and (maxValue + 1) to include -1 in the range
                double randomValue = (double)rand() / (double)(RAND_MAX) * (maxValue + 1);
                // Check if the randomValue is greater than maxValue, if so, assign -1
                // This effectively adds -1 to the range of generated values
                if (randomValue > maxValue)
                    (*matriz)[i][j] = -1;
                else
                    (*matriz)[i][j] = randomValue;
            }
        }
    }
}


void guardarMatrizCSVDouble(double **matriz, int n, const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escribir el CSV.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "%.16lf", matriz[i][j]);
            if (j < n - 1) fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void guardarMatrizJSONDouble(double **matriz, int n, const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escribir el JSON.\n");
        exit(1);
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"tipo\": \"double\",\n");
    fprintf(file, "  \"dimension\": %d,\n", n);
    fprintf(file, "  \"matriz\": [\n");

    for (int i = 0; i < n; i++) {
        fprintf(file, "    [");
        for (int j = 0; j < n; j++) {
            fprintf(file, "%.16lf", matriz[i][j]);
            if (j < n - 1) fprintf(file, ", ");
        }
        fprintf(file, "]");
        if (i < n - 1) fprintf(file, ",\n");
    }

    fprintf(file, "\n  ]\n");
    fprintf(file, "}\n");

    fclose(file);
}

