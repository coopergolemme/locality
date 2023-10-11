#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mem.h>

#include "assert.h"
#include "fileutil.h"

static A2Methods_T methods;



Pnm_ppm make_A2(char *filename, A2Methods_T method_type)
{
    FILE *fp = (filename == NULL) ? stdin : open_or_fail(filename, "rb");
    Pnm_ppm data = Pnm_ppmread(fp, method_type);
    
    fclose(fp);

    return data;
}

void write_A2(A2 transformed, Pnm_ppm source, A2Methods_T method_type)
{
    methods = method_type;

    source->pixels = transformed;
    source->width = methods->width(transformed);
    source->height =  methods->height(transformed);

    Pnm_ppmwrite(stdout, source);
}

void write_timing(char *timing_filename, double time, int width, int height) {
    (void)width;
    (void)height;
    FILE *tf = open_or_fail(timing_filename, "w");
    fprintf(tf, "%f", time);
}

/********** open_or_fail ********
 *      opens file or aborts program
 * 
 * Inputs: a cstring filename, and the mode to open the file with
 *
 * Returns a pointer to the open file
 *
 * Notes: c.r.e if filename could not be opened.
 *
 ************************/
FILE *open_or_fail(char *filename, char *mode)
{
        FILE *fp = fopen(filename, mode);
        assert(fp != NULL);
        return fp;
}



