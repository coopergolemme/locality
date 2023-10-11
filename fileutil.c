/**************************************************************
 *
 *                     fileutil.c
 *
 *      Assignment: locality
 *      Authors: Liam Smith (lsmith26) and Cooper Golemme (UTLN)
 *      Date: October 10, 2023
 * 
 *      Creates an A2 based on plain or blocked method type through reading,
 *      and writes transformed image to stdout.
 *
 *
 **************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mem.h>

#include "assert.h"
#include "fileutil.h"

static A2Methods_T methods;


/********** make_A2 ********
 *
 * Purpose: Creates an A2 image that is mapped/created according to plain
 * or blocked methods. A file is taken as input that contains data of type
 * Pnm_ppm.
 * 
 * Inputs: 
 *      char *filename: a string representing image file that A2 will be
 *      created according to
 * 
 *      A2Methods_T method_type: method type (either plain or blocked) that
 *      will be used to call on applicable functions
 * 
 * Return: 
 *      A Pnm_ppm that contains a struct containg width and height
 *      dimensions of an A2 called pixels that holds the ppm image
 *
 * Notes: ?
 * 
 ************************/
Pnm_ppm make_A2(char *filename, A2Methods_T method_type)
{
<<<<<<< HEAD
    FILE *fp = (filename == NULL) ? stdin : open_or_fail(filename, "rb");
    Pnm_ppm data = Pnm_ppmread(fp, method_type);
    
    fclose(fp);

    return data;
=======
    
        // methods = method_type;
        FILE *fp = (filename == NULL) ? stdin : open_or_fail(filename, "rb");
        Pnm_ppm data = Pnm_ppmread(fp, method_type);
        fclose(fp);

        return data;
>>>>>>> 401832b (comments)
}

/********** write_A2 ********
 *
 * Purpose: To stdout, a transformed A2 image is written based on updated
 * width and height dimensions
 * 
 * Inputs: 
 * 
 *      A2 transformed: A transformed A2 with updated pixel layout, width, and
 *      height
 * 
 *      Pnm_ppm source: Original "holder" of image used that the transformed
 *      image is written to, which is a Pnm_ppm that contains a struct containg 
 *      width and height dimensions of an A2 called pixels that holds the 
 *      ppm image.
 * 
 *      A2Methods_T method_type: method type (either plain or blocked) that
 *      will be used to call on applicable functions
 *	    
 * Return: none (void)
 *
 * Notes: ?
 * 
 ************************/
void write_A2(A2 transformed, Pnm_ppm source, A2Methods_T method_type)
{
<<<<<<< HEAD
    methods = method_type;

    source->pixels = transformed;
    source->width = methods->width(transformed);
    source->height =  methods->height(transformed);

    Pnm_ppmwrite(stdout, source);
=======
        methods = method_type;
        source->pixels = transformed;
        source->width = methods->width(transformed);
        source->height =  methods->height(transformed);
        fprintf(stderr, "Writeing to file\n");
        Pnm_ppmwrite(stdout, source);
>>>>>>> 401832b (comments)
}

void write_timing(char *timing_filename, double time, int width, int height) {
    (void)width;
    (void)height;
    FILE *tf = open_or_fail(timing_filename, "w");
    fprintf(tf, "%f", time);
}

/********** open_or_fail ********
 *
 * Purpose: Opens a file containing a ppm, if provided
 * 
 * Inputs: 
 *      char *filename: a string representing image file that A2 will be
 *      created according to
 *      char *mode: a string representing the mode that the file will be
 *      opened/read according to
 *	    
 * Return:
 *      FILE *fp as a pointer to an opened file
 *
 * Notes: 
 *      asserts that a file pointer (fp) is not null after attempting to
 *      open the provided file
 * 
 ************************/
FILE *open_or_fail(char *filename, char *mode)
{
        FILE *fp = fopen(filename, mode);
        assert(fp != NULL);
        return fp;
}



