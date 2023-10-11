/**************************************************************
 *
 *                     uarray2b.c
 *
 *      Assignment: locality
 *      Authors: Liam Smith (lsmith26) and Cooper Golemme (cgolem01)
 *      Date: October 10, 2023
 * 
 *      Creation of a UArray2b as a UArray created two dimensionally,
 *      partitioned into blocks
 *
 *
 **************************************************************/

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


void check_and_print(int i, int j, UArray2b_T a, void *p1, void *p2);
void fill_test(int i, int j, UArray2b_T a, void *p1, void *p2);

struct T {
        int height;
        int width;
        int size; 
        int blocksize;
        UArray2_T elems;
};

// int main (int argc, char **argv)
// {
//     (void) argc; (void) argv;

//     int width = 5;
//     int height = 5;
//     int size = sizeof(int);
//     int blocksize = 2;
//     T test_array = UArray2b_new(width, height, size, blocksize);

//     printf("test array width: %i\n", UArray2b_width(test_array));
//     printf("test array height: %i\n", UArray2b_height(test_array));
//     printf("test array size: %i\n", UArray2b_size(test_array));
//     printf("test array blocksize: %i\n", UArray2b_blocksize(test_array));
//     int x = 0;
    
//     UArray2b_map(test_array, fill_test, &x);
//     printf("\nNumber of maps: %d\n", x);
   
//     UArray2b_map(test_array, check_and_print, NULL);

//     int *c = UArray2b_at(test_array, 4, 1);
//     fprintf(stderr, "At: %d\n", *c);
//     (void)c;


//     UArray2b_free(&test_array); 

//     return 0;
// }

 /********** UArray2b_new ********
 *
 * Purpose:
 *      Creates a UArray2b according to blocked logic where each block is
 *      represented as a UArray_T
 * 
 * Inputs: 
 *      int width: an integer representing the horizontal width of a UArray2b
 * 
 *      int height: an integer representing the vertical height of a UArray2b
 * 
 *      int size: an integer representing the size of an element's bytes that
 *      will be stored in each block's cells
 * 
 *      int blocksize: an integer representing the number of cells on one side
 *      of a block
 *	    
 * Return: 
 *      A UArray2b_T as type T, which has been initialized with width, height,
 *      element size, and blocksize properties.
 *
 * Notes: 
 *      asserts?
 * 
 ************************/
T UArray2b_new(int width, int height, int size, int blocksize)
{
        fprintf(stderr, "NEw with block\n");

        assert(width >= 0 && height >= 0); // ??
        T uarray2b;
        NEW(uarray2b);

        uarray2b->height = height;
        uarray2b->width = width;
        uarray2b->size = size;
        uarray2b->blocksize = blocksize;

        int uarray2_dim_width = width / blocksize;
        if (width % blocksize != 0) {
                uarray2_dim_width++;
        }
        int uarray2_dim_height = height / blocksize;
        if (height % blocksize != 0) {
                uarray2_dim_height++;
        }

        int size_of_elem = blocksize * blocksize * size;

        uarray2b->elems = UArray2_new(uarray2_dim_width, 
                            uarray2_dim_height, size_of_elem);

        for (int col = 0; col < uarray2_dim_width; col++) {
                for (int row = 0; row < uarray2_dim_height; row++) {
                        UArray_T *elem = UArray2_at(uarray2b->elems, col, row);
                        *elem = UArray_new(blocksize * blocksize, size);
                }
        }
        
        return uarray2b;
}

/* new blocked 2d array: blocksize as large as possible provided
 * block occupies at most 64KB (if possible)
 */

 /********** UArray2b_new_64K_block ********
 *
 * Purpose:
 *      Maximum allocator of a UArray2b with "default" 64KB blocksize ?
 * 
 * Inputs: 
 *      int width: an integer representing the horizontal width of a UArray2b
 * 
 *      int height: an integer representing the vertical height of a UArray2b
 * 
 *      int size: an integer representing the size of an element's bytes that
 *      will be stored in each block's cells
 *	    
 * Return: 
 *      A UArray2b_T as type T, which has been initialized with width, height,
 *      element size, and blocksize properties.
 *
 * Notes: 
 *      ?
 * 
 ************************/
