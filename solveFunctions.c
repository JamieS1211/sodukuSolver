//
// Created by Jamie on 27/10/2017.
//
#include <stdio.h>
#include <math.h>
#include "config.h"
#include "main.h"
#include "printFunctions.h"

void solveRows(int ***p) {
    for (int row = 0; row < size; row++) {
        solveCellsInRow(p, row, 1);
    }
}


void solveColumns(int ***p) {
    for (int column = 0; column < size; column++) {
        solveCellsInColumn(p, column, 1);
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
       solveCellsInColumn(p, column, 2);
    }

    for (int row = 0; row < size; row++) {
        solveCellsInRow(p, row, 2);
    }

    for (int leftColumn = 0; leftColumn < size; leftColumn = leftColumn + sizeRoot) {
        for (int topRow = 0; topRow < size; topRow = topRow + sizeRoot) {
            // For every top left cell of a block
            solvePairCellsInBlock(p, leftColumn, topRow);
        }
    }
}


//TODO check for x cells in a row, column or block that have the same x numbers in each. Eliminate those x numbers from all other cells in the row column or block.
void solveCellsInRow(int ***p, int row, int maxOrder) {
    printSudokuWithSuggestions(p);

    int maximumToCheck = size / 2;
    if (maxOrder < maximumToCheck) {
        maximumToCheck = maxOrder;
    }

    int columns[size];
    int columnsRequired = 1;

    for (int i = 0; i < pow(size, maximumToCheck); i++) {

        int previousColumnValue = size;
        int count = i;

        int correctOrder = 1;

        for (int j = size - 1; j >= 0; j--) {
            int positionWorth = pow(size, j) ;
            int positionValue = 0;

            while (count >= positionWorth) {
                count = count - positionWorth;
                positionValue++;
            }

            if (positionValue > 0) {
                if (positionValue >= previousColumnValue) {
                    correctOrder = 0;
                }
                previousColumnValue = positionValue;
            }

            columns[j] = positionValue;
        }

        for (int j = size - 1; j >= 0; j--) {
            if (columns[j] > 0) {
                columnsRequired = j + 1;
                break;
            }
        }

        // If row combination has not been used before and is valid.
        if (correctOrder == 1 && columnsRequired > 0) {
            int solvedCell = 0;
            for (int l = 0; l < columnsRequired; l++) {
                if (findFinalCellValue(p, columns[l], row) != 0) {
                    solvedCell = 1;
                    break;
                }
            }


            if (solvedCell != 0 && columnsRequired == 1) { // If this is just a value
                eliminatePossibleFromRow(p, columns[0], row, findFinalCellValue(p, columns[0], row));
                eliminatePossibleFromColumn(p, columns[0], row, findFinalCellValue(p, columns[0], row));
                eliminatePossibleFromBlock(p, columns[0], row, findFinalCellValue(p, columns[0], row));
            } else if (solvedCell == 0) {// If no cells are solved

                int valuesAbsentTogether = 0;
                int valuesPresentTogether = 0;
                int valuesElsewhereCalculated[size + 1] = {-1};
                int valuesElsewhere[size + 1] = {-1}; //True or false if a value can be in a place other then thease two cells

                for (int option = 1; option <= size; option++) {
                    valuesElsewhereCalculated[option] = 0;
                    valuesElsewhere[option] = 0;
                }


                // for every option if both cells have that value possible add to values that match.
                for (int option = 1; option <= size; option++) {

                    int valuePresentInAllCells = 1;
                    int valueAbsentInAllCells = 1;

                    for (int m = 0; m < columnsRequired; m++) {
                        if (p[columns[m]][row][option] == 0) {
                            valuePresentInAllCells = 0;
                            break;
                        }
                    }

                    for (int m = 0; m < columnsRequired; m++) {
                        if (p[columns[m]][row][option] == 1) {
                            valueAbsentInAllCells = 0;
                            break;
                        }
                    }


                    if (valuePresentInAllCells) {
                        valuesPresentTogether++;
                        valuesElsewhereCalculated[option] = 1;

                        if (cellsWithSuggestionInRow(p, row, option) > columnsRequired) {
                            valuesElsewhere[option] = 1;
                        } else if (cellsWithSuggestionInRow(p, row, option) == -1) {
                            valuesElsewhere[option] = -1;
                        }
                    } else if (valueAbsentInAllCells) {
                        valuesAbsentTogether++;
                    }
                }

                // Cells are are identical and must be split between these x cells
                if (valuesPresentTogether == columnsRequired && valuesAbsentTogether == size - columnsRequired) {
                    for (int option = 1; option <= size; option++) {

                        int valuePresentInAllCells = 1;

                        for (int m = 0; m < columnsRequired; m++) {
                            if (p[columns[m]][row][option] == 0) {
                                valuePresentInAllCells = 0;
                                break;
                            }
                        }

                        if (valuePresentInAllCells) {
                            eliminatePossibleFromRow(p, columns[0], row, option);

                            for (int m = 0; m < columnsRequired; m++) {
                                setIfPossible(p, columns[m], row, option, 1);
                            }
                        }
                    }
                } else if (valuesPresentTogether >= columnsRequired) {
                    int count = 0; //The number of values that must occupy one of these two cells

                    for (int option = 0; option <= size; option++) {
                        if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                            count++;
                        }
                    }

                    if (count == columnsRequired) {
                        for (int option = 1; option <= size; option++) {
                            if (valuesElsewhere[option] == 1 && valuesElsewhereCalculated[option] == 1) {
                                { // Remove suggestions for other values in these cells
                                    for (int m = 0; m < columnsRequired; m++) {
                                        setIfPossible(p, columns[m], row, option, 0);
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

//TODO check for x cells in a row, column or block that have the same x numbers in each. Eliminate those x numbers from all other cells in the row column or block.
void solveCellsInColumn(int ***p, int column, int maxOrder) {
    int maximumToCheck = size / 2;
    if (maxOrder < maximumToCheck) {
        maximumToCheck = maxOrder;
    }

    int rows[size];
    int rowsRequired;

    for (int i = 0; i < pow(size, maximumToCheck); i++) {

        int previousRowValue = size;
        int count = i;

        int correctOrder = 1;

        for (int j = size - 1; j >= 0; j--) {
            int positionWorth = pow(size, j) ;
            int positionValue = 0;

            while (count >= positionWorth) {
                count = count - positionWorth;
                positionValue++;
            }

            if (positionValue > 0) {
                if (positionValue >= previousRowValue) {
                    correctOrder = 0;
                }
                previousRowValue = positionValue;
            }

            rows[j] = positionValue;
        }

        for (int j = size - 1; j >= 0; j--) {
            if (rows[j] > 0) {
                rowsRequired = j + 1;
                break;
            }
        }

        // If row combination has not been used before and is valid.
        if (correctOrder && rowsRequired > 0) {
            int solvedCell = 0;
            for (int l = 0; l < rowsRequired; l++) {
                if (findFinalCellValue(p, column, rows[l]) != 0) {
                    solvedCell = 1;
                    break;
                }
            }

            if (solvedCell != 0 && rowsRequired == 1) { // If this is just a value
                eliminatePossibleFromRow(p, column, rows[0], findFinalCellValue(p, column, rows[0]));
                eliminatePossibleFromColumn(p, column, rows[0], findFinalCellValue(p, column, rows[0]));
                eliminatePossibleFromBlock(p, column, rows[0], findFinalCellValue(p, column, rows[0]));
            } else if (solvedCell == 0) {// If no cells are solved

                int valuesAbsentTogether = 0;
                int valuesPresentTogether = 0;
                int valuesElsewhereCalculated[size + 1] = {-1};
                int valuesElsewhere[size + 1] = {-1}; //True or false if a value can be in a place other then thease two cells

                for (int option = 1; option <= size; option++) {
                    valuesElsewhereCalculated[option] = 0;
                    valuesElsewhere[option] = 0;
                }


                // for every option if both cells have that value possible add to values that match.
                for (int option = 1; option <= size; option++) {

                    int valuePresentInAllCells = 1;
                    int valueAbsentInAllCells = 1;

                    for (int m = 0; m < rowsRequired; m++) {
                        if (p[column][rows[m]][option] == 0) {
                            valuePresentInAllCells = 0;
                            break;
                        }
                    }

                    for (int m = 0; m < rowsRequired; m++) {
                        if (p[column][rows[m]][option] == 1) {
                            valueAbsentInAllCells = 0;
                            break;
                        }
                    }


                    if (valuePresentInAllCells) {
                        valuesPresentTogether++;
                        valuesElsewhereCalculated[option] = 1;

                        if (cellsWithSuggestionInColumn(p, column, option) > rowsRequired) {
                            valuesElsewhere[option] = 1;
                        } else if (cellsWithSuggestionInColumn(p, column, option) == -1) {
                            valuesElsewhere[option] = -1;
                        }
                    } else if (valueAbsentInAllCells) {
                        valuesAbsentTogether++;
                    }
                }

                // Cells are are identical and must be split between these x cells
                if (valuesPresentTogether == rowsRequired && valuesAbsentTogether == size - rowsRequired) {
                    printSudokuWithSuggestions(p);

                    for (int option = 1; option <= size; option++) {

                        int valuePresentInAllCells = 1;

                        for (int m = 0; m < rowsRequired; m++) {
                            if (p[column][rows[m]][option] == 0) {
                                valuePresentInAllCells = 0;
                                break;
                            }
                        }

                        if (valuePresentInAllCells) {
                            eliminatePossibleFromColumn(p, column, rows[0], option);

                            for (int m = 0; m < rowsRequired; m++) {
                                setIfPossible(p, column, rows[m], option, 1);
                            }
                        }
                    }


                    printSudokuWithSuggestions(p);
                    printf("test");
                } else if (valuesPresentTogether >= rowsRequired) {
                    int count = 0; //The number of values that must occupy one of these two cells

                    for (int option = 0; option <= size; option++) {
                        if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                            count++;
                        }
                    }

                    if (count == rowsRequired) {
                        for (int option = 1; option <= size; option++) {
                            if (valuesElsewhere[option] == 1 && valuesElsewhereCalculated[option] == 1) {
                                { // Remove suggestions for other values in these cells
                                    for (int m = 0; m < rowsRequired; m++) {
                                        setIfPossible(p, column, rows[m], option, 0);
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

// TODO check for x cells in a row, column or block that have the same x numbers in each. Eliminate those x numbers from all other cells in the row column or block.
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

void solveXWing(int ***p) {
    for (int columnA = 0; columnA < size; columnA++) {
        for (int columnB = 0; columnB < size; columnB++) {

            if (columnA < columnB) {

                for (int rowA = 0; rowA < size; rowA++) {
                    for (int rowB = 0; rowB < size; rowB++) {

                        if (rowA < rowB) {

                            // If all 4 cells are not yet solved
                            if (findFinalCellValue(p, columnA, rowA) == 0 && findFinalCellValue(p, columnA, rowB) == 0
                                && findFinalCellValue(p, columnB, rowA) == 0 &&
                                findFinalCellValue(p, columnB, rowB) == 0) {


                                // for every option if both cells have that value possible add to values that match.
                                for (int option = 1; option <= size; option++) {
                                    if ((p[columnA][rowA][option] == 1) && (p[columnA][rowB][option] == 1) &&
                                        cellsWithSuggestionInColumn(p, columnA, option) == 2
                                        && (p[columnB][rowA][option] == 1) && (p[columnB][rowB][option] == 1) &&
                                        cellsWithSuggestionInColumn(p, columnB, option) == 2) {


                                        printf("\nColumn X-Wing pattern found at cells %i,%i   %i,%i   %i,%i   %i,%i \n",
                                               columnA, rowA, columnA, rowB, columnB, rowA, columnB, rowB);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    for (int rowA = 0; rowA < size; rowA++) {
        for (int rowB = 0; rowB < size; rowB++) {

            if (rowA < rowB) {

                for (int columnA = 0; columnA < size; columnA++) {
                    for (int columnB = 0; columnB < size; columnB++) {

                        if (columnA < columnB) {

                            // If all 4 cells are not yet solved
                            if (findFinalCellValue(p, columnA, rowA) == 0 && findFinalCellValue(p, columnA, rowB) == 0
                                && findFinalCellValue(p, columnB, rowA) == 0 &&
                                findFinalCellValue(p, columnB, rowB) == 0) {


                                // for every option if both cells have that value possible add to values that match.
                                for (int option = 1; option <= size; option++) {
                                    if ((p[columnA][rowA][option] == 1) && (p[columnB][rowA][option] == 1) &&
                                        cellsWithSuggestionInRow(p, rowA, option) == 2
                                        && (p[columnA][rowB][option] == 1) && (p[columnB][rowB][option] == 1) &&
                                        cellsWithSuggestionInRow(p, rowB, option) == 2) {


                                        printf("\nRow X-Wing pattern found at cells %i,%i   %i,%i   %i,%i   %i,%i \n",
                                               columnA, rowA, columnA, rowB, columnB, rowA, columnB, rowB);
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