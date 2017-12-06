//
// Created by Jamie on 27/10/2017.
//
#include <math.h>
#include <stdio.h>
#include "global.h"
#include "cellFunctions.h"
#include "utilityFunctions.h"

extern int steps;

/**
 * Solves hidden singles in all rows of the given sudoku and returns the amount of changes made
 *
 * @param p         Sudoku pointer
 * @return          Number of changes made
 */
int solveRows(int ***p) {
    Cell cell;
    Block block;

    int changes = 0;

    for (int row = 0; row < size; row++) { // Traveling down
        cell.row = row;

        // Remove suggestions for values in the same row, column or block.
        for (int column = 0; column < size; column++) { // Traveling right
            cell.column = column;
            setBlockForCell(&block, cell);

            if (findFinalCellValue(p, cell) == 0) {

                // If this is only cell in this row, column or block that can have a value set it to that value
                for (int option = 1; option <= size; option++) {
                    if (p[cell.column][cell.row][option] == 1) {
                        if (cellsWithSuggestionInColumn(p, cell.column, option) == 1
                            || cellsWithSuggestionInRow(p, cell.row, option) == 1
                            || cellsWithSuggestionInBlock(p, block, option) == 1) {

                            steps++;
                            if (outputSolveSteps) {
                                printf("[Step %i]: Found and solved hidden single in row %i for cell %i, %i. Setting value to %i \n", steps ,cell.row, cell.column, cell.row, option);
                            }

                            changes++;
                            setFinalCalculatedValue(p, cell, option);
                        }
                    }
                }
            }
        }
    }

    return changes;
}

/**
 * Solves hidden singles in all columns of the given sudoku and returns the amount of changes made
 *
 * @param p         Sudoku pointer
 * @return          Number of changes made
 */
int solveColumns(int ***p) {
    Cell cell;
    Block block;

    int changes = 0;

    for (int column = 0; column < size; column++) { // Traveling right
        cell.column = column;

        for (int row = 0; row < size; row++) { // Traveling down
            cell.row = row;
            setBlockForCell(&block, cell);

            if (findFinalCellValue(p, cell) == 0) {

                // If this is only cell in this row, column or block that can have a value set it to that value
                for (int option = 1; option <= size; option++) {
                    if (p[cell.column][cell.row][option] == 1) {
                        if (cellsWithSuggestionInColumn(p, cell.column, option) == 1
                            || cellsWithSuggestionInRow(p, cell.row, option) == 1
                            || cellsWithSuggestionInBlock(p, block, option) == 1) {

                            steps++;
                            if (outputSolveSteps) {
                                printf("[Step %i]: Found and solved hidden single in column %i for cell %i, %i. Setting value to %i \n", steps, cell.column, cell.column, cell.row, option);
                            }

                            setFinalCalculatedValue(p, cell, option);
                            changes++;
                        }
                    }
                }
            }
        }
    }

    return changes;
}

/**
 * Solves hidden singles in all blocks of the given sudoku and returns the amount of changes made
 *
 * @param p         Sudoku pointer
 * @return          Number of changes made
 */
