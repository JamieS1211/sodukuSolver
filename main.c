#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "simpleSolveFunctions.h"
#include "printFunctions.h"
#include "advancedSolveFunctions.h"
#include "patturnSolveFunctions.h"

int invalid(int ***p);
int isComplete(int ***p);

void fillPossible(int ***p, int column, int row);
void setOnlyPossible(int ***p, int column, int row, int number);
void setIfPossible(int ***p, int column, int row, int number, int possible);
void eliminatePossibleFromRowExcept(int ***p, int column, int row, int number);
void eliminatePossibleFromColumnExcept(int ***p, int column, int row, int number);
void eliminatePossibleFromBlockExcept(int ***p, int column, int row, int number);
void eliminatePossibleFromRow(int ***p, int row, int number);
void eliminatePossibleFromColumn(int ***p, int column, int number);
void eliminatePossibleFromBlock(int ***p, int column, int row, int number);

int cellsWithSuggestionInRow(int ***p, int row, int number);
int cellsWithSuggestionInColumn(int ***p, int column, int number);
int cellsWithSuggestionInBlock(int ***p, int column, int row, int number);

int areCellsIdenticle(int ***p, int columnA, int rowA, int columnB, int rowB);

int findFinalCellValue(int ***p, int column, int row);

int areCellsIdenticle(int ***p, int columnA, int rowA, int columnB, int rowB);

int invalid(int ***p);

int isComplete(int ***p);

void * xmalloc(size_t n);


int main() {
    clock_t start = clock(), diff;

    //TODO remove need for columns and size data and use global size info at all times.
    int options = size;
    int ***sudoku = NULL; // stored as sudoku[column][row][suggestions]
    int ***sudokuTemp = NULL; // stored as sudoku[column][row][suggestions]

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



    fp = fopen("realSudoku7.txt", "r");
    /**
     * 1 - Breezy
     * 2 - Easy
     * 3 - Medium
     * 4 - Hard
     */


    sudoku = xmalloc(size * sizeof(*sudoku));
    sudokuTemp = xmalloc(size * sizeof(*sudoku));

    for (int column = 0; column < size; column++) {
        sudoku[column] = xmalloc(size * sizeof(*sudoku[column]));
        sudokuTemp[column] = xmalloc(size * sizeof(*sudokuTemp[column]));

        for (int row = 0; row < size; row++) {
            // Add one so that each place corresponds to the value of its place.
            // Then use place 0 as wright protection.
            //TODO setup wright protection
            sudoku[column][row] = xmalloc((options + 1) * sizeof(*sudoku[column][row]));
            sudokuTemp[column][row] = xmalloc((options + 1) * sizeof(*sudokuTemp[column][row]));
        }
    }

    for (int row = 0; row < size; row++) {
        int input[9];

        //TODO make this size variable
        fscanf(fp, "%i %i %i %i %i %i %i %i %i",
               &input[0], &input[1], &input[2],
               &input[3], &input[4], &input[5],
               &input[6], &input[7], &input[8]);

        for (int column = 0; column < size; column++) {
            if (input[column] == 0) {
                fillPossible(sudoku, column, row);
                fillPossible(sudokuTemp, column, row);
            } else if (input[column] > 0 && input[column] <= options) {
                setOnlyPossible(sudoku, column, row, input[column]);
                setOnlyPossible(sudokuTemp, column, row, input[column]);
            } else {
                printf("A input error has occurred!");
                exit;
            }
        }
    }

    printf("Input suduku: \n");
    printSudoku(sudoku);

    int count = 1;
    while (!isComplete(sudoku)) {

        if (invalid(sudoku)) {
            printf("The sudoku you entered is invalid. Program will end.");    diff = clock() - start;

            diff = clock() - start;

            int microsec = diff/ CLOCKS_PER_SEC;
            printf("\nTime taken %d microseconds",microsec);
            return 1;
        }

        //Duplicate lines so each action happens both before and after all other actions so if one action is
        // dependant on another they are all completed until no more can be done using the simple solve systems.
        solveRows(sudoku);
        solveColumns(sudoku);
        solveBlocks(sudoku);
        solveColumns(sudoku);
        solveRows(sudoku);

        solveSuggestionBlockLines(sudoku);

        solveHighOrderLinkedCells(sudoku);

        solveXWing(sudoku);

        int change = 0;
        for (int column = 0; column < size; column++) {
            for (int row = 0; row < size; row++) {
                for (int option = 1; option <= size; option++) {
                    if (sudokuTemp[column][row][option] != sudoku[column][row][option]) {
                        change = 1;
                    }

                    sudokuTemp[column][row][option] = sudoku[column][row][option];
                }
            }
        }

        if (!change) {
            printf("Unable to solve the sudoku entered, will print current progress. Stages complete = %i. Program will end. \n", count);
            printSudokuWithSuggestions(sudoku);
            printSudoku(sudoku);
            diff = clock() - start;

            int microsec = diff/ CLOCKS_PER_SEC;
            printf("\nTime taken %d microseconds",microsec);
            return 1;
        }

        count++;
    }

    printf("Stage %i:\n\n", count);
    printSudoku(sudoku);


    diff = clock() - start;

    int microsec = diff/ CLOCKS_PER_SEC;
    printf("\nTime taken %d microseconds",microsec);
    return 0;
}


