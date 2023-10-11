/**************************************************************
 *
 *                     fileutil.c
 *
 *     Assignment: iii
 *     Authors: Liam Smith (lsmith26) and Cooper Golemme (UTLN)
 *     Date: October 10, 2023
 * 
 *      Purpose
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
 * Purpose:
 * 
 * Inputs: 
 *	    
 * Return: 
 *
 * Notes: 
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
 * Purpose:
 * 
 * Inputs: 
 *	    
 * Return: 
 *
 * Notes: 
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
 * Purpose:
 * 
 * Inputs: 
 *	    
 * Return: 
 *
 * Notes: 
 * 
 ************************/
FILE *open_or_fail(char *filename, char *mode)
{
        FILE *fp = fopen(filename, mode);
        assert(fp != NULL);
        return fp;
}



