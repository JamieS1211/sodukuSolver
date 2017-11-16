//
// Created by Jamie on 27/10/2017.
//
#include <stdio.h>
#include <math.h>
#include "config.h"
#include "main.h"

typedef struct cell {
    int row;
    int column;
} Cell;

void solveHighOrderLinkedCells(int ***p) {
    int maxOrder = size / 2;

    for (int column = 0; column < size; column++) {
       solveHighOrderCellsInColumn(p, column, maxOrder);
    }

    for (int row = 0; row < size; row++) {
        solveHighOrderCellsInRow(p, row, maxOrder);
    }

    for (int leftColumn = 0; leftColumn < size; leftColumn = leftColumn + sizeRoot) {
        for (int topRow = 0; topRow < size; topRow = topRow + sizeRoot) {
            // For every top left cell of a block
            solveHighOrderCellsInBlock(p, leftColumn, topRow, maxOrder);
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

            for (int j = 0; j < columnsRequired - 1; j++) {

                if (columns[j] >= columns[j + 1]) {
                    correctOrder = 0;
                    break;
                }

                if (columns[j] == columns[j + 1]) {
                    correctOrder = 0;
                    break;
                }
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

            for (int j = 0; j < rowsRequired - 1; j++) {

                if (rows[j] >= rows[j + 1]) {
                    correctOrder = 0;
                    break;
                }

                if (rows[j] == rows[j + 1]) {
                    correctOrder = 0;
                    break;
                }
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

void solveHighOrderCellsInBlock(int ***p, int columnStart, int rowStart, int maxOrder) {
    int maximumToCheck = size / 2;
    if (maxOrder < maximumToCheck) {
        maximumToCheck = maxOrder;
    }

    Cell cells[size];
    int cellsRequired;

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

            cells[j].column = positionValue % sizeRoot;
            cells[j].row = (positionValue - cells[j].column) / sizeRoot;
        }

        for (int j = size - 1; j >= 0; j--) {
            if (cells[j].column > 0 || cells[j].row > 0) {
                cellsRequired = j + 1;
                break;
            }
        }

        if (cellsRequired > 1) {
            for (int j = 0; j < cellsRequired - 1; j++) {

                if (cells[j].column >= cells[j + 1].column || cells[j].row >= cells[j + 1].row) {
                    correctOrder = 0;
                }

                if (cells[j].column == cells[j + 1].column && cells[j].row == cells[j].row) {
                    correctOrder = 0;
                }
            }

            // If row combination has not been used before and is valid.
            if (correctOrder) {
                int solvedCell = 0;
                for (int l = 0; l < cellsRequired; l++) {
                    int value = findFinalCellValue(p, cells[l].column, cells[l].row);
                    if (value != 0) {
                        solvedCell = 1;
                        eliminatePossibleFromBlockExcept(p, cells[l].column, cells[l].row, value);
                        eliminatePossibleFromBlockExcept(p, cells[l].column, cells[l].row, value);
                        eliminatePossibleFromBlockExcept(p, cells[l].column, cells[l].row, value);
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

                        for (int m = 0; m < cellsRequired; m++) {
                            if (p[cells[m].column][cells[m].row][option] == 0) {
                                valuePresentInAllCells = 0;
                                break;
                            }
                        }

                        for (int m = 0; m < cellsRequired; m++) {
                            if (p[cells[m].column][cells[m].row][option] == 1) {
                                valueAbsentInAllCells = 0;
                                break;
                            }
                        }


                        if (valuePresentInAllCells) {
                            valuesPresentTogether++;
                            valuesElsewhereCalculated[option] = 1;

                            if (cellsWithSuggestionInBlock(p, columnStart, rowStart, option) > cellsRequired) {
                                valuesElsewhere[option] = 1;
                            } else if (cellsWithSuggestionInBlock(p, columnStart, rowStart, option) == -1) {
                                valuesElsewhere[option] = -1;
                            }
                        } else if (valueAbsentInAllCells) {
                            valuesAbsentTogether++;
                        }
                    }


                    // Cells are are identical and must be split between these x cells
                    if (valuesPresentTogether == cellsRequired && valuesAbsentTogether == size - cellsRequired) {
                        for (int option = 1; option <= size; option++) {

                            int valuePresentInAllCells = 1;

                            for (int m = 0; m < cellsRequired; m++) {
                                if (p[cells[m].column][cells[m].row][option] == 0) {
                                    valuePresentInAllCells = 0;
                                    break;
                                }
                            }

                            if (valuePresentInAllCells) {
                                eliminatePossibleFromBlock(p, columnStart, rowStart, option);

                                for (int m = 0; m < cellsRequired; m++) {
                                    setIfPossible(p, cells[m].column, cells[m].row, option, 1);
                                }
                            }
                        }
                    } else if (valuesPresentTogether >= cellsRequired) { // Other suggestions in cell hiding
                        int count = 0; //The number of values that must occupy one of these two cells

                        for (int option = 0; option <= size; option++) {
                            if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                                count++;
                            }
                        }

                        if (count == cellsRequired) {
                            for (int option = 1; option <= size; option++) {
                                // Exist somewhere else or not in all cells
                                if (valuesElsewhere[option] > 0 || valuesElsewhereCalculated[option] == 0) {
                                    for (int m = 0; m < cellsRequired; m++) {
                                        setIfPossible(p, cells[m].column, cells[m].row, option, 0);
                                    }
                                }
                            }
                        }
                    } else if (valuesPresentTogether < cellsRequired && cellsRequired > 2) { // Cells must be split between thease x cells
                        int totalValuesBetweenCells = 0;
                        int valuesUsedInCells[size + 1] = {0};

                        for (int option = 1; option <= size; option++) {
                            for (int m = 0; m < cellsRequired; m++) {
                                if (p[cells[m].column][cells[m].row][option] == 1) {
                                    valuesUsedInCells[option] = 1;
                                    totalValuesBetweenCells++;
                                    break;
                                }
                            }
                        }



                        if (totalValuesBetweenCells == cellsRequired) {
                            int cellsToEliminate[size];

                            for (int m = 0; m < size; m++) {
                                cellsToEliminate[m] = 1;
                            }

                            for (int m = 0; m < cellsRequired; m++) {
                                cellsToEliminate[(cells[m].column * 3) + cells[m].row] = 0;
                            }

                            for (int position = 0; position < size; position++) {
                                for (int option = 0; option < size; option++) {
                                    if (valuesUsedInCells[option] == 1 && cellsToEliminate[position] == 1) {
                                        setIfPossible(p, (position - (position % 3)) / 3, position % 3, option, 0);
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