// Sets all values of the given cell to possible
void fillPossible(int ***p, int column, int row) {
    for (int i = 1; i <= size; i++) {
        p[column][row][i] = 1;
    }
}

// Sets cell to be only one possible
void setOnlyPossible(int ***p, int column, int row, int number) {

    eliminatePossibleFromRow(p, row, number);
    eliminatePossibleFromColumn(p, column, number);
    eliminatePossibleFromBlock(p, column, row, number);

    for (int i = 1; i <= size; i++) {
        p[column][row][i] = (i == number);
    }
}

// Sets a value of the given cell to possible or imposable
void setIfPossible(int ***p, int column, int row, int number, int possible) {
        p[column][row][number] = possible;
}

// Eliminates a number from the column
void eliminatePossibleFromColumnExcept(int ***p, int column, int row, int number) {
    eliminatePossibleFromColumn(p, column, number);
    setIfPossible(p, column, row, number, 1);
}

// Eliminates a number from the row except at the given point
void eliminatePossibleFromRowExcept(int ***p, int column, int row, int number) {
    eliminatePossibleFromRow(p, row, number);
    setIfPossible(p, column, row, number, 1);
}

// Eliminates a number from the column
void eliminatePossibleFromColumn(int ***p, int column, int number) {
    for (int r = 0; r < size; r++) {
        setIfPossible(p, column, r, number, 0);
    }
}

// Eliminates a number from the row
void eliminatePossibleFromRow(int ***p, int row, int number) {
    for (int c = 0; c < size; c++) {
        setIfPossible(p, c, row, number, 0);
    }
}

// Eliminates a number from the block
void eliminatePossibleFromBlockExcept(int ***p, int column, int row, int number) {
    eliminatePossibleFromBlock(p, column, row, number);
    setIfPossible(p, column, row, number, 1);
}

// Eliminates a number from the block
void eliminatePossibleFromBlock(int ***p, int column, int row, int number) {
    // Find row and column address for top left cell in block
    int r = row - (row % sizeRoot);
    int c = column - (column % sizeRoot);

    for (int columnOffset = 0; columnOffset < sizeRoot; columnOffset++) {
        for (int rowOffset = 0; rowOffset < sizeRoot; rowOffset++) {
            // 0 represents impossible
            setIfPossible(p, c + columnOffset, r + rowOffset, number, 0);
        }
    }
}

// Returns amount of cells in the column with the suggestion.
int cellsWithSuggestionInColumn(int ***p, int column, int number) {
    int possiblePlaces = 0;

    for (int row = 0; row < size; row++) {
        // Option is a possible value of this cell.
        if (findFinalCellValue(p, column, row) == number) {
            return -1;
        } else if (p[column][row][number] == 1) {
            possiblePlaces++;
        }
    }

    return possiblePlaces;
}

// Returns amount of cells in the row with the suggestion.
int cellsWithSuggestionInRow(int ***p, int row, int number) {
    int possiblePlaces = 0;

    for (int column = 0; column < size; column++) {
        // Option is a possible value of this cell.
        if (findFinalCellValue(p, column, row) == number) {
            return -1;
        } else if (p[column][row][number] == 1) {
            possiblePlaces++;
        }
    }

    return possiblePlaces;
}

// Returns amount of cells in the block with the suggestion.
int cellsWithSuggestionInBlock(int ***p, int column, int row, int number) {
    int possiblePlaces = 0;

    // Find row and column address for top left cell in block
    int r = row - (row % sizeRoot);
    int c = column - (column % sizeRoot);

    for (int columnOffset = 0; columnOffset < sizeRoot; columnOffset++) {
        for (int rowOffset = 0; rowOffset < sizeRoot; rowOffset++) {
            // Option is a possible value of this cell.
            if (findFinalCellValue(p, c + columnOffset, r + rowOffset) == number) {
                return -1;
            } else if (p[c + columnOffset][r + rowOffset][number] == 1) {
                possiblePlaces++;
            }
        }
    }

    return possiblePlaces;
}

// Returns cell value if cell is known else returns a 0 for not known yet.
int findFinalCellValue(int ***p, int column, int row) {
    int possibilities = 0;
    int value = 0;

    for (int option = 1; option <= size; option++) {
        // Option is a possible value of this cell.
        if (p[column][row][option] == 1) {
            possibilities++;
            value = option;
        }
    }

    if (possibilities > 1) {
        return 0;
    } else {
        return value;
    }
}

// Returns if two cells have the same values / suggestions
int areCellsIdenticle(int ***p, int columnA, int rowA, int columnB, int rowB) {
    for (int option = 1; option <= size; option++) {
        if (p[columnA][rowA][option] != p[columnB][rowB][option]) {
            return 0;
        }
    }

    return 1;
}

int invalid(int ***p) {
    int temp[size];
    int value;
    int unknowns = 0;
    //TODO check if value already used in column

    //TODO check if value already used in row

    //TODO check if value already used in block

    return 0;
}

int isComplete(int ***p) {
    for (int column = 0; column < size; column++) {
        for (int row = 0; row < size; row ++){
            if (findFinalCellValue(p, column, row) == 0) {
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