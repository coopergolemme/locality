/**************************************************************
 *
 *                     fileutil.c
 *
 *      Assignment: locality
 *      Authors: Liam Smith (lsmith26) and Cooper Golemme (cgolem01)
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
 * Notes: 
 *      If file is not provided, input is taken through stdin
 * 
 ************************/
Pnm_ppm make_A2(char *filename, A2Methods_T method_type)
{
        /* if filename is null, read through stdin, otherwise read fp becomes
        a pointer to filename */
        FILE *fp = (filename == NULL) ? stdin : open_or_fail(filename, "rb");

        /* data created after file is read and mapped according to method */
        Pnm_ppm data = Pnm_ppmread(fp, method_type);
        fclose(fp);

        return data;
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
 * Notes: 
 *      Image is written according to Pnm_ppmwrite from pnm.h
 * 
 ************************/
void write_A2(A2 transformed, Pnm_ppm source, A2Methods_T method_type)
{
        /* updating of pixels, width, and height dimensions based
        on transformed properties */
        methods = method_type;
        source->pixels = transformed;
        source->width = methods->width(transformed);
        source->height =  methods->height(transformed);

        /* image written to stdout in source Pnm_ppm */
        Pnm_ppmwrite(stdout, source);
}


/********** write_timing ********
 *
 * Purpose:
 *      Writes timing information based on CPU and rotation information 
 *      to a file
 * 
 * Inputs: 
 * 
 *      char *timing_filename: time filename provided for writing to
 * 
 *      double time: time value printed to output file
 * 
 *      int width: integer representing an image's width
 *
 *      int height: integer representing an image's height
 *	    
 * Return: 
 *      none (void)
 *
 * Notes: 
 *      open_or_fail will assert to ensure that provided file exists and is
 *      not NULL
 *      Makes the file with timing_filename if file does not exist
 * 
 ************************/
void write_timing(char *timing_filename, double time, int width, int height)
{ 
        /* file pointer tf created based on open_or_fail return */
        FILE *tf = open_or_fail(timing_filename, "w");
        fprintf(tf, "Total Time: %f\n", time);
        fprintf(tf, "Time per pixel: %f\n", time / (width * height));
        fclose(tf);
}

/********** open_or_fail ********
 *
 * Purpose: Opens a file containing a ppm, if provided
 * 
 * Inputs: 
 *      char *filename: a string representing image file that A2 will be
 *      created according to
 *
 *      char *mode: a string representing the mode that the file will be
 *      opened/read according to
 *	    
 * Return:
 *      FILE *fp as a pointer to an opened file
 *
 * Notes: 
 *      asserts that a file pointer (fp) is not null after attempting to
 *      open the provided file
 *      if file cannot be opened, exit through EXIT FAILURE
 * 
 ************************/
FILE *open_or_fail(char *filename, char *mode)
{
        FILE *fp = fopen(filename, mode);

        /* checks if file can be opened */
        if (fp == NULL){
                fprintf(stderr, "File: %s could not be opened\n", filename);
                exit(EXIT_FAILURE);
        }
        
        return fp;
}