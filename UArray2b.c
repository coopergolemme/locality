#include "uarray2b.h"
#include <uarray.h>
#include <math.h>
typedef struct T *T{
    int height;
    int width;
    int size; 
    int blocksize;
    UArray2_T elems;
};

T UArray2b_new(int width, int height, int size, int blocksize);
T UArray2b_new_64K_block(int width, int height, int size);
void UArray2b_free(T *array2b);
int UArray2b_width(T array2b);
int UArray2b_height(T array2b);
int UArray2b_size(T array2b);
int UArray2b_blocksize(T array2b);
extern void *UArray2b_at(T array2b, int column, int row);
extern void  UArray2b_map(T array2b, void apply(int col, int row, T array2b,
                                     void *elem, void *cl), void *cl);



// TODO FIX THIS 
T UArray2b_new(int width, int height, int size, int blocksize)
{
    assert(width >= 0 && height >= 0); // ??
    T uarray2b;
    NEW(uarray2b);
    uarray2b = {
        height;
        width;
        size;
        blocksize;
    };
    int x = ceil(sqrt(blocksize));
    uarray2b->elems = UArray2_new(x, x, size);
    for (int i = 0; i < UArray_length(elems); i++) {
        for (int j = 0; j < UArray_length(elems); j++) {
            UArray2_T elem = UArray_at(uarray2b->elems, j, i);
            elem = UArray_new(x, size);
        }
    }
    return uarray2b;
}

/* new blocked 2d array: blocksize as large as possible provided
 * block occupies at most 64KB (if possible)
 */
T UArray2b_new_64K_block(int width, int height, int size)
{
   return UArray2b_new(width, height, size, 64000);
}

void UArray2b_free(T *array2b)
{
    assert(array2b);
}

int UArray2b_width(T array2b)
{
    assert(array2b);
    return array2b->width;

}
int UArray2b_height(T array2b)
{
    assert(array2b);
    return array2b->height;

}
int UArray2b_size(T array2b)
{
    assert(array2b);
    return array2b->size;

}
int UArray2b_blocksize(T array2b)
{
    assert(array2b);
    return array2b->blocksize;

}

/* return a pointer to the cell in the given column and row.
 * index out of range is a checked run-time error
 */
extern void *UArray2b_at(T array2b, int column, int row)
{
    assert(array2b);
    assert(column < array2b->width && row < array2b->height);
    assert(column >= 0 && row >= 0);
    
    UArray2_T temp = UArray2_at(array2b, column / array2b->blocksize, 
                                row / array2b->blocksize);
                                
    return UArray_at(temp,
                     column % array2b->blocksize + row % array2b->blocksize);
}


/* visits every cell in one block before moving to another block */
extern void  UArray2b_map(T array2b, 
                          void apply(int col, int row, T array2b,
                                     void *elem, void *cl), 
                          void *cl)
{
    assert(array2b && apply);
    for (int col = 0; col < array2b->width; col++) {
        for (int row = 0; row < array2b->height; row++) {
            T elem = UArray2b_at(array2b, col, row);
            for (int k = 0; k < blocksize; k++) {
                apply(col, row, array2b, UArray_at(elem, k), cl);
            }
        }
    }
}