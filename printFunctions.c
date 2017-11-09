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
            printf("==================================================\n");
        } else {
            printf("¦¦----+----+----¦¦----+----+----¦¦----+----+----¦¦\n");
        }
        for (int column = 0; column < size; column++) {
            if (column % 3 == 0) {
                printf("¦¦");
            } else {
                printf("¦");
            }

            value = findFinalCellValue(p, column, row);

            if (value == 0) {
                printf("    ");
            } else {
                printf(" %i  ", value);
            }
        }
        printf("¦¦ \n");
    }
    printf("==================================================\n\n\n");
}


void printSudokuBig(int ***p) {
    for (int row = 0; row < size; row ++) {

        if (row % 3 == 0) {
            printf("===============================================================================================\n");
        } else {
            printf("¦¦---------+---------+---------¦¦---------+---------+---------¦¦---------+---------+---------¦¦\n");
        }

        for (int commentRow = 0; commentRow < sizeRoot; commentRow++) {

            for (int column = 0; column < size; column++) {

                int cellValue = findFinalCellValue(p, column, row);

                if (column % 3 == 0) {
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


    printf("===============================================================================================\n\n\n");
}


void printSudokuWithSuggestions(int ***p) {

    //return;

    printf("Start of sudoku with hits \n");


    for (int row = 0; row < size; row ++) {

        if (row % 3 == 0) {
            printf("===============================================================================================\n");
        } else {
            printf("¦¦---------+---------+---------¦¦---------+---------+---------¦¦---------+---------+---------¦¦\n");
        }

        for (int commentRow = 0; commentRow < sizeRoot; commentRow++) {

            for (int column = 0; column < size; column++) {

                int cellValue = findFinalCellValue(p, column, row);

                if (column % 3 == 0) {
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


    printf("===============================================================================================\n\n\n");

    printf("\n end of sudoku with hits \n");
}