int solveBlocks(int ***p) {
    Cell cell;
    Block block;

    int changes = 0;

    for (int c = 0; c < sizeRoot; c++) {
        block.blockColumn = c;

        for (int r = 0; r < sizeRoot; r++) {
            block.blockRow = r;

            for (int columnOffset = 0; columnOffset < sizeRoot; columnOffset++) {
                cell.column = (block.blockColumn * sizeRoot) + columnOffset;

                for (int rowOffset = 0; rowOffset < sizeRoot; rowOffset++) {
                    cell.row = (block.blockRow * sizeRoot) + rowOffset;

                    if (findFinalCellValue(p, cell) == 0) {

                        // If this is only cell in this row, column or block that can have a value set it to that value
                        for (int option = 1; option <= size; option++) {
                            if (p[cell.column][cell.row][option] == 1) {
                                if (cellsWithSuggestionInColumn(p, cell.column, option) == 1
                                    || cellsWithSuggestionInRow(p, cell.row, option) == 1
                                    || cellsWithSuggestionInBlock(p, block, option) == 1) {

                                    steps++;
                                    if (outputSolveSteps) {
                                        printf("[Step %i]: Found and solved hidden single in block %i %i for cell %i, %i. Setting value to %i \n", steps, block.blockColumn, block.blockRow, cell.column, cell.row, option);
                                    }

                                    setFinalCalculatedValue(p, cell, option);
                                    changes++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return changes;
}

/** Finds cell suggestions to the order entered constrained in certain blocks and columns or rows and eliminates suggestions that are in disallowed places then returns the number of changes made
 *
 * @param p             Sudoku pointer
 * @param order         Order of solutions to find
 * @return              Number of changes made
 */
int solveSuggestionBlockLines(int ***p, int order) {
    int counter[order];
    for (int i = 0; i < order; i++) {
        counter[i] = 0;
    }

    Block blocks[order];
    int blocksSelected = 1;

    int startSteps = steps;

    do {
        int correctOrder = 1;

        for (int j = 0; j < order; j++) {
            blocks[j].blockColumn = counter[j] % sizeRoot;
            blocks[j].blockRow = (counter[j] - blocks[j].blockColumn) / sizeRoot;
        }

        for (int j = sizeRoot - 1; j >= 0; j--) {
            if (blocks[j].blockColumn > 0 || blocks[j].blockRow > 0) {
                blocksSelected = j + 1;
                break;
            }
        }


        int columnSelection = 1;
        int rowSelection = 1;


        if (blocksSelected > 1) {
            for (int j = 0; j < blocksSelected - 1; j++) {

                if (blocks[j].blockRow * sizeRoot + blocks[j].blockColumn >=
                    blocks[j + 1].blockRow * sizeRoot + blocks[j + 1].blockColumn) {
                    correctOrder = 0;
                }

                if (blocks[j].blockColumn != blocks[j + 1].blockColumn) {
                    columnSelection = 0;
                }

                if (blocks[j].blockRow != blocks[j + 1].blockRow) {
                    rowSelection = 0;
                }
            }
        }

        // If block combination has not been used before and is valid.
        if (blocksSelected == order && correctOrder && (columnSelection || rowSelection)) {

            Cell cell;

            if (columnSelection) {

                int leftColumn = blocks[0].blockColumn;

                for (int option = 1; option <= size; option++) {

                    int columnsUsed[sizeRoot];
                    for (int j = 0; j < sizeRoot; j++) {
                        columnsUsed[j] = 0;
                    }


                    for (int j = 0; j < blocksSelected; j++) {

                        for (int columnOffset = 0; columnOffset < sizeRoot; columnOffset++) {
                            int column = columnOffset + (blocks[j].blockColumn * sizeRoot);

                            for (int rowOffset = 0; rowOffset < sizeRoot; rowOffset++) {
                                int row = rowOffset + (blocks[j].blockRow * sizeRoot);

                                if (p[column][row][option] == 1) {
                                    columnsUsed[columnOffset] = 1;
                                    break;
                                }
                            }

                        }
                    }

                    int usedColumnCount = 0;
                    for (int c = 0; c < sizeRoot; c++) {
                        if (columnsUsed[c] == 1) {
                            usedColumnCount++;
                        }
                    }

                    if (usedColumnCount == blocksSelected) {
                        for (int c = 0; c < sizeRoot; c++) {
                            if (columnsUsed[c] == 1) {

                                // If this column was used we must eliminate this option from rows of the column that are not in the selected blocks
                                for (int row = 0; row < size; row++) {
                                    int rowInSelection = 0;
                                    for (int j = 0; j < sizeRoot; j++) {
                                        if (row >= (blocks[j].blockRow * sizeRoot) && row < (blocks[j].blockRow + 1) * sizeRoot) {
                                            rowInSelection = 1;
                                            break;
                                        }
                                    }

                                    int madeChange = 0;

                                    if (!rowInSelection && (p[(leftColumn * sizeRoot) + c][row][option] == 1)) {
                                        cell.row = row;
                                        cell.column = (leftColumn * sizeRoot) + c;
                                        eliminatePossibleFromCell(p, cell, option);

                                        if (!madeChange) {
                                            madeChange = 1;
                                            steps++;

                                            if (outputSolveSteps) {
                                                printf("[Step %i]: Found constrained row suggestion of order eliminated %i from blocks", steps, order, option);

                                                for (int j = 0; j < blocksSelected; j++) {
                                                    printf(" %i, %i ", blocks[j].blockColumn, blocks[j].blockRow);
                                                }

                                                printf("\n");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (rowSelection) {

                int leftrow = blocks[0].blockRow;

                for (int option = 1; option <= size; option++) {

                    int rowsUsed[sizeRoot];
                    for (int j = 0; j < sizeRoot; j++) {
                        rowsUsed[j] = 0;
                    }


                    for (int j = 0; j < blocksSelected; j++) {

                        for (int rowOffset = 0; rowOffset < sizeRoot; rowOffset++) {
                            int row = rowOffset + (blocks[j].blockRow * sizeRoot);

                            for (int columnOffset = 0; columnOffset < sizeRoot; columnOffset++) {
                                int column = columnOffset + (blocks[j].blockColumn * sizeRoot);

                                if (p[row][column][option] == 1) {
                                    rowsUsed[rowOffset] = 1;
                                    break;
                                }
                            }

                        }
                    }

                    int usedrowCount = 0;
                    for (int c = 0; c < sizeRoot; c++) {
                        if (rowsUsed[c] == 1) {
                            usedrowCount++;
                        }
                    }

                    if (usedrowCount == blocksSelected) {
                        for (int c = 0; c < sizeRoot; c++) {
                            if (rowsUsed[c] == 1) {

                                // If this row was used we must eliminate this option from columns of the row that are not in the selected blocks
                                for (int column = 0; column < size; column++) {
                                    int columnInSelection = 0;
                                    for (int j = 0; j < sizeRoot; j++) {
                                        if (column >= (blocks[j].blockColumn * sizeRoot) && column < (blocks[j].blockColumn + 1) * sizeRoot) {
                                            columnInSelection = 1;
                                            break;
                                        }
                                    }

                                    int madeChange = 0;

                                    if (!columnInSelection && (p[(leftrow * sizeRoot) + c][column][option] == 1)) {
                                        cell.column = column;
                                        cell.row = (leftrow * sizeRoot) + c;
                                        eliminatePossibleFromCell(p, cell, option);

                                        if (!madeChange) {
                                            madeChange = 1;
                                            steps++;

                                            if (outputSolveSteps) {
                                                printf("[Step %i]: Found constrained row suggestion of order eliminated %i from blocks", steps, order, option);

                                                for (int j = 0; j < blocksSelected; j++) {
                                                    printf(" %i, %i ", blocks[j].blockColumn, blocks[j].blockRow);
                                                }

                                                printf("\n");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } while (!nBitCounterAddOne(order, counter, size));

    return steps - startSteps;
}