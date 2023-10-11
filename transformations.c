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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assert.h"

#include "transformations.h"

typedef A2Methods_UArray2 A2; 
static A2Methods_T methods;


void apply_rotate90(int i, int j, A2 array2, void *source, void *dest);
void apply_rotate180(int i, int j, A2 array2, void *source, void *dest);
void apply_rotate270(int i, int j, A2 array2, void *source, void *dest);

/********** UArray2b_free ********
 *
 * Purpose:
 * 
 * Inputs: 
 *	    
 * Return: 
 *
 * Notes: 
 * 
 ************************/
A2 rotate(Pnm_ppm source_ppm, int degree, A2Methods_T method_type)
{
        methods = method_type; 

    // printf("INSIDE ROTATE\n");
    
    A2 source = source_ppm->pixels;
    int source_width = methods->width(source);
    int source_height = methods->height(source);
    int pixel_size = methods->size(source_ppm->pixels);

        A2 dest;
    
        if (degree == 90) {
                dest = methods->new(source_height, source_width, pixel_size);
                methods->map_default(source, apply_rotate90, dest);

        } else if (degree == 180) {
                dest = methods->new(source_width, source_height, pixel_size);
                methods->map_default(source, apply_rotate180, dest);

        } else if (degree == 270) {
                dest = methods->new(source_height, source_width, pixel_size);
                methods->map_default(source, apply_rotate270, dest);

        } else if (degree == 0) {
                return source; // fix this, maybe do the same as above with zero function
            
        } else {
                return NULL; 
        }
        return dest;
}

/********** UArray2b_free ********
 *
 * Purpose:
 * 
 * Inputs: 
 *	    
 * Return: 
 *
 * Notes: 
 * 
 ************************/
void apply_rotate90(int i, int j, A2 array2, void *source, void *dest)
{
        (void) array2;
        Pnm_rgb dest_unupdated = methods->at(dest, methods->width(dest) - j - 1, i);
        *dest_unupdated = *(Pnm_rgb)source;
}

/********** UArray2b_free ********
 *
 * Purpose:
 * 
 * Inputs: 
 *	    
 * Return: 
 *
 * Notes: 
 * 
 ************************/
void apply_rotate180(int i, int j, A2 array2, void *source, void *dest)
{

        (void) array2;
        Pnm_rgb dest_unupdated = methods->at(dest, methods->width(dest) - i - 1, 
                                                methods->height(dest) - j - 1);
        *dest_unupdated = *(Pnm_rgb)source;
}

/********** UArray2b_free ********
 *
 * Purpose:
 * 
 * Inputs: 
 *	    
 * Return: 
 *
 * Notes: 
 * 
 ************************/
void apply_rotate270(int i, int j, A2 array2, void *source, void *dest)
{
        (void) array2;
        Pnm_rgb dest_unupdated = methods->at(dest, i, methods->width(dest) - j - 1);
        *dest_unupdated = *(Pnm_rgb)source;
}

// extra
A2 flip_horizontal(A2 array2)
{
        (void)array2;
        return NULL;
}

// extra
A2 flip_vertical(A2 array2)
{
        (void)array2;
        return NULL;
}

// extra
A2 transpose(A2 array2)
{
        (void)array2;
        return NULL;
}

