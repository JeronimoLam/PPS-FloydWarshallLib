#ifndef FW_COMPUTE_H
#define FW_COMPUTE_H

#include "../include/FW_Lib_CommonTypes.h"

#include <omp.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void compute_FW_int_paralell(FW_Matrix FW, int threads_num, int no_path);
void compute_FW_int_sequential(FW_Matrix FW, int no_path);

void compute_FW_float_paralell(FW_Matrix FW, int threads_num, int no_path);
void compute_FW_float_sequential(FW_Matrix FW, int no_path);

void compute_FW_double_paralell(FW_Matrix FW, int threads_num, int no_path);
void compute_FW_double_sequential(FW_Matrix FW, int no_path);




#endif //FW_COMPUTE_H