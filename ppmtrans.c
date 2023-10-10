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

void open_and_rotate(char *filename, int rotation, A2Methods_T method_type);

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

        printf("hello?\n");
        for (i = 1; i < argc; i++) {
                printf("in for?\n");
                printf("in for?\n");
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
        input_filename = (time_file_name == NULL) ? argv[i] : argv[i - 2];
        open_and_rotate(input_filename, rotation, methods);
}

void open_and_rotate(char *filename, int rotation, A2Methods_T method_type)
{
        // methods = uarray2_methods_plain;
        A2Methods_T methods = method_type;
        // (void) method_type;
        // A2Methods_T methods = uarray2_methods_blocked;

        Pnm_ppm source_ppm = make_A2(filename, methods);
        A2 source_pix = source_ppm->pixels;
        A2 transformed = rotate(source_ppm, rotation, methods);
        write_A2(transformed, source_ppm, methods);
        
        if (rotation != 0) methods->free(&source_pix);
        Pnm_ppmfree(&source_ppm);
}
