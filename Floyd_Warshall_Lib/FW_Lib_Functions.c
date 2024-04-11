#include "../include/FW_Lib_Functions.h"
#include "File_Manager/file_handler.h"
#include "FW_compute.h"

#define DEFAULT_BLOCK_SIZE 128
#define DEFAULT_THREAD_NUM 4
#define DEFAULT_OUTPUT_FORMAT 1     // Imprime INF en lugar de -1 por defecto
#define DEFAULT_PRINT_DIST_MATRIX 1 // Imprime la matriz de distancia por defecto
#define DEFAULT_NO_PATH 1           // No imprime ni procesa la matriz de caminos por defecto

// Private functions
void print_matrix(void *, unsigned int, DataType);
char *dataTypeToString(DataType);
unsigned int nextPowerOf2(unsigned int);
int *initializePathMatrix(FW_Matrix *G);

// Lib Functions
FW_Matrix create_structure(DataType dataType, char *path, int BS, FW_attr_t *attr)
{
    FW_attr_t local_attr;
    if (attr == NULL)
    {
        local_attr = new_FW_attr();
    }
    else
    {
        local_attr = *attr;
    }

    FW_Matrix FW;
    FILE *file = NULL;

    // Open the file and set file type
    file = getFile(path);
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open the file.\n");
        exit(EXIT_FAILURE);
    }
    FW.fileType = getFileType();

    // Autodetect of datatype if not specified
    if (dataType == UNDEFINED)
    {
        FW.datatype = AutoDetectDataType(FW.fileType, file);
    }
    else
    {
        FW.datatype = dataType;
    }

    // Calculation of matrix size
    FW.size = calculateMatrixSize(FW.fileType, file); // Calulates rows and cols
    FW.norm_size = nextPowerOf2(FW.size);             // Calculates the max size of the matrix (nxn)

    // Set Block Size
    if (BS != -1)
    {
        FW.BS = BS;
    }
    else
    {
        FW.BS = DEFAULT_BLOCK_SIZE;
    }

    createMatrixes(&FW, file, local_attr.no_path); // TODO: Revisar tema de espacio en memoria al pasar el FW como parametro. Se duplican las matrices?

    // print path matrix
    // print_matrix(FW.dist, FW.norm_size, FW.datatype);
    print_matrix(FW.path, FW.norm_size, TYPE_INT);

    return FW;
}

void compute_FW_paralell(FW_Matrix FW, FW_attr_t *attr)
{

    FW_attr_t local_attr;
    if (attr == NULL)
    {
        local_attr = new_FW_attr();
    }
    else
    {
        local_attr = *attr;
    }

    // Set Thread Num
    // if (threads_num == -1)
    // {
    //     threads_num = DEFAULT_BLOCK_SIZE;
    // }

    switch (FW.datatype)
    {
    case TYPE_INT:
        compute_FW_int_paralell(FW, local_attr.thread_num, local_attr.no_path);
        break;
    case TYPE_FLOAT:
        compute_FW_float_paralell(FW, local_attr.thread_num, local_attr.no_path);
        break;
    case TYPE_DOUBLE:
        compute_FW_double_paralell(FW, local_attr.thread_num, local_attr.no_path);
        break;

    default:
        fprintf(stderr, "Error: Unsupported data type for Floyd-Warshall computation.\n");
        exit(EXIT_FAILURE);
    }
}

void compute_FW_sequential(FW_Matrix FW, FW_attr_t *attr)
{
    FW_attr_t local_attr;
    if (attr == NULL)
    {
        local_attr = new_FW_attr();
    }
    else
    {
        local_attr = *attr;
    }

    switch (FW.datatype)
    {
    case TYPE_INT:
        compute_FW_int_sequential(FW, local_attr.no_path);
        break;
    case TYPE_FLOAT:
        compute_FW_float_sequential(FW, local_attr.no_path);
        break;
    case TYPE_DOUBLE:
        compute_FW_double_sequential(FW, local_attr.no_path);
        break;

    default:
        fprintf(stderr, "Error: Unsupported data type for Floyd-Warshall computation.\n");
        exit(EXIT_FAILURE);
    }
}

void save_structure(FW_Matrix FW, char *path, char *name, FileType fileType, FW_attr_t *attr)
{
    FW_attr_t localAttr;

    if (attr == NULL)
    {
        // attr es NULL, usa valores predeterminados
        localAttr = new_FW_attr();
    }
    else
    {
        // attr no es NULL, usa los valores proporcionados
        localAttr = *attr;
    }

    if (localAttr.print_distance_matrix == 0 & localAttr.no_path == 1)
    {
        printf("Select a matrix to export\n");
        return;
    }

    // Create copies of path and name for manipulation
    char pathCopy[1024];
    char nameCopy[256]; // Assuming name length won't exceed 256, adjust as needed
    strncpy(pathCopy, path, sizeof(pathCopy) - 1);
    strncpy(nameCopy, name, sizeof(nameCopy) - 1);
    pathCopy[sizeof(pathCopy) - 1] = '\0'; // Ensure null termination
    nameCopy[sizeof(nameCopy) - 1] = '\0'; // Ensure null termination

    // Remove trailing '/' or '\' from pathCopy
    int pathLength = strlen(pathCopy);
    if (pathCopy[pathLength - 1] == '/' || pathCopy[pathLength - 1] == '\\')
    {
        pathCopy[pathLength - 1] = '\0';
    }

    // Modify nameCopy to contain content only until the first '.'
    char *dotPosition = strchr(nameCopy, '.');
    if (dotPosition != NULL)
    {
        *dotPosition = '\0';
    }

    char fullPath[1280]; // Buffer for full path
    sprintf(fullPath, "%s/%s", pathCopy, nameCopy);
    // printf("Full Path: %s\n", fullPath);

    saveMatrix(FW, fullPath, fileType, localAttr.print_distance_matrix, localAttr.no_path, localAttr.text_in_output);
}

