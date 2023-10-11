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
#include <time.h>

#include "assert.h"
#include "fileutil.h"
#include "transformations.h"
#include "cputiming.h"
#include "cputiming_impl.h"



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

Dimensions open_and_rotate(char *filename, int rotation, A2Methods_T method_type);
void timer_write(char *timing_file, double time);
void open_and_rotate_timed(char *image_filename, int rotation, A2Methods_T method_type, char *timing_filename);

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
        // int ir;

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
                        fprintf(stderr, "blockmajor\n");
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
        
        input_filename = argv[i];
        
        if(time_file_name == NULL) {
                open_and_rotate(input_filename, rotation, methods);
        }

        else {
                open_and_rotate_timed(input_filename,
                                      rotation, 
                                      methods, 
                                      time_file_name);
        }
}

void timer_write(char *timing_file, double time){
        (void)timing_file;
        (void)time;
        return;
}

void open_and_rotate_timed(char *image_filename, 
                           int rotation,
                           A2Methods_T method_type,
                           char *timing_filename)
{
        CPUTime_T timer = CPUTime_New();
        CPUTime_Start(timer);

        Dimensions dims = open_and_rotate(image_filename, rotation, method_type);

        double time = CPUTime_Stop(timer);
        write_timing(timing_filename, time, dims.width, dims.height);

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
 *      none (void)
 *
 * Notes: What will happen if no input file is provided? Not called, right?
 *      Just want to confirm
 * 
 ************************/
Dimensions open_and_rotate(char *filename, int rotation, A2Methods_T method_type)
{
        A2Methods_T methods = method_type;

        Pnm_ppm source_ppm = make_A2(filename, methods);
        
        A2 source_pix = source_ppm->pixels;

        Dimensions dims;
        dims.height = methods->height(source_pix);
        dims.width = methods->width(source_pix);

        A2 transformed = rotate(source_ppm, rotation, methods);
        write_A2(transformed, source_ppm, methods);
        
        if (rotation != 0) methods->free(&source_pix);
        Pnm_ppmfree(&source_ppm);
        return dims;
}
