#include "common_functions.h"

// Sets the maximum decimal length.
void setMaxDecimalLength(unsigned int length);

// Updates the maximum decimal length.
void update_maxDecimalLength(char *);

// Checks if a character is a delimiter.
int isDelimiter(char ch);


// This variable stores the maximum decimal length.
static unsigned int maxDecimalLength = 0;

unsigned int getMaxDecimalLength()
{
    return maxDecimalLength;
}

void setMaxDecimalLength(unsigned int length)
{
    maxDecimalLength = length;
}


char *trim(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}


int tokenToInt(char *token)
{
    if (strstr(token, "INF") != NULL || atoi(token) == -1)
    {
        return INT_MAX;
    }
    return atoi(token);
}

float tokenToFloat(char *token)
{
    if (strstr(token, "INF") != NULL || atof(token) == -1.0)
    {
        return FLT_MAX;
    }
    return atof(token);
}

double tokenToDouble(char *token)
{
    if (strstr(token, "INF") != NULL || atof(token) == -1.0)
    {
        return DBL_MAX;
    }
    return atof(token);
}

char *readNextToken(FILE *file)
{
    size_t capacity = 10; // Initial capacity
    size_t len = 0;       // Current length of the token
    char *token = (char *)malloc(capacity * sizeof(char));
    if (!token)
        return NULL; // Allocation failed

    char ch;
    while ((ch = fgetc(file)) != EOF && !isDelimiter(ch))
    {
        // Resize token buffer if necessary
        if (len + 1 >= capacity)
        {
            capacity *= 2; // Double the capacity
            char *newToken = (char *)realloc(token, capacity * sizeof(char));
            if (!newToken)
            {
                free(token);
                exit(10); // Reallocation failed
            }
            token = newToken;
        }

        // Append the character to the token
        token[len++] = ch;
    }

    if (len == 0 && ch == EOF)
    { // No more tokens
        free(token);
        return NULL;
    }

    // Null-terminate the token
    token[len] = '\0';

    // Calculo la cantidad de lugares decimales
    char *dotPosition;

    if (token)
    {
        token = trim(token);
        update_maxDecimalLength(token);
    }
    return token;
}


void print_int_matrix_to_file(FW_Matrix *FW, FILE *file, int *matrix, unsigned int row, unsigned int col, unsigned int disconnected_str, char * string)
{
    int value = matrix[row * FW->norm_size + col];

    if (value == INT_MAX && disconnected_str)
    {
        fprintf(file, string);
    }
    else
    {
        fprintf(file, "%d", value == INT_MAX ? -1 : value);
    }
}

void print_float_matrix_to_file(FW_Matrix * FW, FILE *file, float *matrix, unsigned int row, unsigned int col, unsigned int disconnected_str, char * string)
{
    float value = matrix[row * FW->norm_size + col];

    if (value == FLT_MAX && disconnected_str)
    {
        fprintf(file, string);
    }
    else
    {
        fprintf(file, "%.*f", FW->decimal_length, value == FLT_MAX ? -1.0 : value);
    }
}

void print_double_matrix_to_file(FW_Matrix * FW, FILE * file, double *matrix, unsigned int row, unsigned int col, unsigned int disconnected_str, char * string)
{
    double value = matrix[row * FW->norm_size + col];

    if (value == DBL_MAX && disconnected_str)
    {
        fprintf(file, string);
    }
    else
    {
        fprintf(file, "%.*lf", FW->decimal_length, value == DBL_MAX ? -1.0 : value);

    }
}


FILE *open_result_file(const char *path, const char *extension)
{
    char fullPath[1024]; // Buffer para la ruta completa
    sprintf(fullPath, "%s_%s", path, extension);

    FILE *file = fopen(fullPath, "w");
    if (file == NULL)
    {
        printf("No se pudo abrir el archivo para escribir el JSON.\n");
        exit(1);
    }

    return file;
}


// Private functions -----------------------------------------------------------


void update_maxDecimalLength(char *token)
{
    char *dotPosition = strchr(token, '.');
    if (dotPosition != NULL)
    {
        unsigned int decimalLength = strlen(dotPosition + 1);

        if (decimalLength > maxDecimalLength)
        {
            maxDecimalLength = decimalLength;
        }
    }
}

int isDelimiter(char ch)
{
    return ch == ',' || ch == '\n' || ch == EOF || ch == ']';
}