void freeFW_Matrix(FW_Matrix *matrix)
{
    if (matrix != NULL)
    {
        // Free the dynamically allocated dist member, if it's not NULL.
        if (matrix->dist != NULL)
        {
            free(matrix->dist);
            matrix->dist = NULL; // Avoid dangling pointer
        }
        // Free the dynamically allocated path member, if it's not NULL.
        if (matrix->path != NULL)
        {
            free(matrix->path);
            matrix->path = NULL; // Avoid dangling pointer
        }
    }
}

char *FW_details_to_string(FW_Matrix *element, FW_attr_t *attr)
{
    char *result_martix = "";
    char *result_attr = "";
    if (element != NULL)
        sprintf(result_martix, "Matrix Size: %d\nNormalized Size: %d\nBlock Size: %d\nData Type: %s\nDecimal Part: %d\n", element->size, element->norm_size, element->BS, dataTypeToString(element->datatype), element->decimal_length);
    if (attr != NULL)
        sprintf(result_attr, "Thread Num: %d\nNo Path: %s\nPrint distance matrix: %s\nText in output: %s", attr->thread_num, attr->no_path ? "True" : "False", attr->print_distance_matrix ? "True" : "False", attr->text_in_output ? "INF" : "-1");
    if (element != NULL && attr != NULL)
    {
        sprintf(result_martix, "%s\n%s", result_martix, result_attr);
        return result_martix;
    }
    return strcat(result_martix, result_attr);
}

void print_FW_matrixes(FW_Matrix * element, char * print, int blocks)
{

    if (strstr(print, "all") || strstr(print, "dist"))
    {
        if (blocks)
        {
            printf("Distance Matrix Loaded in blocks\n");
            print_matrix(element->dist, element->norm_size, element->datatype);
            printf("\n");
        }
        if (!blocks)
        {
            printf("Distance Matrix Loaded Normal\n"); // TODO: Check implementation of this
            print_matrix(reorganizeToLinear(element->dist, element->norm_size, element->BS, element->datatype), element->size, element->datatype);
            printf("\n");
        }
    }
    if (strstr(print, "all") || strstr(print, "path"))
    {
        if (blocks)
        {
            printf("Path Matrix Loaded in blocks\n");
            print_matrix(element->path, element->norm_size, TYPE_INT);
            printf("\n");
        }
        if (!blocks)
        {
            printf("Path Matrix Loaded Normal\n"); // TODO: Check implementation of this
            print_matrix(reorganizeToLinear(element->path, element->norm_size, element->BS, TYPE_INT), element->size, TYPE_INT);
            printf("\n");
        }
    }
    printf("\n");
}

//----------------------------------------------- ATTR Init -----------------------------------------
FW_attr_t new_FW_attr()
{
    FW_attr_t attr;
    attr.text_in_output = DEFAULT_OUTPUT_FORMAT;
    attr.print_distance_matrix = DEFAULT_PRINT_DIST_MATRIX;
    attr.no_path = DEFAULT_NO_PATH;
    attr.thread_num = DEFAULT_THREAD_NUM;

    return attr;
}

void init_FW_attr(FW_attr_t *attr)
{
    attr->text_in_output = DEFAULT_OUTPUT_FORMAT;
    attr->print_distance_matrix = DEFAULT_PRINT_DIST_MATRIX;
    attr->no_path = DEFAULT_NO_PATH;
    attr->thread_num = DEFAULT_THREAD_NUM;
}

// ------------------------------------------------------------------ Private Section ------------------------------------------------------------------

// AUX Functions
void print_matrix(void *matrix, unsigned int size, DataType dataType)
{
    int i, j;
    switch (dataType)
    {
    case TYPE_INT:
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                printf("%d ", ((int *)matrix)[i * size + j]);
            }
            printf("\n");
        }
        break;
    case TYPE_FLOAT:
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                float value = ((float *)matrix)[i * size + j];
                printf("%f ", value);
            }
            printf("\n");
        }
        break;
    case TYPE_DOUBLE:
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                double value = ((double *)matrix)[i * size + j];
                printf("%lf ", value);
            }
            printf("\n");
        }
        break;
    default:
        printf("Unsupported data type for printing.\n");
        break;
    }
    printf("\n");
}

char *dataTypeToString(DataType dt)
{
    char *result = malloc(30); // allocate enough memory for the prefix and the datatype string
    switch (dt)
    {
    case TYPE_INT:
        result = "INT";
        break;
    case TYPE_FLOAT:
        result = "INT";

        break;
    case TYPE_DOUBLE:
        result = "DOUBLE";
        break;
    case UNDEFINED:
    default:
        result = "UNDEFINED";
    }
    return result;
}

unsigned int nextPowerOf2(unsigned int n)
{
    unsigned count = 0;

    // First n in the below condition is for the case where n is 0
    if (n && !(n & (n - 1)))
    {
        return n;
    }

    while (n != 0)
    {
        n >>= 1;
        count += 1;
    }

    return 1 << count;
}