#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "global.h"
#include "global.c"
#include "printFunctions.h"
#include "cellFunctions.h"
#include "simpleSolveFunctions.h"
#include "advancedSolveFunctions.h"
#include "patturnSolveFunctions.h"

extern int steps;

int isComplete(int ***p);
void * xmalloc(size_t n);

int main() {
    int options = size;
    int ***sudoku = NULL; // stored as sudoku[column][row][suggestions]

    FILE *fp;
    //fp = fopen("testSudoku11.txt", "r");
    /**
     * 1 - Shows column completion (naked single)
     * 2 - Shows row completion (naked single)
     * 3 - Shows block completion  (naked single)
     *
     * 4 - Shows column completion (hidden single)
     * 5 - Shows row completion (hidden single)
     * 6 - Shows block completion  (hidden single)
     *
     * 7 - Shows column completion (all suggestions in block of number are in same column)
     * 8 - Shows row completion (all suggestions in block of number are in same row)
     *
     * 9 - Shows column completion (naked pair) [5,6 pair remove possibilities of 5 and 6 in last two rows of first column]
     * 10 - Shows row completion (naked pair) [5,6 pair remove possibilities of 5 and 6 in last two columns of first row]
     * 11 - Shows block completion  (naked pair) [5,6 pair remove possibilities of 5 and 6 being in last row of block]
    */

    /**
     * 1 - Breezy
     * 2 - Easy
     * 3 - Medium
     * 4 - Hard
     */

    fp = fopen("realSudoku144.txt", "r");

    if (fp == NULL) {
        printf("File not found. Program ending\n");
        exit(0);
    }

    sudoku = xmalloc(size * sizeof(*sudoku));

    for (int column = 0; column < size; column++) {
        sudoku[column] = xmalloc(size * sizeof(*sudoku[column]));

        for (int row = 0; row < size; row++) {
            // Add one so that each place corresponds to the value of its place.
            // Then use place 0 as wright protection.
            sudoku[column][row] = xmalloc((options + 1) * sizeof(*sudoku[column][row]));
        }
    }

    Cell cell;

    int msec = 0, trigger = 10; /* 10ms */
    clock_t before = clock();

    for (int row = 0; row < size; row++) {
        int input[size];
        cell.row = row;

        for (int i = 0; i < size; i++) {
            fscanf(fp, "%i", &input[i]);
        }

        for (int column = 0; column < size; column++) {
            cell.column = column;
            if (input[column] == 0) {
                setInitialUnknown(sudoku, cell);
            } else if (input[column] > 0 && input[column] <= options) {
                setInitialValue(sudoku, cell, input[column]);
            } else {
                printf("A input error has occurred!");
                exit;
            }
        }
    }

    printf("Input sudoku: \n");
    printSudoku(sudoku);

    for (int column = 0; column < size; column++) {
        cell.column = column;

         for (int row = 0; row < size; row++) {
             cell.row = row;

             if (sudoku[cell.column][cell.row][0] == -1) {
                 setupSuggestions(sudoku, cell);
             }
         }
    }

    for (int column = 0; column < size; column++) {
        cell.column = column;

         for (int row = 0; row < size; row++) {
             cell.row = row;

             if (sudoku[cell.column][cell.row][0] == 1) {
                 setFinalCalculatedValue(sudoku, cell, findFinalCellValue(sudoku, cell));
             }
         }
    }

    while (!isComplete(sudoku)) {

        int changes = 0;

        changes += solveRows(sudoku);
        changes += solveColumns(sudoku);
        changes += solveBlocks(sudoku);

        for (int n = 1; n <= size; n++) {
            if (changes == 0 && n > 1) {
                changes += solveHighOrderLinkedCells(sudoku, n);
            }

            if (changes == 0 && n < sizeRoot) {
                changes += solveSuggestionBlockLines(sudoku, n);
            }
        }

        if (changes == 0) {
            changes += solveXWing(sudoku);
        }

        if (changes == 0) {
            printf("Unable to solve the sudoku entered, will print current progress. Steps complete = %i. Program will end. \n", steps);
            printSudokuWithSuggestions(sudoku);
            printSudoku(sudoku);

            clock_t difference = clock() - before;
            msec = difference * 1000 / CLOCKS_PER_SEC;
            printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);

            return 1;
        }
    }

    printf("\nSudoku has been completed after %i steps\n", steps);
    printSudoku(sudoku);

    clock_t difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);

    return 0;
}

int isComplete(int ***p) {
    Cell cell;

    for (int column = 0; column < size; column++) {
        cell.column = column;

        for (int row = 0; row < size; row++){
            cell.row = row;

            if (findFinalCellValue(p, cell) == 0) {
                return 0;
            }
        }
    }

    return 1;
}

void * xmalloc(size_t n) {
    void *p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(1);
    }
    return p;
}