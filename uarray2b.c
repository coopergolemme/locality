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

struct T {
        int height;
        int width;
        int size; 
        int blocksize;
        UArray2_T elems;
};

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
 *      asserts that width, height, size, and blocksize are valid
 *      NEW inherintely asserts the creation of uarray2b
 * 
 ************************/
T UArray2b_new(int width, int height, int size, int blocksize)
{
        /* asserts proper dimensions for creation of UArray */
        assert(width >= 0 && height >= 0 && size >= 0 && blocksize >= 0);

        /* uarray2b created as type UArray2b_T */
        T uarray2b;
        NEW(uarray2b);

        /* updating uarray2b dimensions */
        uarray2b->height = height;
        uarray2b->width = width;
        uarray2b->size = size;
        uarray2b->blocksize = blocksize;

        /* calculation of UArray2 dimensions */
        int uarray2_dim_width = width / blocksize;
        if (width % blocksize != 0) {
                uarray2_dim_width++;
        }
        int uarray2_dim_height = height / blocksize;
        if (height % blocksize != 0) {
                uarray2_dim_height++;
        }

        /* size of element calculation */
        int size_of_elem = blocksize * blocksize * size;

        /* creation of elems as UArray2 based on calculated dimensions */
        uarray2b->elems = UArray2_new(uarray2_dim_width, 
                            uarray2_dim_height, size_of_elem);

        /* populating each UArray as a block */
        for (int col = 0; col < uarray2_dim_width; col++) {
                for (int row = 0; row < uarray2_dim_height; row++) {
                        UArray_T *elem = UArray2_at(uarray2b->elems, col, row);
                        *elem = UArray_new(blocksize * blocksize, size);
                }
        }
        
        return uarray2b;
}

 /********** UArray2b_new_64K_block ********
 *
 * Purpose:
 *      Maximum allocator of a UArray2b with "default" 64KB blocksize
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
 *      Default blocksize set to 1 if element size is greater than or equal
 *      to 64KB
 * 
 ************************/
T UArray2b_new_64K_block(int width, int height, int size)
{
        /* kbs represented as 64KB */
        int kbs = 64 * 1024;

        /* alt_bs non-1 blocksize value */
        int alt_bs  = ceil(sqrt(kbs / size));

        /* blocksize becomes 1 if size is greater than 64KB */
        int bs = (size >= kbs) ? bs = 1 : alt_bs;

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

        /* frees every UArray (block) in the UArray2 */
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
 * Purpose: 
 *          Converts a 2d index pair (col, row) to a one dimentional index
 *          in a one dimensional array
 * 
 * Inputs: 
 *      int col: column of desired index
 * 
 *      int row: row of desired index
 * 
 *      int width: width of the 2d array
 *	    
 * Return: 
 *      int for index of the element in the 1d array that corresponds to
 *      the index of the element in a corresponding 2d array
 *
 * 
 ************************/
int flattened_index(int col, int row, int width)
{
        return width * row + col;
}

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
 *      asserts that array2b and block UArray exist before casting and returning
 *      asserts that columna and row dimensions are valid based on width and
 *      height
 * 
 ************************/
void *Block_at(T array2b, int column, int row)
{
        assert(array2b);
        assert(column < array2b->width && row < array2b->height);
        assert(column >= 0 && row >= 0);

        /* creation of block with applicable blocksize */
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
 *      asserts that block and elem exist before casting and returning
 *      block inherintely asserts based on row and column dimensions
 * 
 ************************/
void *UArray2b_at(T array2b, int column, int row)
{
        /* creation of block returned from Block_at function */
        assert(array2b);
        int bs = array2b->blocksize;
        UArray_T *block = Block_at(array2b, column, row);
        assert(block);

        /* calculation of block col and row values */
        int block_col = column % bs;
        int block_row = row % bs;

        /* block index as "flattened" from 1D array */
        int block_index = flattened_index(block_col, block_row, bs);

        /* element accessed from block's flattened index */
        void *elem = UArray_at(*block, block_index);
        assert(elem);

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
 *      asserts that block exists as a return from UArray2_at
 *      asserts that elem exists as a return from UArray_at
 *      takes in an apply mapping function
 * 
 ************************/
extern void  UArray2b_map(T array2b, 
                          void apply(int col, int row, T array2b,
                                     void *elem, void *cl), 
                          void *cl)
{
        /* width and height calculation from UArray2 */
        int num_blocks_width = UArray2_width(array2b->elems);
        int num_blocks_height = UArray2_height(array2b->elems);
        int bs = array2b->blocksize;

        /* mapping through according to block major logic */
        for (int row = 0; row < num_blocks_height; row++) {
                for (int col = 0; col < num_blocks_width; col++) {

                        UArray_T *block = UArray2_at(array2b->elems, col, row);
                        assert(block);

                        /* mapping through a block's invidual rows and cols */
                        for (int b_col = 0; b_col < bs; b_col++) {
                                for (int b_row = 0; b_row < bs; b_row++) {
                                    
                                        bool beyond_width = col * bs + 
                                            (b_col + 1) > array2b->width;
                                        bool beyond_height = row * bs + 
                                            (b_row + 1) > array2b->height;

                                        /* continue if beyond a block's
                                        dimensions */
                                        if (beyond_width || beyond_height) {
                                                continue;

                                        } else {
                                                int f_i = flattened_index(b_col,
                                                                          b_row,
                                                                          bs);
                                                void *elem = UArray_at(*block, 
                                                                        f_i);
                                                assert(elem);

                                                apply(col * bs + b_col, 
                                                row * bs + b_row, array2b,
                                                elem, cl);
                                        }
                                }
                        }
                }
        }
}

#undef T