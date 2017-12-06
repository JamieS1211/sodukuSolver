//
// Created by Jamie on 27/10/2017.
//
#include <stdio.h>
#include <math.h>
#include "global.h"
#include "cellFunctions.h"

extern int steps;

//TODO generalise for higher order
void solveXWing(int ***p) {
    /*
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

                                        eliminatePossibleFromRowExcept(p, columnA, rowA, option);
                                        setIfPossible(p, columnB, rowA, option, 1);

                                        eliminatePossibleFromRowExcept(p, columnA, rowB, option);
                                        setIfPossible(p, columnB, rowB, option, 1);
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

                                        eliminatePossibleFromColumnExcept(p, columnA, rowA, option);
                                        setIfPossible(p, columnA, rowB, option, 1);

                                        eliminatePossibleFromColumnExcept(p, columnB, rowA, option);
                                        setIfPossible(p, columnB, rowB, option, 1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }*/
}