T UArray2b_new_64K_block(int width, int height, int size)
{
        int kbs = 64 * 1024;
        int bs2  = ceil(sqrt(kbs / size));

        int bs = (size >= kbs) ? bs = 1 : bs2;

        fprintf(stderr, "%d", bs);
        return UArray2b_new(width, height, size, bs);
}

/********** UArray2b_free ********
 *
 * Purpose: Frees a UArray2b and its contained elements
 * 
 * Inputs: 
 *      T array2b: a UArray2b_T with width, height, element size and
 *      blocksize properties as well as a UArray2_T which stores blocked
 *      elements
 *	    
 * Return: 
 *      none (void)
 *
 * Notes: 
 *      asserts that array2b exists before attempting to free
 * 
 ************************/
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

/********** UArray2b_width ********
 *
 * Purpose:
 *      Calculates the horizontal width of a UArray2b
 * 
 * Inputs: 
 *      T array2b: a UArray2b_T with width, height, element size and
 *      blocksize properties as well as a UArray2_T which stores blocked
 *      elements
 *	    
 * Return: 
 *      an integer representing the horizontal width of a UArray2_T
 *
 * Notes: 
 *      asserts that array2b exists before calculating its width
 * 
 ************************/
int UArray2b_width(T array2b)
{
        assert(array2b);
        return array2b->width;
}

/********** UArray2b_height ********
 *
 * Purpose:
 *      Calculates the vertical height of a UArray2b
 * 
 * Inputs: 
 *      T array2b: a UArray2b_T with width, height, element size and
 *      blocksize properties as well as a UArray2_T which stores blocked
 *      elements
 *	    
 * Return: 
 *      an integer representing the vertical height of a UArray2_T
 *
 * Notes: 
 *      asserts that array2b exists before calculating its height
 * 
 ************************/
int UArray2b_height(T array2b)
{
        assert(array2b);
        return array2b->height;
    }

/********** UArray2b_size ********
 *
 * Purpose:
 *      Calculates the size of an element capablle of being
 *      stored in a cell of a block of a UArray2b
 * 
 * Inputs: 
 *      T array2b: a UArray2b_T with width, height, element size and
 *      blocksize properties as well as a UArray2_T which stores blocked
 *      elements
 *	    
 * Return: 
 *      an integer representing the element size being stored
 *
 * Notes: 
 *      asserts that array2b exists before calculating its elements' size
 * 
 ************************/
int UArray2b_size(T array2b)
{
        assert(array2b);
        return array2b->size;
}

/********** UArray2b_blocksize ********
 *
 * Purpose:
 *      Calculates the size of an element capablle of being
 *      stored in a cell of a block of a UArray2b
 * 
 * Inputs: 
 *      T array2b: a UArray2b_T with width, height, element size and
 *      blocksize properties as well as a UArray2_T which stores blocked
 *      elements
 *	    
 * Return: 
 *      an integer representing the number of cells on one side
 *      of a block
 *
 * Notes: 
 *      asserts that array2b exists before calculating its blocksize
 * 
 ************************/
int UArray2b_blocksize(T array2b)
{
        assert(array2b);
        return array2b->blocksize;
}

/********** flattened_index ********
 *
 * Purpose: ?
 * 
 * Inputs: 
 *      int col:
 * 
 *      int row:
 * 
 *      int width:
 *	    
 * Return: 
 *      ?
 *
 * Notes: 
 * 
 ************************/
int flattened_index(int col, int row, int width)
{
        return width * row + col;
}

/* return a pointer to the cell in the given column and row.
 * index out of range is a checked run-time error
 */

