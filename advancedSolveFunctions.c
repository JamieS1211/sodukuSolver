//
// Created by Jamie on 27/10/2017.
//
#include <stdio.h>
#include <math.h>
#include "global.h"
#include "cellFunctions.h"

extern int steps;

/**
 * Organises runs high order column, row and block solving for the given order and returns the number of changes made
 *
 * @param p             Sudoku pointer
 * @param order         Order of solutions to look for
 * @return              Numnber of changes made
 */
int solveHighOrderLinkedCells(int ***p, int order) {
    int changes = 0;

    if (order > (size / 2)) {
        if (debug) {
            printf("[WARNING] High order lined cells was attempted at an above maximum order. Request canceled.");
        }

        return 0;
    }

    for (int row = 0; row < size; row++) {
        changes += solveHighOrderCellsInRow(p, row, order);
    }

    for (int column = 0; column < size; column++) {
       changes += solveHighOrderCellsInColumn(p, column, order);
    }

    Block block;
    for (int blockColumn = 0; blockColumn < sizeRoot; blockColumn++) {
        block.blockColumn = blockColumn;

        for (int blockRow = 0; blockRow < sizeRoot; blockRow++) {
            block.blockRow = blockRow;

            changes += solveHighOrderCellsInBlock(p, block, order);
        }
    }

    return changes;
}

/**
 * Solves high order linked cells in the given row for the given order and returns the number of changes made
 *
 * @param p             Sudoku pointer
 * @param row           Row to search
 * @param order         Order of solutions
 * @return              Number of changes made
 */
