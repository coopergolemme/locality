/**************************************************************
 *
 *                     fileutil.h
 *
 *      Assignment: locality
 *      Authors: Liam Smith (lsmith26) and Cooper Golemme (cgolem01)
 *      Date: October 10, 2023
 * 
 *      Utility functions used for reading and writing from and to image files
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
