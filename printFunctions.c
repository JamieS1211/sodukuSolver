//
// Created by Jamie on 09/11/2017.
//
#include <stdio.h>
#include "config.h"
#include "main.h"

void printSudoku(int ***p) {
    int value;
    for (int row = 0; row < size; row ++){
        if (row % 3 == 0) {
            printSolidLine();
        } else {
            printBrokenLine();
        }
        for (int column = 0; column < size; column++) {
            if (column % sizeRoot == 0) {
                printf("¦¦");
            } else {
                printf("¦");
            }

            value = findFinalCellValue(p, column, row);

            if (value == 0) {
                printf("   ");
            } else {
                printf(" %i ", value);
            }
        }
        printf("¦¦ \n");
    }

    printSolidLine();
    printf("\n\n");
}


void printSudokuBig(int ***p) {
    for (int row = 0; row < size; row ++) {

        if (row % 3 == 0) {
            printFullSolidLine();
        } else {
            printFullBrokenLine();
        }

        for (int commentRow = 0; commentRow < sizeRoot; commentRow++) {

            for (int column = 0; column < size; column++) {

                int cellValue = findFinalCellValue(p, column, row);

                if (column % sizeRoot == 0) {
                    printf("¦¦");
                } else {
                    printf("¦");
                }

                for (int commentColumn = 0; commentColumn < sizeRoot; commentColumn++) {

                    int valueToCheck = 1 + (sizeRoot * commentRow) + commentColumn;

                    if (cellValue == 0) {
                        printf("   ");
                    } else {
                        if (((1 + size) / 2) == valueToCheck) {
                            printf(" %i ", cellValue);
                        } else {
                            printf("   ");
                        }
                    }
                }

            }
            printf("¦¦\n");
        }
    }

    printFullSolidLine();
    printf("\n\n");
}


void printSudokuWithSuggestions(int ***p) {
    for (int row = 0; row < size; row ++) {

        if (row % 3 == 0) {
            printFullSolidLine();
        } else {
            printFullBrokenLine();
        }

        for (int commentRow = 0; commentRow < sizeRoot; commentRow++) {

            for (int column = 0; column < size; column++) {

                int cellValue = findFinalCellValue(p, column, row);

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
                        } else {
                            printf("   ");
                        }
                    } else {
                        if (((1 + size) / 2) == valueToCheck) {
                            printf(" %i ", cellValue);
                        } else {
                            printf("   ");
                        }
                    }
                }

            }
            printf("¦¦\n");
        }
    }

    printFullSolidLine();
    printf("\n\n");
}

void printFullSolidLine() {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("==");
            } else {
                printf("=");
            }

            for (int k = 0; k < size; k++) {
                printf("=");
            }
        }
    }
    printf("==\n");
}

void printFullBrokenLine() {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("¦¦");
            } else {
                printf("+");
            }

            for (int k = 0; k < size; k++) {
                printf("-");
            }
        }
    }
    printf("¦¦\n");
}



void printSolidLine() {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("=====");
            } else {
                printf("====");
            }
        }
    }
    printf("==\n");
}

void printBrokenLine() {
    for (int i = 0; i < sizeRoot; i++) {
        for (int j = 0; j < sizeRoot; j++) {
            if (j == 0) {
                printf("¦¦");
            } else {
                printf("+");
            }

            printf("---");
        }
    }
    printf("¦¦\n");
}