//
// Created by Jamie on 05/12/2017.
//
#include "global.h"
#include "cellFunctions.h"

/**
 * Eliminates the given number from anywhere in the given column
 *
 * @param p             Sudoku pointer
 * @param column        Column
 * @param number
 */
void eliminatePossibleFromColumn(int ***p, int column, int number) {
    Cell cell;
    cell.column = column;
    for (int row = 0; row < size; row++) {
        cell.row = row;

        eliminatePossibleFromCell(p, cell, number);
    }
}

/**
 * Eliminates the given number from anywhere in the given column
 *
 * @param p             Sudoku pointer
 * @param column        Row
 * @param number
 */
void eliminatePossibleFromRow(int ***p, int row, int number) {
    Cell cell;
    cell.row = row;
    for (int column = 0; column < size; column++) {
        cell.column = column;


        eliminatePossibleFromCell(p, cell, number);
    }
}

/**
 * Eliminates the given number from anywhere in the given column
 *
 * @param p             Sudoku pointer
 * @param block        Block
 * @param number
 */
void eliminatePossibleFromBlock(int ***p, Block block, int number) {
    Cell cell;
    for (int columnOffset = 0; columnOffset < sizeRoot; columnOffset++) {
        cell.column = (block.blockColumn * sizeRoot) + columnOffset;
        for (int rowOffset = 0; rowOffset < sizeRoot; rowOffset++) {
            cell.row = (block.blockRow * sizeRoot) + rowOffset;

            eliminatePossibleFromCell(p, cell, number);
        }
    }
}