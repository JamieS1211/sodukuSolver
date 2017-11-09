//
// Created by Jamie on 27/10/2017.
//
#include <stdio.h>
#include <math.h>
#include "config.h"
#include "main.h"

void solveRows(int ***p) {
    int options = size;

    for (int row = 0; row < size; row++) {
        // Traveling down

        // For each row we check every column to scan left to right on each row.
        int temp[options];
        int unknowns = 0;

        // Remove suggestions for values in the same row, column or block.
        for (int column = 0; column < size; column++) {
            // Traveling right
            int value = findFinalCellValue(p, column, row);
            if (value == 0) {

                // If this is only cell in this row, column or block that can have a value set it to that value
                for (int option = 1; option <= options; option++) {
                    if (p[column][row][option] == 1) {
                        if (cellsWithSuggestionInColumn(p, column, option) == 1
                            || cellsWithSuggestionInRow(p, row, option) == 1
                            || cellsWithSuggestionInBlock(p, column, row, option) == 1) {
                            setOnlyPossible(p, column, row, option);
                        }
                    }
                }

                unknowns++;
            } else {
                temp[value] = 1;
                eliminatePossibleFromRow(p, column, row, value);
                eliminatePossibleFromColumn(p, column, row, value);
                eliminatePossibleFromBlock(p, column, row, value);
            }
        }
    }
}


void solveColumns(int ***p) {
    int options = size;

    for (int column = 0; column < size; column++) {
        // Traveling right

        // For each column we check every row to scan up to down on each column.
        int temp[options];
        int unknowns = 0;

        for (int row = 0; row < size; row++) {
            // Traveling down
            int value = findFinalCellValue(p, column, row);
            if (value == 0) {

                // If this is only cell in this row, column or block that can have a value set it to that value
                for (int option = 1; option <= options; option++) {
                    if (p[column][row][option] == 1) {
                        if (cellsWithSuggestionInColumn(p, column, option) == 1
                            || cellsWithSuggestionInRow(p, row, option) == 1
                            || cellsWithSuggestionInBlock(p, column, row, option) == 1) {
                            setOnlyPossible(p, column, row, option);
                        }
                    }
                }

                unknowns++;
            } else {
                temp[value] = 1;
                eliminatePossibleFromRow(p, column, row, value);
                eliminatePossibleFromColumn(p, column, row, value);
                eliminatePossibleFromBlock(p, column, row, value);
            }
        }


    }
}

void solveBlocks(int ***p) {
    int options = size;

    for (int c = 0; c < size; c += sizeRoot) {
        for (int r = 0; r < size; r += sizeRoot) {

            for (int columnOffset = 0; columnOffset < sizeRoot; columnOffset++) {
                for (int rowOffset = 0; rowOffset < sizeRoot; rowOffset++) {
                    // Option is a possible value of this cell.
                    int column = c + columnOffset;
                    int row = r + rowOffset;
                    int temp[options];
                    int unknowns = 0;

                    int value = findFinalCellValue(p, column, row);
                    if (value == 0) {

                        // If this is only cell in this row, column or block that can have a value set it to that value
                        for (int option = 1; option <= options; option++) {
                            if (p[column][row][option] == 1) {
                                if (cellsWithSuggestionInColumn(p, column, option) == 1
                                    || cellsWithSuggestionInRow(p, row, option) == 1
                                    || cellsWithSuggestionInBlock(p, column, row, option) == 1) {
                                    setOnlyPossible(p, column, row, option);
                                }
                            }
                        }

                        unknowns++;
                    } else {
                        temp[value] = 1;
                        eliminatePossibleFromRow(p, column, row, value);
                        eliminatePossibleFromColumn(p, column, row, value);
                        eliminatePossibleFromBlock(p, column, row, value);
                    }
                }
            }

        }
    }
    return;
}

// Removes possibilities when all suggestions for a number in a block are in same row or column
void solveSuggestionBlockLines(int ***p) {

    for (int leftColumn = 0; leftColumn < size; leftColumn = leftColumn + sizeRoot) {
        for (int topRow = 0; topRow < size; topRow = topRow + sizeRoot) {

            for (int option = 1; option <= size; option++) {

                if (cellsWithSuggestionInBlock(p, leftColumn, topRow, option) >= 2) {

                    int cValuesUsed[sizeRoot];
                    int rValuesUsed[sizeRoot];

                    for (int i = 0; i < sizeRoot; i++) {
                        cValuesUsed[i] = 0;
                        rValuesUsed[i] = 0;
                    }

                    for (int c = 0; c < sizeRoot; c++) {
                        int column = leftColumn + c;

                        for (int r = 0; r < sizeRoot; r++) {
                            int row = topRow + r;

                            // If that value is at this position mark this row and column as used
                            if (p[column][row][option] == 1) {
                                cValuesUsed[c] = 1;
                                rValuesUsed[r] = 1;
                            }
                        }
                    }

                    int usedColumnCount = 0;
                    int lastColumnUsed = 0;
                    for (int c = 0; c < sizeRoot; c++) {
                        if (cValuesUsed[c] == 1) {
                            usedColumnCount++;
                            lastColumnUsed = leftColumn + c;
                        }
                    }

                    int usedRowCount = 0;
                    int lastRowUsed = 0;
                    for (int r = 0; r < sizeRoot; r++) {
                        if (rValuesUsed[r] == 1) {
                            usedRowCount++;
                            lastRowUsed = topRow + r;
                        }
                    }

                    if (usedColumnCount == 1) {
                        for (int row = 0; row < size; row++) {
                            if ((row < topRow || row >= topRow + sizeRoot) && p[lastColumnUsed][row][option] == 1) {
                                setIfPossible(p, lastColumnUsed, row, option, 0);
                            }
                        }
                    }

                    if (usedRowCount == 1) {
                        for (int column = 0; column < size; column++) {
                            if ((column < leftColumn || column >= leftColumn + sizeRoot) && p[column][lastRowUsed][option] == 1) {
                                setIfPossible(p, column, lastRowUsed, option, 0);
                            }
                        }
                    }
                }
            }
        }
    }
}

void solvePairs(int ***p) {

    for (int column = 0; column < size; column++) {
        findPairCellsInColumn(p, column);
    }

    for (int row = 0; row < size; row++) {
        findPairCellsInRow(p, row);
    }

    for (int leftColumn = 0; leftColumn < size; leftColumn = leftColumn + sizeRoot) {
        for (int topRow = 0; topRow < size; topRow = topRow + sizeRoot) {
            // For every top left cell of a block
            findPairCellsInBlock(p, leftColumn, topRow);
        }
    }
}
