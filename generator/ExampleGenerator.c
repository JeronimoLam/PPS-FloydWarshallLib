#include "src/matrix_operations.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INT_VALUE 100

#define DEFAULT_TYPE 'i'
#define DEFAULT_SIZE 10
#define DEFAULT_DIRECTION 1
#define DEFAULT_FORMAT 'C'
#define DEFAULT_CSV_PATH "./output.csv"
#define DEFAULT_JSON_PATH "./output.json"
#define DEFAULT_DECIMAL_ZERO 0
#define DEFAULT_IS_FLOAT 0
#define DEFAULT_NEGATIVOS_DENSIDAD 0.6
#define DEFAULT_PARTE_ENTERA 4
#define DEFAULT_PARTE_DECIMAL 12


void readInput(char *input, size_t size);

int main()
{
    char tipo = DEFAULT_TYPE;
    int is_float = DEFAULT_IS_FLOAT;
    int n = DEFAULT_SIZE;
    int direction = DEFAULT_DIRECTION;
    char formato = DEFAULT_FORMAT;
    char path[256] = {0};
    char input[256];
    const char *defaultPath;
    int parteEntera, parteDecimal, decimalCero = DEFAULT_DECIMAL_ZERO;
    double porcentajeCompletitud = DEFAULT_NEGATIVOS_DENSIDAD;

    // Tipo de números
    do
    {
        printf("Ingrese el tipo de numero (i = integer, d = decimal) [default: %c]: ", DEFAULT_TYPE);
        readInput(input, sizeof(input));
        if (input[0] == '\n')
        {
            break; // Sale del bucle ya que se eligió el valor predeterminado.
        }
    } while (sscanf(input, "%c", &tipo) != 1 || (tipo != 'i' && tipo != 'd'));

    // Dimensiones de la matriz
    do
    {
        printf("Ingrese las dimensiones de la matriz [default: %d]: ", DEFAULT_SIZE);
        readInput(input, sizeof(input));
        if (input[0] == '\n')
        {
            break; // Sale del bucle ya que se eligió el valor predeterminado.
        }
    } while (sscanf(input, "%d", &n) != 1 || n <= 0);

    // Tipo de Grafo
    do
    {
        printf("Tipo de Grafo: (1: Dirigido | 0: No dirigido) [default: %d]: ", DEFAULT_DIRECTION);
        readInput(input, sizeof(input));
        if (input[0] == '\n')
        {
            break; // Sale del bucle ya que se eligió el valor predeterminado.
        }
    } while (sscanf(input, "%d", &direction) != 1 || (direction != 1 && direction != 0));

    // Configuración adicional para números flotantes y dobles
    if (tipo == 'd')
    {
        parteEntera = DEFAULT_PARTE_ENTERA;
        parteDecimal = DEFAULT_PARTE_DECIMAL;

        do
        {
            printf("Ingrese la parte entera [default: %d]: ", parteEntera);
            readInput(input, sizeof(input));
            if (input[0] == '\n')
                break; // Presionó Enter, usar valor predeterminado

        } while (sscanf(input, "%d", &parteEntera) != 1);

        do
        {
            printf("Ingrese la parte decimal [default: %d]: ", parteDecimal);
            readInput(input, sizeof(input));
            if (input[0] == '\n')
                break; // Presionó Enter, usar valor predeterminado

        } while (sscanf(input, "%d", &parteDecimal) != 1);

        do
        {
            printf("Parte decimal toda en 0? (1: Yes | 0: No) [default: %d]: ", DEFAULT_DECIMAL_ZERO);
            readInput(input, sizeof(input));
            if (input[0] == '\n')
                break; // Presionó Enter, usar valor predeterminado

        } while (sscanf(input, "%d", &decimalCero) != 1 || (decimalCero != 1 && decimalCero != 0));
    }

    // Densidad de -1
    do
    {
        printf("Porcentaje de completitud [default: %.1f]: ", DEFAULT_NEGATIVOS_DENSIDAD);
        readInput(input, sizeof(input));
        if (input[0] == '\n')
            break; // Sale del bucle ya que se eligió el valor predeterminado.

    } while (sscanf(input, "%lf", &porcentajeCompletitud) != 1);

    // Formato de salida
    do
    {
        printf("Ingrese el formato de salida (C para CSV, J para JSON) [default: %c]: ", DEFAULT_FORMAT);
        readInput(input, sizeof(input));
        if (input[0] == '\n')
        {
            break;
        }
        else if (sscanf(input, "%c", &formato) == 1 && (formato == 'C' || formato == 'c' || formato == 'J' || formato == 'j'))
        {
            break; // Sale del bucle si se ingresa una opción válida.
        }
    } while (1);

    defaultPath = (formato == 'J' || formato == 'j') ? DEFAULT_JSON_PATH : DEFAULT_CSV_PATH;

    if ((tipo == 'd' || tipo == 'D') && (formato == 'j' || formato == 'J'))
    {
        // Tipo de números
        do
        {
            printf("1 = float | 0 = double: [default: %d]: ", DEFAULT_IS_FLOAT);
            readInput(input, sizeof(input));
            if (input[0] == '\n')
            {
                break; // Sale del bucle ya que se eligió el valor predeterminado.
            }
        } while (sscanf(input, "%d", &is_float) != 1 || (is_float != 1 && is_float != 0));
    }

    // Path del archivo de salida
    printf("Ingrese el path del archivo de salida [default: %s]: ", defaultPath);
    readInput(input, sizeof(input));
    if (sscanf(input, "%s", path) != 1)
    {
        strcpy(path, defaultPath);
    }

    srand(time(NULL)); // Inicialización del generador de números aleatorios

    // Imprime los valores ingresados
    printf("\nTipo de numero: %c\n", tipo);
    printf("Dimensiones de la matriz: %d\n", n);
    printf("Tipo de Grafo: %s\n", direction == 1 ? "Dirigido" : "No dirigido");
    if (tipo == 'f' || tipo == 'd')
    {
        printf("Parte entera: %d\n", parteEntera);
        printf("Parte decimal: %d\n", parteDecimal);

        printf("Parte decimal toda en 0: %s\n", decimalCero ? "Si" : "No");
    }
    printf("Porcentaje de completitud: %.1f\n", porcentajeCompletitud);
    printf("Formato de salida: %c\n", formato);
    printf("Path del archivo de salida: %s\n\n", path);

    //  Generar la matriz y guardarla en el archivo
    if (tipo == 'i')
    {
        int **matrizInt;
        IntMatrixGenerator(n, &matrizInt, MAX_INT_VALUE, 1 - porcentajeCompletitud, direction);
        if (formato == 'C' || formato == 'c')
        {
            IntMatrix2CSV(matrizInt, n, path);
        }
        else
        {
            IntMatrix2JSON(matrizInt, n, path);
        }
        // Liberar memoria
        for (int i = 0; i < n; i++)
            free(matrizInt[i]);
        free(matrizInt);
    }
    else if (tipo == 'd')
    {
        double **matrizDouble;
        DoubleMatrixGenerator(n, &matrizDouble, parteEntera, parteDecimal, decimalCero, 1 - porcentajeCompletitud, direction);
        if (formato == 'C' || formato == 'c')
        {
            DoubleMatrix2CSV(matrizDouble, n, path, parteDecimal, decimalCero);
        }
        else
        {
            DoubleMatrix2JSON(matrizDouble, n, path, parteDecimal, decimalCero, is_float);
        }
        // Liberar memoria
        for (int i = 0; i < n; i++)
            free(matrizDouble[i]);
        free(matrizDouble);
    }
    else
    {
        printf("Tipo de número no reconocido.\n");
        return 1;
    }

    printf("Archivo generado con exito en '%s'.\n\n", path);

    return 0;
}


//------------------------------------ Private Functions ------------------------------------//

void readInput(char *input, size_t size) {
    if (fgets(input, size, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        exit(EXIT_FAILURE);
    }
}