/********** Block_at ********
 *
 * Purpose: 
 *      Used during mapping, the function returns the entire block that
 *      exists when considering row and column indeces
 * 
 * Inputs: 
 *      T array2b: a UArray2b_T with width, height, element size and
 *      blocksize properties as well as a UArray2_T which stores blocked
 *      elements
 *      
 *      int column: an integer representing the column of an element within
 *      a UArray2b
 * 
 *      int row: an integer representing the row of an element within
 *      a UArray2b
 *      
 *	    
 * Return:
 *      The block that exists at a given row and column index as a void pointer
 *
 * Notes: 
 *      asserts ?
 * 
 ************************/
void *Block_at(T array2b, int column, int row)
{
        assert(array2b);
        assert(column < array2b->width && row < array2b->height);
        assert(column >= 0 && row >= 0);

        int bs = array2b->blocksize;
        UArray_T *block = UArray2_at(array2b->elems, column / bs, row / bs);

        assert(block);
        return block;
}

/********** UArray2b_at ********
 *
 * Purpose:
 *      Access the element at a given row and column index of a UArray2b
 * 
 * Inputs: 
 *      int column: an integer representing the column of an element within
 *      a UArray2b
 * 
 *      int row: an integer representing the row of an element within
 *      a UArray2b
 *	    
 * Return: 
 *      the element at a given row and column index of a UArray2b as a void
 *      pointer
 * 
 * Notes: 
 *      asserts ?
 * 
 ************************/
void *UArray2b_at(T array2b, int column, int row)
{
        int bs = array2b->blocksize;
        UArray_T *block = Block_at(array2b, column, row);

        int block_col = column % bs;
        int block_row = row % bs;

        int block_index = flattened_index(block_col, block_row, bs);

        void *elem = UArray_at(*block, block_index);

        return elem;
}

/********** UArray2b_map ********
 *
 * Purpose:
 *      Maps accross a UArray2b according to block major logic. This means that
 *      every cell of a block will be visited according to row major logic
 *      before the next block is visited, again, according to row major logic.
 * 
 * Inputs: 
 *      T array2b: a UArray2b_T with width, height, element size and
 *      blocksize properties as well as a UArray2_T which stores blocked
 *      elements
 * 
 *      apply: a function which is applied to every element of the UArray2b
 * 
 *      void *cl: pointer to a variable needed by the void apply 
 *      function pointer.
 *	    
 * Return: 
 *      none (void)
 *
 * Notes: 
 *      throw in some asserts
 * 
 ************************/
extern void  UArray2b_map(T array2b, 
                          void apply(int col, int row, T array2b,
                                     void *elem, void *cl), 
                          void *cl)
{
        int num_blocks_width = UArray2_width(array2b->elems);
        int num_blocks_height = UArray2_height(array2b->elems);
        int blocksize = array2b->blocksize;

        for (int row = 0; row < num_blocks_height; row++) {
                for (int col = 0; col < num_blocks_width; col++) {

                        UArray_T *block = UArray2_at(array2b->elems, col, row);

                        // printf("NEW BLOCK at [col, row] : [%d, %d]\n", col, row);
                        for (int block_col = 0; block_col < blocksize; block_col++) { // change to bCol
                                for (int block_row = 0; block_row < blocksize; block_row++) { // change to bRow
                                    
                                        bool beyond_width = col * blocksize + 
                                            (block_col + 1) > array2b->width;
                                        bool beyond_height = row * blocksize + 
                                            (block_row + 1) > array2b->height;

                                        if (beyond_width || beyond_height) {
                                                continue;

                                        } else {
                                                int uarray_index = flattened_index(block_col, 
                                                                                block_row, 
                                                                                blocksize);
                                                void *elem = UArray_at(*block, uarray_index);
                                                apply(col * blocksize + block_col, row * blocksize + block_row, array2b, elem, cl);
                                        }
                                }
                        }
                }
        }
}


void check_and_print(int col, int row, UArray2b_T a, void *elem, void *cl) 
{
        (void) a; (void) cl;
        printf("ar[%d,%d]: %d\n", col, row, *(int*)elem);
}


void fill_test(int i, int j, UArray2b_T a, void *elem, void *cl) 
{
        (void) a; (void) i; (void) j;
        int *sum = cl;
        *sum = *sum + 1;
        int *int_elem = elem;
        *int_elem = *sum;
}

#undef T