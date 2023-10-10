#ifndef TRANSFORMATIONS_INCLUDED
#define TRANSFORMATIONS_INCLUDED

#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

// typedef A2Methods_UArray2 A2; 


A2Methods_UArray2 rotate(Pnm_ppm source_ppm, int degree, A2Methods_T method_type);

#endif