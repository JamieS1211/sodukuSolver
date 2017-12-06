//
// Created by Jamie on 09/11/2017.
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "global.h"
#include "cellFunctions.h"

void printSudoku(int ***p) {
    int maxDigitLength = floor(log10(abs(size))) + 1;

    Cell cell;
    int value;

    for (int row = 0; row < size; row ++){
        cell.row = row;

        if (row % sizeRoot == 0) {
            printSolidLine(maxDigitLength);
        } else {
            printBrokenLine(maxDigitLength);
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
                for (int i = 0; i < maxDigitLength; i ++) {
                    printf(" ");
                }
            } else {
                printf(" %i ", value);
                int extraDigitSpace = maxDigitLength - (floor(log10(abs(value))) + 1);
                for (int i = 0; i < extraDigitSpace; i++) {
                    printf(" ");
                }
            }
        }
        printf("¦¦ \n");
    }

    printSolidLine(maxDigitLength);
    printf("\n\n");
}


void printSudokuWithSuggestions(int ***p) {
    int maxDigitLength = floor(log10(abs(size))) + 1;

    Cell cell;

    for (int row = 0; row < size; row ++) {
        cell.row = row;

        if (row % sizeRoot == 0) {
            printFullSolidLine(maxDigitLength);
        } else {
            printFullBrokenLine(maxDigitLength);
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
                            int extraDigitSpace = maxDigitLength - (floor(log10(abs(valueToCheck))) + 1);
                            for (int i = 0; i < extraDigitSpace; i++) {
                                printf(" ");
                            }
                        } else {
                            printf("  ");
                            for (int i = 0; i < maxDigitLength; i ++) {
                                printf(" ");
                            }
                        }
                    } else {
                        if (((1 + size) / 2) == valueToCheck) {
                            printf(" %i ", cellValue);
                            int extraDigitSpace = maxDigitLength - (floor(log10(abs(cellValue))) + 1);
                            for (int i = 0; i < extraDigitSpace; i++) {
                                printf(" ");
                            }
                        } else {
                            printf("  ");
                            for (int i = 0; i < maxDigitLength; i ++) {
                                printf(" ");
                            }
                        }
                    }
                }

            }
            printf("¦¦\n");
        }
    }

    printFullSolidLine(maxDigitLength);
    printf("\n\n");
}

void printFullSolidLine(int maxDigitLength) {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("==");
            } else {
                printf("=");
            }

            for (int k = 0; k < size; k++) {
                for (int l = 0; l < maxDigitLength; l++) {
                    printf("=");
                }
            }
        }
    }
    printf("==\n");
}

void printFullBrokenLine(int maxDigitLength) {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("¦¦");
            } else {
                printf("+");
            }

            for (int k = 0; k < size; k++) {
                for (int l = 0; l < maxDigitLength; l++) {
                    printf("-");
                }
            }
        }
    }
    printf("¦¦\n");
}



void printSolidLine(int maxDigitLength) {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("==");
            } else {
                printf("=");
            }

            printf("==");
            // For every digit make a space
            for (int k = 0; k < maxDigitLength; k++) {
                printf("=");
            }
        }
    }
    printf("==\n");
}

void printBrokenLine(int maxDigitLength) {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("¦¦");
            } else {
                printf("+");
            }

            printf("--");
            // For every digit make a space
            for (int k = 0; k < maxDigitLength; k++) {
                printf("-");
            }
        }
    }
    printf("¦¦\n");
}