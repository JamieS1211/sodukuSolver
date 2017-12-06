//
// Created by Jamie on 09/11/2017.
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "global.h"
#include "cellFunctions.h"

/**
 * Prints the sudoku leaving spaces for unknown numbers
 *
 * @param p             Sudoku pointer
 */
void printSudoku(int ***p) {
    int maxDigitsInNumber = floor(log10(abs(size))) + 1;

    Cell cell;
    int value;

    for (int row = 0; row < size; row ++){
        cell.row = row;

        if (row % sizeRoot == 0) {
            printSolidLine(maxDigitsInNumber);
        } else {
            printBrokenLine(maxDigitsInNumber);
        }

        for (int column = 0; column < size; column++) {
            cell.column = column;

            if (column % sizeRoot == 0) {
                printf("¦¦");
            } else {
                printf("¦");
            }

            value = findFinalCellValue(p, cell);

            if (value == 0) {
                printf("  ");
                for (int i = 0; i < maxDigitsInNumber; i ++) {
                    printf(" ");
                }
            } else {
                printf(" %i ", value);
                int extraDigitSpace = maxDigitsInNumber - (floor(log10(abs(value))) + 1);
                for (int i = 0; i < extraDigitSpace; i++) {
                    printf(" ");
                }
            }
        }
        printf("¦¦ \n");
    }

    printSolidLine(maxDigitsInNumber);
    printf("\n\n");
}

/**
 * Prints the sudoku with current suggestions shown where number is not known
 *
 * @param p             Sudoku pointer
 */
void printSudokuWithSuggestions(int ***p) {
    int maxDigitsInNumber = floor(log10(abs(size))) + 1;

    Cell cell;

    for (int row = 0; row < size; row ++) {
        cell.row = row;

        if (row % sizeRoot == 0) {
            printFullSolidLine(maxDigitsInNumber);
        } else {
            printFullBrokenLine(maxDigitsInNumber);
        }

        for (int commentRow = 0; commentRow < sizeRoot; commentRow++) {

            for (int column = 0; column < size; column++) {
                cell.column = column;

                int cellValue = findFinalCellValue(p, cell);

                if (column % sizeRoot == 0) {
                    printf("¦¦");
                } else {
                    printf("¦");
                }

                for (int commentColumn = 0; commentColumn < sizeRoot; commentColumn++) {

                    int valueToCheck = 1 + (sizeRoot * commentRow) + commentColumn;

                    if (cellValue == 0) {
                        if (p[column][row][valueToCheck] == 1) {
                            printf(" %i ", valueToCheck);
                            int extraDigitSpace = maxDigitsInNumber - (floor(log10(abs(valueToCheck))) + 1);
                            for (int i = 0; i < extraDigitSpace; i++) {
                                printf(" ");
                            }
                        } else {
                            printf("  ");
                            for (int i = 0; i < maxDigitsInNumber; i ++) {
                                printf(" ");
                            }
                        }
                    } else {
                        if (((1 + size) / 2) == valueToCheck) {
                            printf(" %i ", cellValue);
                            int extraDigitSpace = maxDigitsInNumber - (floor(log10(abs(cellValue))) + 1);
                            for (int i = 0; i < extraDigitSpace; i++) {
                                printf(" ");
                            }
                        } else {
                            printf("  ");
                            for (int i = 0; i < maxDigitsInNumber; i ++) {
                                printf(" ");
                            }
                        }
                    }
                }

            }
            printf("¦¦\n");
        }
    }

    printFullSolidLine(maxDigitsInNumber);
    printf("\n\n");
}

/**
 * Prints long solid line for printing sudoku with suggestions
 *
 * @param maxDigitsInNumber        Maximum amount of digits for a number in this sudoku
 */
void printFullSolidLine(int maxDigitsInNumber) {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("==");
            } else {
                printf("=");
            }

            for (int k = 0; k < size; k++) {
                for (int l = 0; l < maxDigitsInNumber; l++) {
                    printf("=");
                }
            }
        }
    }
    printf("==\n");
}

/**
 * Prints long dashed line for printing sudoku with suggestions
 *
 * @param maxDigitsInNumber        Maximum amount of digits for a number in this sudoku
 */
void printFullBrokenLine(int maxDigitsInNumber) {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("¦¦");
            } else {
                printf("+");
            }

            for (int k = 0; k < size; k++) {
                for (int l = 0; l < maxDigitsInNumber; l++) {
                    printf("-");
                }
            }
        }
    }
    printf("¦¦\n");
}

/**
 * Prints solid line for printing sudoku
 *
 * @param maxDigitsInNumber        Maximum amount of digits for a number in this sudoku
 */
void printSolidLine(int maxDigitsInNumber) {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("==");
            } else {
                printf("=");
            }

            printf("==");
            // For every digit make a space
            for (int k = 0; k < maxDigitsInNumber; k++) {
                printf("=");
            }
        }
    }
    printf("==\n");
}

/**
 * Prints dashed line for printing sudoku
 *
 * @param maxDigitsInNumber        Maximum amount of digits for a number in this sudoku
 */
void printBrokenLine(int maxDigitsInNumber) {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("¦¦");
            } else {
                printf("+");
            }

            printf("--");
            // For every digit make a space
            for (int k = 0; k < maxDigitsInNumber; k++) {
                printf("-");
            }
        }
    }
    printf("¦¦\n");
}