/**************************************************************
 *
 *                     transformations.c
 *
 *      Assignment: locality
 *      Authors: Liam Smith (lsmith26) and Cooper Golemme (cgolem01)
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


void apply_rotate90(int col, int row, A2 array2, void *source, void *dest);
void apply_rotate180(int col, int row, A2 array2, void *source, void *dest);

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
 *      An A2 image of pixels that correspond to a rotated image, or 
 *      NULL if degree input is invalid
 *      
 * Notes: 
 *      asserts that source_ppm exists
 * 
 ************************/
A2 rotate(Pnm_ppm source_ppm, int degree, A2Methods_T method_type)
{
        /* asserts the existance of source_ppm */
        assert(source_ppm);

        /* methods gets set to method_type as plain or blocked */
        methods = method_type; 

        /* source created based on source_ppm pixels with width, height
        and element size dimensions */
        A2 source = source_ppm->pixels;
        int source_width = methods->width(source);
        int source_height = methods->height(source);
        int pixel_size = methods->size(source_ppm->pixels);

        /* declaration of destination A2 that file will be written to */
        A2 dest;
    
        /* destination created based on degree with applicable mapping
        function called */
        if (degree == 90) {
                dest = methods->new(source_height, source_width, pixel_size);
                methods->map_default(source, apply_rotate90, dest);

        } else if (degree == 180) {
                dest = methods->new(source_width, source_height, pixel_size);
                methods->map_default(source, apply_rotate180, dest);

        } else if (degree == 0) {
                return source;
            
        } else {
                return NULL; 
        }

        /* destination image returned */
        return dest;
}

/********** apply_rotate90 ********
 *
 * Purpose:
 *      Mapping function for 90 degree counter clockwise rotations
 * 
 * Inputs: 
 *      int col: integer representing the column index
 * 
 *      int row: integer representing the row index
 * 
 *      A2 array2: A2 that contains an image
 * 
 *      void *source: void pointer to source image
 * 
 *      void *dest: void pointer to destination image
 *	    
 * Return: 
 *      none (void)
 *
 * Notes: 
 *      assert that dest_unupdated returns value from at function
 * 
 ************************/
void apply_rotate90(int col, int row, A2 array2, void *source, void *dest)
{
        (void) array2;

        /* dest_unupdated is a pixel coming from a source image */
        Pnm_rgb dest_unupdated = methods->at(dest, 
                                             methods->width(dest) - row - 1, 
                                             col);
        assert(dest_unupdated);

        /* dest_unupdated gets becomes the source pixel location */
        *dest_unupdated = *(Pnm_rgb)source;
}

/********** apply_rotate180 ********
 *
 * Purpose:
 *      Mapping function for 180 degree counter clockwise rotations
 * 
 * Inputs: 
 *      int col: integer representing the column index
 * 
 *      int row: integer representing the row index
 * 
 *      A2 array2: A2 that contains an image
 * 
 *      void *source: void pointer to source image
 * 
 *      void *dest: void pointer to destination image
 *	    
 * Return: 
 *      none (void)
 *
 * Notes: 
 *      assert that dest_unupdated returns value from at function
 * 
 ************************/
void apply_rotate180(int col, int row, A2 array2, void *source, void *dest)
{

        (void) array2;

        /* dest_unupdated is a pixel coming from a source image */
        Pnm_rgb dest_unupdated = methods->at(dest, 
                                             methods->width(dest) - col - 1,
                                             methods->height(dest) - row - 1);
        assert(dest_unupdated);

        /* dest_unupdated gets becomes the source pixel location */
        *dest_unupdated = *(Pnm_rgb)source;
}