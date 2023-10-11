/**************************************************************
 *
 *                     transformations.c
 *
 *      Assignment: locality
 *      Authors: Liam Smith (lsmith26) and Cooper Golemme (UTLN)
 *      Date: October 10, 2023
 * 
 *      Transformation functions used to rotate an image and map
 *      accordingly
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

// should move to .h file?
void apply_rotate90(int i, int j, A2 array2, void *source, void *dest);
void apply_rotate180(int i, int j, A2 array2, void *source, void *dest);
void apply_rotate270(int i, int j, A2 array2, void *source, void *dest);

/********** rotate ********
 *
 * Purpose:
 *      Rotates source image based on degree provided as input, and maps
 *      rotated image according to default based on method type
 * 
 * Inputs: 
 *      Pnm_ppm source_ppm: Original "holder" of image used that the transformed
 *      image is written to, which is a Pnm_ppm that contains a struct containg 
 *      width and height dimensions of an A2 called pixels that holds the 
 *      ppm image.
 * 
 *      int degree: an integer representing the degree of rotation that
 *      the image will be transformed according to
 * 
 *      A2Methods_T method_type: method type (either plain or blocked) that
 *      will be used to call applicable functions
 *      
 *	    
 * Return: 
 *      An A2 image of pixels that correspond to a rotated image
 *      
 * Notes: 
 *      do some asserts
 * 
 ************************/
A2 rotate(Pnm_ppm source_ppm, int degree, A2Methods_T method_type)
{
    methods = method_type; 

    
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

/********** apply_rotate90 ********
 *
 * Purpose:
 *      Mapping function for 90 degree counter clockwise rotations
 * 
 * Inputs: 
 *      int i:
 * 
 *      int j: ?
 * 
 *      A2 array2: A2 that contains an image
 * 
 *      void *source: void pointer to source image
 * 
 *      void *dest: void pointer to destination image
 *	    
 * Return: 
 *
 * Notes: 
 *      change variable names?
 * 
 ************************/
void apply_rotate90(int i, int j, A2 array2, void *source, void *dest)
{
        (void) array2;
        Pnm_rgb dest_unupdated = methods->at(dest, 
                                        methods->width(dest) - j - 1, i);
        *dest_unupdated = *(Pnm_rgb)source;
}

/********** apply_rotate180 ********
 *
 * Purpose:
 *      Mapping function for 180 degree counter clockwise rotations
 * 
 * Inputs: 
 *      int i:
 * 
 *      int j: ?
 * 
 *      A2 array2: A2 that contains an image
 * 
 *      void *source: void pointer to source image
 * 
 *      void *dest: void pointer to destination image
 *	    
 * Return: 
 *
 * Notes: 
 *      change variable names?
 * 
 ************************/
void apply_rotate180(int i, int j, A2 array2, void *source, void *dest)
{

        (void) array2;
        Pnm_rgb dest_unupdated = methods->at(dest, methods->width(dest) - i - 1,
                                        methods->height(dest) - j - 1);
        *dest_unupdated = *(Pnm_rgb)source;
}

/********** apply_rotate270 ********
 *
 * Purpose:
 *      Mapping function for 270 degree counter clockwise rotations
 * 
 * Inputs: 
 *      int i:
 * 
 *      int j: ?
 * 
 *      A2 array2: A2 that contains an image
 * 
 *      void *source: void pointer to source image
 * 
 *      void *dest: void pointer to destination image
 *	    
 * Return: 
 *
 * Notes: 
 *      change variable names?
 * 
 ************************/
void apply_rotate270(int i, int j, A2 array2, void *source, void *dest)
{
        (void) array2;
        Pnm_rgb dest_unupdated = methods->at(dest, i, 
                                        methods->width(dest) - j - 1);
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

