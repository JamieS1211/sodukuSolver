//
// Created by Jamie on 27/10/2017.
//
#include <stdio.h>
#include "global.h"
#include "cellFunctions.h"

extern int steps;

/**
 * Find and solve all X-Wing patterns in sudoku
 *
 * @param p             Sudoku pointer
 * @return              Number of changes made
 */
int solveXWing(int ***p) {
    int startSteps = steps;
    Cell cells[4];

    for (int columnA = 0; columnA < size; columnA++) {
        for (int columnB = 0; columnB < size; columnB++) {

            if (columnA < columnB) {
                cells[0].column = columnA;
                cells[1].column = columnA;
                cells[2].column = columnB;
                cells[3].column = columnB;

                for (int rowA = 0; rowA < size; rowA++) {
                    for (int rowB = 0; rowB < size; rowB++) {

                        if (rowA < rowB) {
                            cells[0].row = rowA;
                            cells[1].row = rowB;
                            cells[2].row = rowA;
                            cells[3].row = rowB;

                            // If all 4 cells are not yet solved
                            if (findFinalCellValue(p, cells[0]) == 0 && findFinalCellValue(p, cells[1]) == 0
                                && findFinalCellValue(p, cells[2]) == 0 &&
                                findFinalCellValue(p, cells[3]) == 0) {


                                // for every option if both cells have that value possible add to values that match.
                                int madeChange = 0;
                                for (int option = 1; option <= size; option++) {
                                    if ((p[cells[0].column][cells[0].row][option] == 1) && (p[cells[1].column][cells[1].row][option] == 1) &&
                                        cellsWithSuggestionInColumn(p, cells[0].column, option) == 2
                                        && (p[cells[2].column][cells[2].row][option] == 1) && (p[cells[3].column][cells[3].row][option] == 1) &&
                                        cellsWithSuggestionInColumn(p, cells[2].column, option) == 2) {

                                        int columnsToClearInGivenRows[size];

                                        for (int i = 0; i < size; i++) {
                                            columnsToClearInGivenRows[i] = 1;
                                        }

                                        for (int i = 0; i < 2; i++) {
                                            columnsToClearInGivenRows[cells[i].row] = 0;
                                        }

                                        Cell cell;
                                        for (int row = 0; row < 2; row++) {
                                            cell.row = cells[row].row;
                                            for (int column = 0; column < size; column++) {
                                                cell.column = column;

                                                if (canCellWright(p, cell)) {
                                                    if (p[cell.column][cell.row][option] == 1) {
                                                        if (!madeChange) {
                                                            madeChange = 1;
                                                            steps++;

                                                            if (outputSolveSteps) {
                                                                printf("[Step %i]: Found X-Wing in cells", steps);

                                                                for (int j = 0; j < 4; j++) {
                                                                    printf(" %i, %i ", cells[j].column, cells[j].row);
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
            }
        }
    }


    for (int rowA = 0; rowA < size; rowA++) {
        for (int rowB = 0; rowB < size; rowB++) {

            if (rowA < rowB) {
                cells[0].row = rowA;
                cells[1].row = rowB;
                cells[2].row = rowA;
                cells[3].row = rowB;

                for (int columnA = 0; columnA < size; columnA++) {
                    for (int columnB = 0; columnB < size; columnB++) {

                        if (columnA < columnB) {
                            cells[0].column = columnA;
                            cells[1].column = columnA;
                            cells[2].column = columnB;
                            cells[3].column = columnB;

                            // If all 4 cells are not yet solved
                            if (findFinalCellValue(p, cells[0]) == 0 && findFinalCellValue(p, cells[1]) == 0
                                && findFinalCellValue(p, cells[2]) == 0 &&
                                findFinalCellValue(p, cells[3]) == 0) {


                                // for every option if both cells have that value possible add to values that match.
                                int madeChange = 0;
                                for (int option = 1; option <= size; option++) {
                                    if ((p[cells[0].column][cells[0].row][option] == 1) &&
                                        (p[cells[2].column][cells[2].row][option] == 1) &&
                                        cellsWithSuggestionInRow(p, cells[0].row, option) == 2
                                        && (p[cells[1].column][cells[1].row][option] == 1) &&
                                        (p[cells[3].row][cells[3].row][option] == 1) &&
                                        cellsWithSuggestionInRow(p, cells[1].row, option) == 2) {

                                        int rowsToClearInGivenColumns[size];

                                        for (int i = 0; i < size; i++) {
                                            rowsToClearInGivenColumns[i] = 1;
                                        }

                                        for (int i = 0; i < 2; i++) {
                                            rowsToClearInGivenColumns[cells[i].row] = 0;
                                        }

                                        Cell cell;
                                        for (int column = 0; column < 2; column++) {
                                            cell.column = cells[column].column;
                                            for (int row = 0; row < size; row++) {
                                                cell.row = row;

                                                if (canCellWright(p, cell)) {
                                                    if (p[cell.column][cell.row][option] == 1) {
                                                        if (!madeChange) {
                                                            madeChange = 1;
                                                            steps++;

                                                            if (outputSolveSteps) {
                                                                printf("[Step %i]: Found X-Wing in cells", steps);

                                                                for (int j = 0; j < 4; j++) {
                                                                    printf(" %i, %i ", cells[j].column, cells[j].row);
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
            }
        }
    }

    return steps - startSteps;
}