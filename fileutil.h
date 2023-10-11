/**************************************************************
 *
 *                     sudoku.c
 *
 *     Assignment: iii
 *     Authors: Liam Smith (lsmith26) and Yoda Ermias (yermia01)
 *     Date: Sep 28, 2023
 * 
 *     For properly formatted 9 x 9 files, the files are interpreted as
 *     sudoku solutions or not accordng to typical game rules.
 *
 *
 **************************************************************/

#ifndef FILEUTIL_INCLUDED
#define FILEUTIL_INCLUDED

#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

typedef A2Methods_UArray2 A2;

Pnm_ppm make_A2(char *filename, A2Methods_T method_type);
FILE *open_or_fail(char *filename, char *mode);
void write_A2(A2 transformed, Pnm_ppm source, A2Methods_T method_type);
void write_timing(char *timing_file, double time, int width, int height);


#endif
