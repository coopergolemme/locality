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
    
    // methods = method_type;


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
    fprintf(stderr, "Writeing to file\n");
    Pnm_ppmwrite(stdout, source);
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
        printf("FILE IS NOT NULL CURRENTLY OPENING\n");
        fprintf(stderr, "FILENAME : %s\n", filename);
        FILE *fp = fopen(filename, mode);
        assert(fp != NULL);
        return fp;
}



