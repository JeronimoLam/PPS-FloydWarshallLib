#ifndef FW_LIB_FUNCTIONS_H
#define FW_LIB_FUNCTIONS_H

#include "FW_Lib_CommonTypes.h"


FW_Matrix create_structure(DataType, char *, int, FW_attr_t *);


/**
 * @brief Performs the Floyd-Warshall algorithm calculation on the provided matrix.
 *
 * This function implements the logic of the Floyd-Warshall algorithm to calculate the shortest distances between all pairs
 * of vertices in a graph, using the distance matrix contained in 'FW'. The calculations are parallelized to improve performance.
 *
 * @param FW The FW_Matrix struct containing the distance matrix and other parameters.
 * @param threads_num The number of threads to be used for parallel execution.
 * @param no_path Flag indicating whether to calculate the shortest path matrix (0) or not (1).
 */
void compute_FW_paralell(FW_Matrix, FW_attr_t *);



/**
 * Computes the Floyd-Warshall algorithm sequentially on the given FW_Matrix to calculate the shortest distances between all pairs
 * of vertices in a graph, using the distance matrix contained in 'FW'.
 *
 * @param matrix The FW_Matrix on which to perform the computation.
 * @param no_path Flag indicating whether to calculate the shortest path matrix (0) or not (1).
 */
void compute_FW_sequential(FW_Matrix, FW_attr_t *);


/**
 * @brief Saves the distance matrix or the shortest path matrix to a file.
 *
 * This function exports the distance matrix or the path matrix (as specified by the parameters) to a file
 * at the specified path. The function adjusts the file name to prevent overwrites and ensures the file format
 * is correct according to the specified type.
 *
 * @param FW The FW_Matrix structure containing the matrices to be exported.
 * @param path Path to the directory where the exported file will be saved.
 * @param name Base name for the exported file. The function may modify this name to prevent overwrites.
 * @param fileType File type for the export (e.g., binary or text).
 * @param dist_matrix Indicates whether the distance matrix should be exported (1) or not (0).
 * @param path_matrix Indicates whether the shortest path matrix should be exported (1) or not (0).
 * @note If both dist_matrix and path_matrix are set to 0, the function will not perform any action.
 */
void save_structure(FW_Matrix, char*, char*, FileType, FW_attr_t *);


/**
 * Prints the details of a FW_Matrix element and optionally the distance and path matrices.
 *
 * @param element The FW_Matrix element to print.
 * @param dist    A flag indicating whether to print the distance matrix (1) or not (0).
 * @param path    A flag indicating whether to print the path matrix (1) or not (0).
 *
 * @return A string containing the details of the FW_Matrix element.
 */
char *  FW_details_to_string(FW_Matrix element);

/**
 * Prints the FW_Matrix element along with its properties and optionally the distance and path matrices.
 *
 * @param element The FW_Matrix element to be printed.
 * @param dist Flag indicating whether to print the distance matrix (1) or not (0).
 * @param path Flag indicating whether to print the path matrix (1) or not (0).
 */
void print_FW(FW_Matrix element, int dist, int path, int blocks);


/**
 * @brief Frees the memory allocated for the FW_Matrix structure.
 * 
 * This function frees the dynamically allocated memory for the dist and path members of the FW_Matrix structure.
 * After freeing the memory, the function sets the pointers to NULL to avoid dangling pointers.
 * 
 * @param matrix A pointer to the FW_Matrix structure to be freed.
 */
void freeFW_Matrix(FW_Matrix* matrix);


FW_attr_t new_FW_attr();
void init_FW_attr(FW_attr_t *);




#endif //FW_LIB_FUNCTIONS_H