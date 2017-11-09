//
// Created by Jamie on 27/10/2017.
//
#include <stdio.h>
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
        solvePairCellsInColumn(p, column);
    }

    for (int row = 0; row < size; row++) {
        solvePairCellsInRow(p, row);
    }

    for (int leftColumn = 0; leftColumn < size; leftColumn = leftColumn + sizeRoot) {
        for (int topRow = 0; topRow < size; topRow = topRow + sizeRoot) {
            // For every top left cell of a block
            solvePairCellsInBlock(p, leftColumn, topRow);
        }
    }
}


//TODO check for x cells in a row, column or block that have the same x numbers in each. Eliminate those x numbers from all other cells in the row column or block.
void solvePairCellsInRow(int ***p, int row) {

    for (int columnA = 0; columnA < size; columnA++) {
        for (int columnB = 0; columnB < size; columnB++) {
            // Only run each option once
            if (columnA < columnB) {

                // If both cells are not yet solved
                if (findFinalCellValue(p, columnA, row) == 0 && findFinalCellValue(p, columnB, row) == 0) {

                    int valuesAbsentTogether = 0;
                    int valuesPresentTogether = 0;
                    int valuesElsewhereCalculated[size + 1];
                    int valuesElsewhere[size + 1]; //True or false if a value can be in a place other then thease two cells

                    for (int option = 1; option <= size; option++) {
                        valuesElsewhereCalculated[option] = 0;
                        valuesElsewhere[option] = 0;
                    }

                    // for every option if both cells have that value possible add to values that match.
                    for (int option = 1; option <= size; option++) {
                        if ((p[columnA][row][option] == 1) && (p[columnB][row][option] == 1)) {
                            valuesPresentTogether++;
                            valuesElsewhereCalculated[option] = 1;

                            if (cellsWithSuggestionInRow(p, row, option) > 2) {
                                valuesElsewhere[option] = 1;
                            }
                        } else if ((p[columnA][row][option] == 0) && (p[columnB][row][option] == 0)) {
                            valuesAbsentTogether++;
                        }
                    }

                    // Cells are are identical
                    if (valuesPresentTogether == 2 && valuesAbsentTogether == 7) {

                        for (int option = 1; option <= size; option++) {
                            if (p[columnA][row][option] == 1 && p[columnB][row][option] == 1) {
                                eliminatePossibleFromRow(p, columnA, row, option);
                                setIfPossible(p, columnB, row, option, 1);
                            }
                        }
                    } else if (valuesPresentTogether >= 2) { //If values share 2 or more values but 2 of those values can only exist in that one place
                        int count = 0; //The number of values that must occupy one of these two cells

                        for (int option = 0; option <= size; option++) {
                            if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                                count++;
                            }
                        }

                        if (count == 2) {
                            for (int option = 1; option <= size; option++) {
                                if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) { // Remove these numbers from being suggestions elsewhere
                                    eliminatePossibleFromRow(p, columnA, row, option);
                                    setIfPossible(p, columnB, row, option, 1);
                                } else { // Remove suggestions for other values in these cells
                                    setIfPossible(p, columnA, row, option, 0);
                                    setIfPossible(p, columnB, row, option, 0);
                                }
                            }
                        }
                    }

                }
            }
        }
    }
}

