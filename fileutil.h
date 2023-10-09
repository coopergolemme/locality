#ifndef FILEUTIL_INCLUDED
#define FILEUTIL_INCLUDED



#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

typedef A2Methods_UArray2 A2;

Pnm_ppm make_A2(char *filename);
void map_image(int i, int j, A2 a, void *elem, void *image);
A2 read_file(FILE *file);
FILE *open_or_fail(char *filename, char *mode);
void write_A2(A2 transformed, Pnm_ppm source);


#endif
