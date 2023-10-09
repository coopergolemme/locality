#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mem.h>

#include "assert.h"
#include "fileutil.h"

static A2Methods_T methods;

// static void check_and_print(int col, int row, A2 a, void *elem, void *cl) 
// {
//     (void) a; (void) cl;
//     printf("ar[%d,%d]: %d\n", col, row, *(int*)elem);
// }

Pnm_ppm make_A2(char *filename)
{
    methods = uarray2_methods_plain; 

    if (filename == NULL){
        printf("File is null\n");
    }
    FILE *fp = (filename == NULL) ? stdin : open_or_fail(filename, "rb");
    Pnm_ppm data = Pnm_ppmread(fp, methods);
    fclose(fp);

    return data;
}

void write_A2(A2 transformed, Pnm_ppm source){
    // A2 *orig_pix = *(A2 *)source->pixels;
    source->pixels = transformed;
    source->width = methods->width(transformed);
    source->height =  methods->height(transformed);
    FILE *fp = open_or_fail("image.ppm", "w");
    Pnm_ppmwrite(fp, source);
    fclose(fp);
    // methods->free(orig_pix);
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
        printf("FILENAME : %s\n", filename);
        FILE *fp = fopen(filename, mode);
        assert(fp != NULL);
        return fp;
}



