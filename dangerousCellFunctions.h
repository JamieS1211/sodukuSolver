//
// Created by Jamie on 05/12/2017.
//

/**
 * Eliminates the given number from anywhere in the given column
 *
 * @param p             Sudoku pointer
 * @param column        Column
 * @param number
 */
void eliminatePossibleFromColumn(int ***p, int column, int number);


/**
 * Eliminates the given number from anywhere in the given column
 *
 * @param p             Sudoku pointer
 * @param column        Row
 * @param number
 */
void eliminatePossibleFromRow(int ***p, int row, int number);

/**
 * Eliminates the given number from anywhere in the given column
 *
 * @param p             Sudoku pointer
 * @param block        Block
 * @param number
 */
void eliminatePossibleFromBlock(int ***p, Block block, int number);