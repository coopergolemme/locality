/**************************************************************
 *
 *                     ppmtrans.c
 *
 *      Assignment: locality
 *      Authors: Liam Smith (lsmith26) and Cooper Golemme (cgolem01)
 *      Date: October 10, 2023
 * 
 *      ppmtrans is the core of the program that takes user input to determine
 *      transformation and mapping methods of an image to be transformed
 *
 *
 **************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "assert.h"
#include "fileutil.h"
#include "transformations.h"
#include "cputiming.h"



#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (false)

typedef struct Dimensions{
        int width;
        int height;
} Dimensions;

Dimensions open_and_rotate(char *filename, int rotation, 
                        A2Methods_T method_type);
void open_and_rotate_timed(char *image_filename, int rotation, 
                        A2Methods_T method_type, char *timing_filename);

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}


int main(int argc, char *argv[]) 
{
        char *time_file_name = NULL;
        (void) time_file_name;
        int   rotation       = 0;
        char *input_filename;
        int   i;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);
        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                                    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                                    "column-major");

                        methods->map_default = methods->map_col_major;
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");

                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }

                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                                        "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }

                } else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                } else if (strcmp(argv[i], "-transpose") == 0) {
                                fprintf(stderr,
                                        "Not implemented: -transpose\n");
                                return EXIT_FAILURE;
                } else if (strcmp(argv[i], "-flip") == 0) {
                                fprintf(stderr,
                                        "Not implemented: -flip\n");
                                return EXIT_FAILURE;
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                                argv[i]);
                        usage(argv[0]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }
        if (rotation == 270) {
                fprintf(stderr, "Not implemented: -rotate 270\n");
                return EXIT_FAILURE;
        }

        /* input filename is set to command line value at ith index */
        input_filename = argv[i];
        

        /* if timing file not provided, regular open and rotate called*/
        if (time_file_name == NULL) {
                open_and_rotate(input_filename, rotation, methods);
        }

        /* timing file provided */
        else {
                open_and_rotate_timed(input_filename,
                                      rotation, 
                                      methods, 
                                      time_file_name);
        }
        return EXIT_SUCCESS;

}

/********** open_and_rotate_timed ********
 *
 *      Opens and rotates an image and prints CPU and rotation information
 *      to timing file
 * 
 * Inputs: 
 *      char *image_filename: a string representing image file that A2 will be
 *      created according to
 * 
 *      int rotation: an integer representing the degree of rotation that
 *      the image will be transformed according to
 * 
 *      A2Methods_T method_type: method type (either plain or blocked) that
 *      will be used to call applicable functions

 *      char *image_filename: a string representing image file that timing
 *      information will be written to
 *          
 * Return: 
 *      none (void)
 *
 * Notes: 
 *      calls functions from CPUTime file
 * 
 ************************/
void open_and_rotate_timed(char *image_filename, 
                           int rotation,
                           A2Methods_T method_type,
                           char *timing_filename)
{
        /* creation of timer */
        CPUTime_T timer = CPUTime_New();
        CPUTime_Start(timer);

        /* Dimension struct created with width and height dimeensions */
        Dimensions dims = open_and_rotate(image_filename, 
                                                rotation, method_type);

        /* time calculated when timer stops, written to file */
        double time = CPUTime_Stop(timer);
        write_timing(timing_filename, time, dims.width, dims.height);

        /* freeing the timer instance */
        CPUTime_Free(&timer);
}


/********** open_and_rotate ********
 *
 * Purpose: If file containing image is provided, a rotation of provided
 * degress is performed, mapped according to row, column, or blocked methods.
 * 
 * 
 * Inputs: 
 *      char *filename: a string representing image file that A2 will be
 *      created according to
 * 
 *      int rotation: an integer representing the degree of rotation that
 *      the image will be transformed according to
 * 
 *      A2Methods_T method_type: method type (either plain or blocked) that
 *      will be used to call applicable functions
 *          
 * Return: 
 *      Dimension struct with width and height elements
 *
 * Notes:
 *      Asserts that source_ppm exists before operations are called on it
 * 
 ************************/
Dimensions open_and_rotate(char *filename, int rotation, 
                                        A2Methods_T method_type)
{
        /* methods received method_type */
        A2Methods_T methods = method_type;

        /* source_ppm created based on file and mapped according to
        method type */
        Pnm_ppm source_ppm = make_A2(filename, methods);
        assert(source_ppm);
        
        /* source_pix created as an A2 based on pixels */
        A2 source_pix = source_ppm->pixels;

        /* dims created as a struct containng width and height dimensions */
        Dimensions dims;
        dims.height = methods->height(source_pix);
        dims.width = methods->width(source_pix);

        /* transformed A2 is rotated according to degree */
        A2 transformed = rotate(source_ppm, rotation, methods);
        write_A2(transformed, source_ppm, methods);
        
        /* if default or 0 rotation, source_pix is freed */
        if (rotation != 0) methods->free(&source_pix);

        /* if rotation not zero, source_ppm freed */
        Pnm_ppmfree(&source_ppm);
        return dims;
}
