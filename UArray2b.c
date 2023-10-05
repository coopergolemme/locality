#include "uarray2b.h"
#include "uarray2.h"
#include <uarray.h>
#include <math.h>
#include "assert.h"
#include "mem.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define T UArray2b_T

struct T {
    int height;
    int width;
    int size; 
    int blocksize;
    UArray2_T elems;
};



void check_and_print(int i, int j, UArray2b_T a, void *p1, void *p2);
void fill_test(int i, int j, UArray2b_T a, void *p1, void *p2);

int main (int argc, char **argv)
{
    (void) argc; (void) argv;

    int width = 5;
    int height = 5;
    int size = sizeof(int);
    int blocksize = 2;
    T test_array = UArray2b_new(width, height, size, blocksize);

    printf("test array width: %i\n", UArray2b_width(test_array));
    printf("test array height: %i\n", UArray2b_height(test_array));
    printf("test array size: %i\n", UArray2b_size(test_array));
    printf("test array blocksize: %i\n", UArray2b_blocksize(test_array));
    int x = 0;
    UArray2b_map(test_array, fill_test, &x);
    printf("\nNumber of maps: %d\n", x);
    UArray2b_map(test_array, check_and_print, NULL);

    

    UArray2b_free(&test_array); 



    return 0;
}

T UArray2b_new(int width, int height, int size, int blocksize)
{
    assert(width >= 0 && height >= 0); // ??
    T uarray2b;
    NEW(uarray2b);

    uarray2b->height = height;
    uarray2b->width = width;
    uarray2b->size = size;
    uarray2b->blocksize = blocksize;

    int uarray2_dim_width = width / blocksize;
    if (width %blocksize != 0){
        uarray2_dim_width++;
    }
    int uarray2_dim_height = height / blocksize;
    if (height % blocksize != 0){
        uarray2_dim_height++;
    }

    int size_of_elem = blocksize * blocksize * size;

    uarray2b->elems = UArray2_new(uarray2_dim_width, uarray2_dim_height, size_of_elem);

    for (int i = 0; i < uarray2_dim_height; i++) {
        for (int j = 0; j < uarray2_dim_width; j++) {
            UArray_T *elem = UArray2_at(uarray2b->elems, j, i);
            *elem = UArray_new(blocksize * blocksize, size);
        }
    }
    printf(" block-width: %d\n", uarray2_dim_width);
    printf(" block-height: %d\n", uarray2_dim_height);
    
    return uarray2b;
}

/* new blocked 2d array: blocksize as large as possible provided
 * block occupies at most 64KB (if possible)
 */
T UArray2b_new_64K_block(int width, int height, int size)
{
    int x = ceil(sqrt(64 * 1024));
    return UArray2b_new(width, height, size, x);
}

void UArray2b_free(T *array2b)
{
    assert(array2b);
    for (int i = 0; i < UArray2_height((*array2b)->elems); i++) {
        for (int j = 0; j < UArray2_width((*array2b)->elems); j++) {
            UArray_free(UArray2_at((*array2b)->elems, j, i));
        }
    }
    UArray2_free(&((*array2b)->elems));
    FREE(*array2b);
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

int flattened_index(int col, int row, int width){
        return width * row + col;
}




/* return a pointer to the cell in the given column and row.
 * index out of range is a checked run-time error
 */
extern void *UArray2b_at(T array2b, int column, int row)
{
    assert(array2b);
    assert(column < array2b->width && row < array2b->height);
    assert(column >= 0 && row >= 0);
    int bs = array2b->blocksize;
    printf("\n%d\n", column / bs);
    printf("\n%d\n", row / bs);

    printf("Width boxes: %d\n", UArray2_width(array2b->elems));
    printf("Height boxes: %d\n", UArray2_height(array2b->elems));
    UArray_T *block = UArray2_at(array2b->elems, column / bs, row / bs);

    assert(block);

    int block_col = column % bs;
    int block_row = row % bs;
    printf("Inside At: ");

    int block_index = flattened_index(block_col, block_row, bs);

    printf("%d\n", block_index);


    return UArray_at(*block, block_index);
}

// void iterate_block(int acc_col, int acc_row, UArray_T array, int blocksize, UArray2b array2b, void apply(int col, int row, T array2b,
//                                      void *elem, void *cl), void *cl){
//     int col_end = acc_col % blocksize;
//     if( acc_col % blocksize !=0= 0){
//         col_end = blocksize;
//     }
//     int row_end = acc_col % blocksize;
//     if( acc_row % blocksize !=0= 0){
//         row_end = blocksize;
//     }
//     for (int col = 0; col < col_end; col++){
//         for (int row = 0; row < row_end; row++){
//             void *elem = UArray_at(array, flattened_index(col, row, blocksize));
//             apply(col, row, array2b, inner, cl);
//         }
//     }

// }

/* visits every cell in one block before moving to another block */
extern void  UArray2b_map(T array2b, 
                          void apply(int col, int row, T array2b,
                                     void *elem, void *cl), 
                          void *cl)
{
    assert(array2b && apply);
    int c = 1;
    for (int col = 0; col < array2b->width; col ++){
        for (int row = 0; row < array2b->height; row++){
            printf("%d\n", c);
            c++;
            if (c == 18){
                printf("col : %d ,, row: %d\n", col, row);
            }
            void *elem = UArray2b_at(array2b, col, row);
            apply(col, row, array2b, elem, cl);
        }
    }    

    
    // int acc_width = array2b->width / array2b->blocksize;
    // if( acc_width % array2b->blocksize != 0){
    //     acc_width++;
    // }
    // int acc_height = array2b->height / array2b->blocksize;
    //  if( acc_height % array2b->blocksize != 0){
    //     acc_height++;
    // }
    // int c = 0;
    // for (int col = 0; col < acc_width; col++) {
    //     for (int row = 0; row < acc_height; row++) {
    //         c++;
    //         printf("OUTER LOOP: %d\n", c);
    //         printf("col of block: %d\n", col);
    //         printf("row of block: %d\n", row);

    //         int height = UArray2_height(array2b->elems);
    //         int width = UArray2_width(array2b->elems);

    //         printf("Height: %d\n", height);
    //         printf("Width: %d\n", width);



    //         UArray_T *elem = UArray2_at(array2b->elems, col, row);

    //         assert(elem);
    //         printf("len: %d\n", UArray_length(*elem));
    //         for (int k = 0; k < UArray_length(*elem); k++){


    //             int block_col = (col * array2b->blocksize) % array2b->blocksize;
    //             int block_row = (row * array2b->blocksize) % array2b->blocksize;

    //             printf("block_col: %d\n", block_col);
    //             printf("block_row: %d\n", block_row);


    //             // 

    //             int flat = flattened_index(block_col, block_row,
    //                                             array2b->blocksize);
    //             flat = flat + k;
    //             printf("flat: %d\n", flat);
    //             void *inner = UArray_at(
    //                 *elem, flat);
    //             apply(col, row, array2b, inner, cl);


    //         }
            
    //     }
    // }
}


void check_and_print(int i, int j, UArray2b_T a, void *elem, void *cl) 
{
    (void) a; (void) cl;
 
    
    printf("ar[%d,%d]: %d\n", i, j, *(int*)elem);
}

void fill_test(int i, int j, UArray2b_T a, void *elem, void *cl) 
{
    (void) a; (void) i; (void) j;
    int *sum = cl;
    *sum = *sum + 1;
    int *int_elem = elem;
    *int_elem = *sum;
    printf("ar[%d,%d]: %d\n", i, j, *(int*)elem);
}

#undef T