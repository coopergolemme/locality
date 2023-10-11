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

#ifndef TRANSFORMATIONS_INCLUDED
#define TRANSFORMATIONS_INCLUDED

#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

// typedef A2Methods_UArray2 A2; 


A2Methods_UArray2 rotate(Pnm_ppm source_ppm, int degree, A2Methods_T method_type);

#endif