int solveHighOrderCellsInRow(int ***p, int row, int order) {
    int startSteps = steps;

    Cell cell;

    cell.row = row;

    int columns[order];
    int columnsRequired = 1;

    for (int i = 0; i < pow(size, order); i++) {

        int previousColumnValue = size;
        int count = i;

        int correctOrder = 1;

        for (int j = order - 1; j >= 0; j--) {
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

        for (int j = order - 1; j >= 0; j--) {
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
            if (correctOrder && columnsRequired == order) {
                int solvedCell = 0;
                for (int j = 0; j < columnsRequired; j++) {
                    cell.column = columns[j];

                    if (findFinalCellValue(p, cell) != 0) {
                        solvedCell = 1;
                    }
                }

                if (solvedCell == 0) {

                    int valuesAbsentTogether = 0;
                    int valuesPresentTogether = 0;
                    int valuesElsewhereCalculated[size + 1] = {-1};
                    int valuesElsewhere[size + 1] = {-1};

                    for (int option = 1; option <= size; option++) {
                        valuesElsewhereCalculated[option] = 0;
                        valuesElsewhere[option] = 0;
                    }


                    // For every option if both cells have that value possible add to values that match.
                    for (int option = 1; option <= size; option++) {

                        int valuePresentInAllCells = 1;
                        int valueAbsentInAllCells = 1;

                        for (int j = 0; j < columnsRequired; j++) {
                            if (p[columns[j]][row][option] == 0) {
                                valuePresentInAllCells = 0;
                                break;
                            }
                        }

                        for (int j = 0; j < columnsRequired; j++) {
                            if (p[columns[j]][row][option] == 1) {
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

                    if (valuesPresentTogether == columnsRequired && valuesAbsentTogether == size - columnsRequired) { // Naked linked cells
                        int madeChange = 0;

                        for (int option = 1; option <= size; option++) {

                            int valuePresentInAllCells = 1;

                            for (int j = 0; j < columnsRequired; j++) {
                                if (p[columns[j]][row][option] == 0) {
                                    valuePresentInAllCells = 0;
                                    break;
                                }
                            }

                            if (valuePresentInAllCells) {

                                for (int column = 0; column < size; column++) {
                                    cell.column = column;

                                    if (canCellWright(p, cell)) {
                                        int clearColumn = 1;
                                        for (int j = 0; j < columnsRequired; j++) {
                                            if (columns[j] == column) {
                                                clearColumn = 0;
                                            }
                                        }

                                        if (clearColumn) {
                                            if (!madeChange) {
                                                madeChange = 1;
                                                steps++;

                                                if (outputSolveSteps) {
                                                    printf("[Step %i]: Found naked linked cell of order %i across row in cells",
                                                           steps, order);

                                                    for (int j = 0; j < columnsRequired; j++) {
                                                        printf(" %i, %i ", columns[j], row);
                                                    }

                                                    printf("\n");
                                                }
                                            }

                                            eliminatePossibleFromCell(p, cell, option);
                                        }
                                    }
                                }
                            }
                        }
                    } else if (valuesPresentTogether >= columnsRequired) { // Hidden linked cells out n values can only exist here so clear other values from their cells
                        int count = 0; //The number of values that must occupy one of these two cells

                        for (int option = 0; option <= size; option++) {
                            if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                                count++;
                            }
                        }

                        int madeChange = 0;

                        if (count == columnsRequired) {
                            for (int option = 1; option <= size; option++) {
                                // Exist somewhere else or not in all cells
                                if (valuesElsewhere[option] > 0 || valuesElsewhereCalculated[option] == 0) {
                                    // Remove suggestions for other values in these cells
                                    for (int j = 0; j < columnsRequired; j++) {
                                        cell.column = columns[j];

                                        if (canCellWright(p, cell)) {
                                            if (!madeChange) {
                                                madeChange = 1;
                                                steps++;

                                                if (outputSolveSteps) {
                                                    printf("[Step %i]: Found linked cell of order %i across row in cells", steps, order);

                                                    for (int j = 0; j < columnsRequired; j++) {
                                                        printf(" %i, %i ", columns[j], row);
                                                    }

                                                    printf("\n");
                                                }
                                            }

                                            eliminatePossibleFromCell(p, cell, option);
                                        }
                                    }
                                }
                            }
                        }
                    } else if (valuesPresentTogether < columnsRequired && columnsRequired > 2) { // Hidden linked cells must be split between these n cells
                        int totalValuesBetweenCells = 0;
                        int valuesUsedInCells[size + 1] = {0};

                        for (int option = 1; option <= size; option++) {
                            for (int j = 0; j < columnsRequired; j++) {
                                if (p[columns[j]][row][option] == 1) {
                                    valuesUsedInCells[option] = 1;
                                    totalValuesBetweenCells++;
                                    break;
                                }
                            }
                        }

                        int madeChange = 0;

                        if (totalValuesBetweenCells == columnsRequired) {
                            int columnsToEliminate[size];

                            for (int j = 0; j < size; j++) {
                                columnsToEliminate[j] = 1;
                            }

                            for (int j = 0; j < columnsRequired; j++) {
                                columnsToEliminate[columns[j]] = 0;
                            }

                            for (int column = 0; column < size; column++) {
                                cell.column = column;

                                if (canCellWright(p, cell)) {
                                    for (int option = 0; option < size; option++) {
                                        if (valuesUsedInCells[option] == 1 && columnsToEliminate[column] == 1) {

                                            if (!madeChange) {
                                                madeChange = 1;
                                                steps++;

                                                if (outputSolveSteps) {
                                                    printf("[Step %i]: Found linked cell of order %i across row in cells",
                                                           steps, order);

                                                    for (int j = 0; j < columnsRequired; j++) {
                                                        printf(" %i, %i ", columns[j], row);
                                                    }

                                                    printf("\n");
                                                }
                                            }

                                            eliminatePossibleFromCell(p, cell, option);
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

    return steps - startSteps;
}

/**
 * Solves high order linked cells in the given column for the given order and returns the number of changes made
 *
 * @param p             Sudoku pointer
 * @param column        Column to search
 * @param order         Order of solutions
 * @return              Number of changes made
 */
int solveHighOrderCellsInColumn(int ***p, int column, int order) {
    int startSteps = steps;

    Cell cell;

    cell.column = column;

    int rows[order];
    int rowsRequired = 1;

    for (int i = 0; i < pow(size, order); i++) {

        int count = i;
        int correctOrder = 1;

        for (int j = order - 1; j >= 0; j--) {
            int positionWorth = pow(size, j);
            int positionValue = 0;

            while (count >= positionWorth) {
                count = count - positionWorth;
                positionValue++;
            }

            rows[j] = positionValue;
        }

        for (int j = order - 1; j >= 0; j--) {
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
            if (correctOrder && rowsRequired == order) {
                int solvedCell = 0;
                for (int j = 0; j < rowsRequired; j++) {
                    cell.row = rows[j];

                    if (findFinalCellValue(p, cell) != 0) {
                        solvedCell = 1;
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

                        for (int j = 0; j < rowsRequired; j++) {
                            if (p[column][rows[j]][option] == 0) {
                                valuePresentInAllCells = 0;
                                break;
                            }
                        }

                        for (int j = 0; j < rowsRequired; j++) {
                            if (p[column][rows[j]][option] == 1) {
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

                    if (valuesPresentTogether == rowsRequired && valuesAbsentTogether == size - rowsRequired) { // Naked linked cells
                        int madeChange = 0;

                        for (int option = 1; option <= size; option++) {

                            int valuePresentInAllCells = 1;

                            for (int j = 0; j < rowsRequired; j++) {
                                if (p[column][rows[j]][option] == 0) {
                                    valuePresentInAllCells = 0;
                                    break;
                                }
                            }

                            if (valuePresentInAllCells) {

                                for (int row = 0; row < size; row++) {
                                    cell.row = row;

                                    if (canCellWright(p, cell)) {
                                        int clearRow = 1;
                                        for (int j = 0; j < rowsRequired; j++) {
                                            if (rows[j] == row) {
                                                clearRow = 0;
                                            }
                                        }

                                        if (clearRow) {
                                            if (!madeChange) {
                                                madeChange = 1;
                                                steps++;

                                                if (outputSolveSteps) {
                                                    printf("[Step %i]: Found naked linked cell of order %i across column in cells",
                                                           steps, order);

                                                    for (int j = 0; j < rowsRequired; j++) {
                                                        printf(" %i, %i ", column, rows[j]);
                                                    }

                                                    printf("\n");
                                                }
                                            }

                                            eliminatePossibleFromCell(p, cell, option);
                                        }
                                    }
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

                        int madeChange = 0;

                        if (count == rowsRequired) {
                            for (int option = 1; option <= size; option++) {
                                // Exist somewhere else or not in all cells
                                if (valuesElsewhere[option] > 0 || valuesElsewhereCalculated[option] == 0) {
                                    // Remove suggestions for other values in these cells
                                    for (int j = 0; j < rowsRequired; j++) {
                                        cell.row = rows[j];

                                        if (canCellWright(p, cell)) {
                                            if (!madeChange) {
                                                madeChange = 1;
                                                steps++;

                                                if (outputSolveSteps) {
                                                    printf("[Step %i]: Found linked cell of order %i across column in cells", steps, order);

                                                    for (int j = 0; j < rowsRequired; j++) {
                                                        printf(" %i, %i ", column, rows[j]);
                                                    }

                                                    printf("\n");
                                                }
                                            }

                                            eliminatePossibleFromCell(p, cell, option);
                                        }
                                    }
                                }
                            }
                        }
                    } else if (valuesPresentTogether < rowsRequired && rowsRequired > 2) { // Cells must be split between thease x cells
                        int totalValuesBetweenCells = 0;
                        int valuesUsedInCells[size + 1] = {0};

                        for (int option = 1; option <= size; option++) {
                            for (int j = 0; j < rowsRequired; j++) {
                                if (p[column][rows[j]][option] == 1) {
                                    valuesUsedInCells[option] = 1;
                                    totalValuesBetweenCells++;
                                    break;
                                }
                            }
                        }

                        int madeChange = 0;

                        if (totalValuesBetweenCells == rowsRequired) {
                            int rowsToEliminate[size];

                            for (int j = 0; j < size; j++) {
                                rowsToEliminate[j] = 1;
                            }

                            for (int j = 0; j < rowsRequired; j++) {
                                rowsToEliminate[rows[j]] = 0;
                            }

                            for (int row = 0; row < size; row++) {
                                cell.row = row;

                                if (canCellWright(p, cell)) {
                                    for (int option = 0; option < size; option++) {
                                        if (valuesUsedInCells[option] == 1 && rowsToEliminate[row] == 1) {

                                            if (!madeChange) {
                                                madeChange = 1;
                                                steps++;

                                                if (outputSolveSteps) {
                                                    printf("[Step %i]: Found linked cell of order %i across column in cells",
                                                           steps, order);

                                                    for (int j = 0; j < rowsRequired; j++) {
                                                        printf(" %i, %i ", column, rows[j]);
                                                    }

                                                    printf("\n");
                                                }
                                            }

                                            eliminatePossibleFromCell(p, cell, option);
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

    return steps - startSteps;
}

/**
 * Solves high order linked cells in the given block for the given order and returns the number of changes made
 *
 * @param p             Sudoku pointer
 * @param block         Block to search
 * @param order         Order of solutions
 * @return              Number of changes made
 */
int solveHighOrderCellsInBlock(int ***p, Block block, int order) {
    int startSteps = steps;

    Cell cell;

    Cell cellsOffsets[order];
    int cellsRequired = 1;

    for (int i = 0; i < pow(size, order); i++) {

        int count = i;
        int correctOrder = 1;

        for (int j = order - 1; j >= 0; j--) {
            int positionWorth = pow(size, j);
            int positionValue = 0;

            while (count >= positionWorth) {
                count = count - positionWorth;
                positionValue++;
            }

            cellsOffsets[j].column = positionValue % sizeRoot;
            cellsOffsets[j].row = (positionValue - cellsOffsets[j].column) / sizeRoot;
        }

        for (int j = order - 1; j >= 0; j--) {
            if (cellsOffsets[j].column > 0 || cellsOffsets[j].row > 0) {
                cellsRequired = j + 1;
                break;
            }
        }

        if (cellsRequired > 1) {
            for (int j = 0; j < cellsRequired - 1; j++) {

                if (cellsOffsets[j].row * sizeRoot + cellsOffsets[j].column >= cellsOffsets[j + 1].row * sizeRoot + cellsOffsets[j + 1].column) {
                    correctOrder = 0;
                }
            }

            if (correctOrder && cellsRequired == order) {
                int solvedCell = 0;
                for (int j = 0; j < cellsRequired; j++) {
                    cell.column = (block.blockColumn * sizeRoot) + cellsOffsets[j].column;
                    cell.row = (block.blockRow * sizeRoot) + cellsOffsets[j].row;

                    int value = findFinalCellValue(p, cell);
                    if (value != 0) {
                        solvedCell = 1;
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

                        for (int j = 0; j < cellsRequired; j++) {
                            if (p[(block.blockColumn * sizeRoot) + cellsOffsets[j].column][(block.blockRow * sizeRoot) + cellsOffsets[j].row][option] == 0) {
                                valuePresentInAllCells = 0;
                                break;
                            }
                        }

                        for (int j = 0; j < cellsRequired; j++) {
                            if (p[(block.blockColumn * sizeRoot) + cellsOffsets[j].column][(block.blockRow * sizeRoot) + cellsOffsets[j].row][option] == 1) {
                                valueAbsentInAllCells = 0;
                                break;
                            }
                        }


                        if (valuePresentInAllCells) {
                            valuesPresentTogether++;
                            valuesElsewhereCalculated[option] = 1;

                            if (cellsWithSuggestionInBlock(p, block, option) > cellsRequired) {
                                valuesElsewhere[option] = 1;
                            } else if (cellsWithSuggestionInBlock(p, block, option) == -1) {
                                valuesElsewhere[option] = -1;
                            }
                        } else if (valueAbsentInAllCells) {
                            valuesAbsentTogether++;
                        }
                    }

                    if (valuesPresentTogether == cellsRequired && valuesAbsentTogether == size - cellsRequired) { // Naked linked cells

                        int madeChange = 0;

                        for (int option = 1; option <= size; option++) {

                            int valuePresentInAllCells = 1;

                            for (int j = 0; j < cellsRequired; j++) {
                                if (p[(block.blockColumn * sizeRoot) + cellsOffsets[j].column][(block.blockRow * sizeRoot) + cellsOffsets[j].row][option] == 0) {
                                    valuePresentInAllCells = 0;
                                    break;
                                }
                            }

                            if (valuePresentInAllCells) {
                                Cell cell;

                                for (int columnOffset = 0; columnOffset < sizeRoot; columnOffset++) {
                                    cell.column = (block.blockColumn * sizeRoot) + columnOffset;

                                    for (int rowOffset = 0; rowOffset < sizeRoot; rowOffset++) {
                                        cell.row = (block.blockRow * sizeRoot) + rowOffset;

                                        if (canCellWright(p, cell)) {
                                            int clearCell = 1;
                                            for (int j = 0; j < cellsRequired; j++) {
                                                if (cellsOffsets[j].column == columnOffset &&
                                                    cellsOffsets[j].row == rowOffset) {
                                                    clearCell = 0;
                                                }

                                                if (clearCell) {
                                                    if (!madeChange) {
                                                        madeChange = 1;
                                                        steps++;

                                                        if (outputSolveSteps) {
                                                            printf("[Step %i]: Found naked linked cell of order %i across block in cells", steps, order);

                                                            for (int j = 0; j < cellsRequired; j++) {
                                                                printf(" %i, %i ", (block.blockColumn * sizeRoot) + cellsOffsets[j].column, (block.blockRow * sizeRoot) + cellsOffsets[j].row);
                                                            }

                                                            printf("\n");
                                                        }
                                                    }

                                                    eliminatePossibleFromCell(p, cell, option);
                                                }
                                            }
                                        }
                                    }
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

                        int madeChange = 0;

                        if (count == cellsRequired) {
                            for (int option = 1; option <= size; option++) {
                                // Exist somewhere else or not in all cells
                                if (valuesElsewhere[option] > 0 || valuesElsewhereCalculated[option] == 0) {
                                    for (int j = 0; j < cellsRequired; j++) {
                                        cell.column = (block.blockColumn * sizeRoot) + cellsOffsets[j].column;
                                        cell.row = (block.blockRow * sizeRoot) + cellsOffsets[j].row;

                                        if (canCellWright(p, cell)) {
                                            if (!madeChange) {
                                                madeChange = 1;
                                                steps++;

                                                if (outputSolveSteps) {
                                                    printf("[Step %i]: Found linked cell of order %i across block in cells", steps, order);

                                                    for (int j = 0; j < cellsRequired; j++) {
                                                        printf(" %i, %i ", (block.blockColumn * sizeRoot) + cellsOffsets[j].column, (block.blockRow * sizeRoot) + cellsOffsets[j].row);
                                                    }

                                                    printf("\n");
                                                }
                                            }

                                            eliminatePossibleFromCell(p, cell, option);
                                        }
                                    }
                                }
                            }
                        }
                    } else if (valuesPresentTogether < cellsRequired && cellsRequired > 2) { // Cells must be split between thease x cells
                        int totalValuesBetweenCells = 0;
                        int valuesUsedInCells[size + 1] = {0};

                        for (int option = 1; option <= size; option++) {
                            for (int j = 0; j < cellsRequired; j++) {
                                if (p[(block.blockColumn * sizeRoot) + cellsOffsets[j].column][(block.blockRow * sizeRoot) + cellsOffsets[j].row][option] == 1) {
                                    valuesUsedInCells[option] = 1;
                                    totalValuesBetweenCells++;
                                    break;
                                }
                            }
                        }

                        int madeChange = 0;

                        if (totalValuesBetweenCells == cellsRequired) {
                            int cellsToEliminate[size];

                            for (int j = 0; j < size; j++) {
                                cellsToEliminate[j] = 1;
                            }

                            for (int j = 0; j < cellsRequired; j++) {
                                cellsToEliminate[(cellsOffsets[j].column * 3) + cellsOffsets[j].row] = 0;
                            }

                            for (int position = 0; position < size; position++) {
                                cell.column = (block.blockColumn * sizeRoot) + ((position - (position % sizeRoot)) / sizeRoot);
                                cell.row = (block.blockRow * sizeRoot) + (position % sizeRoot);


                                if (canCellWright(p, cell)) {
                                    for (int option = 0; option < size; option++) {
                                        if (valuesUsedInCells[option] == 1 && cellsToEliminate[position] == 1) {

                                            if (!madeChange) {
                                                madeChange = 1;
                                                steps++;

                                                if (outputSolveSteps) {
                                                    printf("[Step %i]: Found linked cell of order %i across block in cells", steps, order);

                                                    for (int j = 0; j < cellsRequired; j++) {
                                                        printf(" %i, %i ", (block.blockColumn * sizeRoot) + cellsOffsets[j].column, (block.blockRow * sizeRoot) + cellsOffsets[j].row);
                                                    }

                                                    printf("\n");
                                                }
                                            }

                                            eliminatePossibleFromCell(p, cell, option);
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

    return steps - startSteps;
}