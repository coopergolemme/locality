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

// TODO FIX THIS 
T UArray2b_new(int width, int height, int size, int blocksize)
{
    assert(width >= 0 && height >= 0);
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
    for (int i = 0; i < UArray_length(elems); i++){
        for (int i = 0; i < UArray_length(elems); i++){
            UArray2_T uarray2 = UArray_at(uarray2b->elems);
            uarray2 = UArray2_new(x, x, size);
        }
    }
    return uarray2b;
}

/* new blocked 2d array: blocksize as large as possible provided
 * block occupies at most 64KB (if possible)
 */
T UArray2b_new_64K_block(int width, int height, int size)
{
    assert()

}

void UArray2b_free(T *array2b)
{
    assert(array2b);
    

}

int UArray2b_widtht(T  array2b)
{
    assert(array2b);

}
int UArray2b_height(T  array2b)
{
    assert(array2b);

}
int UArray2b_size(T  array2b)
{
    assert(array2b);

}
int UArray2b_blocksize(T  array2b)
{
    assert(array2b);

}

/* return a pointer to the cell in the given column and row.
 * index out of range is a checked run-time error
 */
extern void *UArray2b_at(T array2b, int column, int row)
{
    assert(array2b);

}

/* visits every cell in one block before moving to another block */
extern void  UArray2b_map(T array2b, 
                          void apply(int col, int row, T array2b,
                                     void *elem, void *cl), 
                          void *cl)
{
    assert(array2b);
}