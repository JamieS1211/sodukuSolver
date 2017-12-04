//
// Created by Jamie on 27/10/2017.
//
#include <stdio.h>
#include <math.h>
#include "config.h"
#include "main.h"
#include "printFunctions.h"

void solveRows(int ***p) {
    int options = size;

    for (int row = 0; row < size; row++) {
        // Traveling down

        // For each row we check every column to scan left to right on each row.
        int temp[options];

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
            } else {
                temp[value] = 1;
                setOnlyPossible(p, column, row, value);
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
            } else {
                temp[value] = 1;
                setOnlyPossible(p, column, row, value);
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
                    } else {
                        temp[value] = 1;
                        setOnlyPossible(p, column, row, value);
                    }
                }
            }

        }
    }
    return;
}

void solveSuggestionBlockLines(int ***p) {


    Block blocks[sizeRoot];
    int blocksSelected = 1;

    for (int i = 0; i < pow(size, sizeRoot); i++) {

        int count = i;
        int correctOrder = 1;

        for (int j = size - 1; j >= 0; j--) {
            int positionWorth = pow(size, j);
            int positionValue = 0;

            while (count >= positionWorth) {
                count = count - positionWorth;
                positionValue++;
            }

            blocks[j].blockColumn = positionValue % sizeRoot;
            blocks[j].blockRow = (positionValue - blocks[j].blockColumn) / sizeRoot;
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
        if (correctOrder && (columnSelection || rowSelection)) {

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

                                    if (!rowInSelection && (p[(leftColumn * sizeRoot) + c][row][option] == 1)) {
                                        setIfPossible(p, (leftColumn * sizeRoot) + c, row, option, 0);
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