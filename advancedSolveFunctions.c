//
// Created by Jamie on 27/10/2017.
//
#include <stdio.h>
#include <math.h>
#include "config.h"
#include "main.h"

void solvePairs(int ***p) {

    for (int column = 0; column < size; column++) {
       solveHighOrderCellsInColumn(p, column, 3);
    }

    for (int row = 0; row < size; row++) {
        solveHighOrderCellsInRow(p, row, 3);
    }

    for (int leftColumn = 0; leftColumn < size; leftColumn = leftColumn + sizeRoot) {
        for (int topRow = 0; topRow < size; topRow = topRow + sizeRoot) {
            // For every top left cell of a block
            //solvePairCellsInBlock(p, leftColumn, topRow);
        }
    }
}

void solveHighOrderCellsInRow(int ***p, int row, int maxOrder) {
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
            int positionWorth = pow(size, j);
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

        if (columnsRequired > 1) {
            int allDifferentCells = 1;
            for (int j = 0; j < columnsRequired - 1; j++) {
                if (columns[j] == columns[j + 1]) {
                    allDifferentCells = 0;
                }
            }

            if (allDifferentCells == 0) {
                correctOrder = 0;
            }



            // If row combination has not been used before and is valid.
            if (correctOrder == 1) {
                int solvedCell = 0;
                for (int l = 0; l < columnsRequired; l++) {
                    int value = findFinalCellValue(p, columns[l], row);
                    if (value != 0) {
                        solvedCell = 1;
                        eliminatePossibleFromRowExcept(p, columns[l], row, value);
                        eliminatePossibleFromColumnExcept(p, columns[l], row, value);
                        eliminatePossibleFromBlockExcept(p, columns[l], row, value);
                    }
                }

                if (solvedCell == 0) {// If no cells are solved

                    int valuesAbsentTogether = 0;
                    int valuesPresentTogether = 0;
                    int valuesElsewhereCalculated[size + 1] = {-1};
                    int valuesElsewhere[
                            size + 1] = {-1}; //True or false if a value can be in a place other then thease two cells

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
                                eliminatePossibleFromRow(p, row, option);

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
                                // Exist somewhere else or not in all cells
                                if (valuesElsewhere[option] > 0 || valuesElsewhereCalculated[option] == 0) {
                                    // Remove suggestions for other values in these cells
                                    for (int m = 0; m < columnsRequired; m++) {
                                        setIfPossible(p, columns[m], row, option, 0);
                                    }
                                }
                            }
                        }
                    } else if (valuesPresentTogether < columnsRequired && columnsRequired > 2) { // Cells must be split between thease x cells
                        int totalValuesBetweenCells = 0;
                        int valuesUsedInCells[size + 1] = {0};

                        for (int option = 1; option <= size; option++) {
                            for (int m = 0; m < columnsRequired; m++) {
                                if (p[columns[m]][row][option] == 1) {
                                    valuesUsedInCells[option] = 1;
                                    totalValuesBetweenCells++;
                                    break;
                                }
                            }
                        }



                        if (totalValuesBetweenCells == columnsRequired) {
                            int columnsToEliminate[size];

                            for (int m = 0; m < size; m++) {
                                columnsToEliminate[m] = 1;
                            }

                            for (int m = 0; m < columnsRequired; m++) {
                                columnsToEliminate[columns[m]] = 0;
                            }

                            for (int column = 0; column < size; column++) {
                                for (int option = 0; option < size; option++) {
                                    if (valuesUsedInCells[option] == 1 && columnsToEliminate[column] == 1) {
                                        setIfPossible(p, column, row, option, 0);
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

void solveHighOrderCellsInColumn(int ***p, int column, int maxOrder) {
    int maximumToCheck = size / 2;
    if (maxOrder < maximumToCheck) {
        maximumToCheck = maxOrder;
    }

    int rows[size];
    int rowsRequired;

    for (int i = 0; i < pow(size, maximumToCheck); i++) {

        int count = i;
        int correctOrder = 1;

        for (int j = size - 1; j >= 0; j--) {
            int positionWorth = pow(size, j);
            int positionValue = 0;

            while (count >= positionWorth) {
                count = count - positionWorth;
                positionValue++;
            }

            rows[j] = positionValue;
        }

        for (int j = size - 1; j >= 0; j--) {
            if (rows[j] > 0) {
                rowsRequired = j + 1;
                break;
            }
        }

        if (rowsRequired > 1) {
            int allDifferentCells = 1;

            for (int j = 0; j < rowsRequired - 1; j++) {

                if (rows[j] >= rows[j + 1]) {
                    correctOrder = 0;
                }

                if (rows[j] == rows[j + 1]) {
                    allDifferentCells = 0;
                }
            }

            if (allDifferentCells == 0) {
                correctOrder = 0;
            }

            // If row combination has not been used before and is valid.
            if (correctOrder) {
                int solvedCell = 0;
                for (int l = 0; l < rowsRequired; l++) {
                    int value = findFinalCellValue(p, column, rows[l]);
                    if (value != 0) {
                        solvedCell = 1;
                        eliminatePossibleFromRowExcept(p, column, rows[l], value);
                        eliminatePossibleFromColumnExcept(p, column, rows[l], value);
                        eliminatePossibleFromBlockExcept(p, column, rows[l], value);
                    }
                }

                if (solvedCell == 0) {// If no cells are solved

                    int valuesAbsentTogether = 0;
                    int valuesPresentTogether = 0;
                    int valuesElsewhereCalculated[size + 1] = {-1};
                    int valuesElsewhere[size + 1] = {-1};

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
                        for (int option = 1; option <= size; option++) {

                            int valuePresentInAllCells = 1;

                            for (int m = 0; m < rowsRequired; m++) {
                                if (p[column][rows[m]][option] == 0) {
                                    valuePresentInAllCells = 0;
                                    break;
                                }
                            }

                            if (valuePresentInAllCells) {
                                eliminatePossibleFromColumn(p, column, option);

                                for (int m = 0; m < rowsRequired; m++) {
                                    setIfPossible(p, column, rows[m], option, 1);
                                }
                            }
                        }
                    } else if (valuesPresentTogether >= rowsRequired) { // Other suggestions in cell hiding
                        int count = 0; //The number of values that must occupy one of these two cells

                        for (int option = 0; option <= size; option++) {
                            if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                                count++;
                            }
                        }

                        if (count == rowsRequired) {
                            for (int option = 1; option <= size; option++) {
                                // Exist somewhere else or not in all cells
                                if (valuesElsewhere[option] > 0 || valuesElsewhereCalculated[option] == 0) {
                                    for (int m = 0; m < rowsRequired; m++) {
                                        setIfPossible(p, column, rows[m], option, 0);
                                    }
                                }
                            }
                        }
                    } else if (valuesPresentTogether < rowsRequired && rowsRequired > 2) { // Cells must be split between thease x cells
                        int totalValuesBetweenCells = 0;
                        int valuesUsedInCells[size + 1] = {0};

                        for (int option = 1; option <= size; option++) {
                            for (int m = 0; m < rowsRequired; m++) {
                                if (p[column][rows[m]][option] == 1) {
                                    valuesUsedInCells[option] = 1;
                                    totalValuesBetweenCells++;
                                    break;
                                }
                            }
                        }



                        if (totalValuesBetweenCells == rowsRequired) {
                            int rowsToEliminate[size];

                            for (int m = 0; m < size; m++) {
                                rowsToEliminate[m] = 1;
                            }

                            for (int m = 0; m < rowsRequired; m++) {
                                rowsToEliminate[rows[m]] = 0;
                            }

                            for (int row = 0; row < size; row++) {
                                for (int option = 0; option < size; option++) {
                                    if (valuesUsedInCells[option] == 1 && rowsToEliminate[row] == 1) {
                                        setIfPossible(p, column, row, option, 0);
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
                                        eliminatePossibleFromBlockExcept(p, columnA, rowA, option);
                                        setIfPossible(p, columnB, rowB, option, 1);
                                    }
                                }
                            } else if (valuesPresentTogether >= 2) { //If values share 2 or more values but 2 of those values can only exist in that one place
                                int count = 0; //The number of values that must occupy one of thease two cells

                                for (int option = 0; option <= size; option++) {
                                    if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                                        count++;
                                    }
                                }

                                if (count == 2) {
                                    for (int option = 1; option <= size; option++) {
                                        // Exist somewhere else or not in all cells
                                        if (valuesElsewhere[option] > 0 || valuesElsewhereCalculated[option] == 0) {
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