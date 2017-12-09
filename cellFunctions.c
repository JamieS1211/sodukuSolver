//
// Created by Jamie on 05/12/2017.
//
#include <stdio.h>
#include "global.h"
#include "dangerousCellFunctions.h"

extern int steps;


/**
 * Returns if a cell can be written to or not
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @return              If cell can be written to
 */
int canCellWright(int ***p, Cell cell) {
    return p[cell.column][cell.row][0] == 0;
}

/**
 * Initially sets up a cell to be calculated
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 */
void setInitialUnknown(int ***p, Cell cell) {
    p[cell.column][cell.row][0] = -1;
}

/**
 * Sets all values of the given cell to possible
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 */
void setupSuggestions(int ***p, Cell cell) {

    if (p[cell.column][cell.row][0] == -1) {

        p[cell.column][cell.row][0] = 0;
        for (int i = 1; i <= size; i++) {
            p[cell.column][cell.row][i] = 1;
        }
    }
}

/**
 * Sets cell to initial value with wright protection
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @param number        Cell value
 */
void setInitialValue(int ***p, Cell cell, int number) {
    p[cell.column][cell.row][0] = 1; // Wright protect 1= setup value 2= final calculated value
    p[cell.column][cell.row][number] = 1;
}

/**
 * Sets cell to final calculated value with wright protection
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @param number        Cell value
 */
void setFinalCalculatedValue(int ***p, Cell cell, int number) {
    clearCellOfOtherValues(p, cell, number);
    clearRegionOfFinalValue(p, cell, number);

    if (p[cell.column][cell.row][0] != 1) {
        p[cell.column][cell.row][0] = 2; // Wright protect 1= setup value 2= final calculated value
    }
    p[cell.column][cell.row][number] = 1;

    if (debug) {
        printf("Set cell %i %i to final value of %i\n", cell.column, cell.row, number);
    }
}

/**
 * Clears the cell of other value
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @param number        Cell value
 */
void clearCellOfOtherValues(int ***p, Cell cell, int number) {
    for (int i = 1; i <= size; i++) {
        if (i != number) {
            eliminatePossibleFromCell(p, cell, i);
        }
    }
}

/**
 * Clears the column, row and block the cell is in of the number (should only be used on final values)
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @param number        Cell value
 */
void clearRegionOfFinalValue(int ***p, Cell cell, int number) {
    Block block;
    setBlockForCell(&block, cell);


    eliminatePossibleFromColumn(p, cell.column, number);
    eliminatePossibleFromRow(p, cell.row, number);
    eliminatePossibleFromBlock(p, block, number);
}

/**
 * Updates a block with the block column and row values using a pointer to the block and a cell in the block
 *
 * @param block         Block pointer
 * @param cell          Cell in block
 */
void setBlockForCell(Block *block, Cell cell) {
    block->blockColumn = (cell.column - (cell.column % sizeRoot)) / sizeRoot;
    block->blockRow = (cell.row - (cell.row % sizeRoot)) / sizeRoot;
}

/**
 * Removes a value as being suggested in a cell
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @param number        Number altering
 */
void eliminatePossibleFromCell(int ***p, Cell cell, int number) {
    if (canCellWright(p, cell)) {
        if (p[cell.column][cell.row][number] == 1) {
            p[cell.column][cell.row][number] = 0;

            if (debug) {
                printf("Removed suggestion of %i in cell %i %i \n", number, cell.column, cell.row);
            }

            suggestionsInCell(p, cell);
        }
    }

}

/**
 * Returns the number of suggestions for a cell. If the cell has a final value returns -1 and sets wright protection;
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @return              Number of suggestions for the cell
 */
int suggestionsInCell(int ***p, Cell cell) {
    if (canCellWright(p, cell)) {
        int suggestions = 0;
        int lastSuggestion = 0;

        for (int i = 1; i <= size; i++) {
            if (p[cell.column][cell.row][i] == 1) {
                suggestions++;
                lastSuggestion = i;
            }
        }

        if (suggestions == 1) {
            setFinalCalculatedValue(p, cell, lastSuggestion);

            steps++;
            if (outputSolveSteps) {
                printf("[Step %i]: Found and solved naked single at cell %i, %i. Setting value to %i \n", steps, cell.column, cell.row, lastSuggestion);
            }
        }

        return suggestions;
    }

    return -1;
}

/**
 * Returns the number of cells in the column with this suggestion. Returns -1 if a cell has this value.
 *
 * @param p             Sudoku pointer
 * @param column        Column to check
 * @param number        Number to check
 * @return              Number of cells in row with this suggestion
 */
int cellsWithSuggestionInColumn(int ***p, int column, int number) {
    Cell cell;
    cell.column = column;
    int possiblePlaces = 0;

    for (int row = 0; row < size; row++) {
        cell.row = row;

        if (findFinalCellValue(p, cell) == number) {
            return -1;
        } else if (p[column][row][number] == 1) {
            possiblePlaces++;
        }
    }

    return possiblePlaces;
}

/**
 * Returns the number of cells in the row with this suggestion. Returns -1 if a cell has this value.
 *
 * @param p             Sudoku pointer
 * @param row           Row to check
 * @param number        Number to check
 * @return              Number of cells in row with this suggestion
 */
int cellsWithSuggestionInRow(int ***p, int row, int number) {
    Cell cell;
    cell.row = row;
    int possiblePlaces = 0;

    for (int column = 0; column < size; column++) {
        cell.column = column;

        if (findFinalCellValue(p, cell) == number) {
            return -1;
        } else if (p[column][row][number] == 1) {
            possiblePlaces++;
        }
    }

    return possiblePlaces;
}

/**
 * Returns the number of cells in the block with this suggestion. Returns -1 if a cell has this value.
 *
 * @param p             Sudoku pointer
 * @param block         Block
 * @param number        Number to check
 * @return              Number of cells in block with this suggestion
 */
int cellsWithSuggestionInBlock(int ***p, Block block, int number) {
    Cell cell;
    int possiblePlaces = 0;

    for (int columnOffset = 0; columnOffset < sizeRoot; columnOffset++) {
        cell.column = (block.blockColumn * sizeRoot) + columnOffset;

        for (int rowOffset = 0; rowOffset < sizeRoot; rowOffset++) {
            cell.row = (block.blockRow * sizeRoot) + rowOffset;

            if (findFinalCellValue(p, cell) == number) {
                return -1;
            } else if (p[cell.column][cell.row][number] == 1) {
                possiblePlaces++;
            }
        }
    }

    return possiblePlaces;
}

/**
 * Returns the value of a cell if known and 0 if cell is not yet known.
 *
 * @param p         Sudoku pointer
 * @param cell      Cell
 * @return          Value of this cell
 */
int findFinalCellValue(int ***p, Cell cell) {

    if (p[cell.column][cell.row][0] != 0) {
        for (int option = 1; option <= size; option++) {
            if (p[cell.column][cell.row][option] == 1) {
                return option;
            }
        }
    }

    return 0;
}