//TODO check for x cells in a row, column or block that have the same x numbers in each. Eliminate those x numbers from all other cells in the row column or block.
void solvePairCellsInColumn(int ***p, int column) {

    for (int rowA = 0; rowA < size; rowA++) {
        for (int rowB = 0; rowB < size; rowB++) {
            // Only run each option once
            if (rowA < rowB) {

                // If both cells are not yet solved
                if (findFinalCellValue(p, column, rowA) == 0 && findFinalCellValue(p, column, rowB) == 0) {

                    int valuesAbsentTogether = 0;
                    int valuesPresentTogether = 0;
                    int valuesElsewhereCalculated[size + 1];
                    int valuesElsewhere[size + 1]; //True or false if a value can be in a place other then thease two cells

                    for (int option = 1; option <= size; option++) {
                        valuesElsewhereCalculated[option] = 0;
                        valuesElsewhere[option] = 0;
                    }


                    // for every option if both cells have that value possible add to values that match.
                    for (int option = 1; option <= size; option++) {
                        if ((p[column][rowA][option] == 1) && (p[column][rowB][option] == 1)) {
                            valuesPresentTogether++;
                            valuesElsewhereCalculated[option] = 1;

                            if (cellsWithSuggestionInColumn(p, column, option) > 2) {
                                valuesElsewhere[option] = 1;
                            }
                        } else if ((p[column][rowA][option] == 0) && (p[column][rowB][option] == 0)) {
                            valuesAbsentTogether++;
                        }
                    }

                    // Cells are are identical
                    if (valuesPresentTogether == 2 && valuesAbsentTogether == 7) {

                        for (int option = 1; option <= size; option++) {
                            if (p[column][rowA][option] == 1 && p[column][rowB][option] == 1) {
                                eliminatePossibleFromColumn(p, column, rowA, option);
                                setIfPossible(p, column, rowB, option, 1);
                            }
                        }
                    } else if (valuesPresentTogether >= 2) { //If values share 2 or more values but 2 of those values can only exist in that one place
                        int count = 0; //The number of values that must occupy one of these two cells

                        for (int option = 0; option <= size; option++) {
                            if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                                count++;
                            }
                        }

                        if (count == 2) {
                            for (int option = 1; option <= size; option++) {
                                if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) { // Remove these numbers from being suggestions elsewhere
                                    eliminatePossibleFromColumn(p, column, rowA, option);
                                    setIfPossible(p, column, rowB, option, 1);
                                } else { // Remove suggestions for other values in these cells
                                    setIfPossible(p, column, rowA, option, 0);
                                    setIfPossible(p, column, rowB, option, 0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//TODO check for x cells in a row, column or block that have the same x numbers in each. Eliminate those x numbers from all other cells in the row column or block.
void solvePairCellsInBlock(int ***p, int columnStart, int rowStart) {

    // Find row and column address for top left cell in block
    int r = rowStart - (rowStart % sizeRoot);
    int c = columnStart - (columnStart % sizeRoot);

    for (int columnAOffset = 0; columnAOffset < sizeRoot; columnAOffset++) {
        for (int rowAOffset = 0; rowAOffset < sizeRoot; rowAOffset++) {

            int columnA = c + columnAOffset;
            int rowA = r + rowAOffset;

            for (int columnBOffset = 0; columnBOffset < sizeRoot; columnBOffset++) {
                for (int rowBOffset = 0; rowBOffset < sizeRoot; rowBOffset++) {

                    int columnB = c + columnBOffset;
                    int rowB = r + rowBOffset;


                    if (columnA < columnB || rowA < rowB) {

                        // If both cells are not yet solved
                        if (findFinalCellValue(p, columnA, rowA) == 0 && findFinalCellValue(p, columnB, rowB) == 0) {

                            int valuesAbsentTogether = 0;
                            int valuesPresentTogether = 0;
                            int valuesElsewhereCalculated[size + 1];
                            int valuesElsewhere[size + 1]; //True or false if a value can be in a place other then thease two cells

                            for (int option = 1; option <= size; option++) {
                                valuesElsewhereCalculated[option] = 0;
                                valuesElsewhere[option] = 0;
                            }


                            // for every option if both cells have that value possible add to values that match.
                            for (int option = 1; option <= size; option++) {
                                if ((p[columnA][rowA][option] == 1) && (p[columnB][rowB][option] == 1)) {
                                    valuesPresentTogether++;
                                    valuesElsewhereCalculated[option] = 1;

                                    if (cellsWithSuggestionInBlock(p, columnA, rowA, option) > 2) {
                                        valuesElsewhere[option] = 1;
                                    }
                                } else if ((p[columnA][rowA][option] == 0) && (p[columnB][rowB][option] == 0)) {
                                    valuesAbsentTogether++;
                                }
                            }

                            // Cells are are identical
                            if (valuesPresentTogether == 2 && valuesAbsentTogether == 7) {

                                for (int option = 1; option <= size; option++) {
                                    if (p[columnA][rowA][option] == 1 && p[columnB][rowB][option] == 1) {
                                        eliminatePossibleFromBlock(p, columnA, rowA, option);
                                        setIfPossible(p, columnB, rowB, option, 1);
                                    }
                                }
                            } else if (valuesPresentTogether >=
                                       2) { //If values share 2 or more values but 2 of those values can only exist in that one place
                                int count = 0; //The number of values that must occupy one of thease two cells

                                for (int option = 0; option <= size; option++) {
                                    if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                                        count++;
                                    }
                                }

                                if (count == 2) {
                                    for (int option = 1; option <= size; option++) {
                                        if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] ==
                                                                            1) { // Remove thease numbers from being suggestions elsewhere
                                            eliminatePossibleFromBlock(p, columnA, rowA, option);
                                            setIfPossible(p, columnB, rowB, option, 1);
                                        } else { // Remove suggestions for other values in thease cells
                                            setIfPossible(p, columnA, rowA, option, 0);
                                            setIfPossible(p, columnB, rowB, option, 0);
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
}
