#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "config.h"
#include "solveFunctions.h"

int invalid(int ***p);
int isComplete(int ***p);


void fillPossible(int ***p, int column, int row);
void setOnlyPossible(int ***p, int column, int row, int number);
void setIfPossible(int ***p, int column, int row, int number, int possible);
void eliminatePossibleFromRow(int ***p, int column, int row, int number);
void eliminatePossibleFromColumn(int ***p, int column, int row, int number);
void eliminatePossibleFromBlock(int ***p, int column, int row, int number);

void findPairCellsInRow(int ***p, int row);
void findPairCellsInColumn(int ***p, int column);
void findPairCellsInBlock(int ***p, int columnStart, int rowStart);

int cellsWithSuggestionInRow(int ***p, int row, int number);
int cellsWithSuggestionInColumn(int ***p, int column, int number);
int cellsWithSuggestionInBlock(int ***p, int column, int row, int number);

int findFinalCellValue(int ***p, int column, int row);


void * xmalloc(size_t n);
void printSudoku(int ***p);
void printSudokuBig(int ***p);
void printSudokuWithSugestions(int ***p);


int main() {
    clock_t start = clock(), diff;

    //TODO remove need for columns and size data and use global size info at all times.
    int options = size;
    int ***p = NULL; // stored as p[column][row][suggestions]

    FILE *fp;
    fp = fopen("testSudoku9.txt", "r");


    p = xmalloc(size * sizeof(*p));

    for (int column = 0; column < size; column++) {
        p[column] = xmalloc(size * sizeof(*p[column]));

        for (int row = 0; row < size; row++) {
            // Add one so that each place corresponds to the value of its place.
            // Then use place 0 as wright protection.
            //TODO setup wright protection
            p[column][row] = xmalloc((options + 1) * sizeof(*p[column][row]));
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
                fillPossible(p, column, row);
            } else if (input[column] > 0 && input[column] <= options) {
                setOnlyPossible(p, column, row, input[column]);
            } else {
                printf("A input error has occurred!");
                exit;
            }
        }
    }

    printf("Input suduku: \n");
    printSudoku(p);
    printSudokuBig(p);

    int count = 1;
    while (!isComplete(p)) {

        if (invalid(p)) {
            printf("The sudoku you entered is invalid. Program will end.");    diff = clock() - start;

            int msec = diff * 1000 / CLOCKS_PER_SEC;
            printf("\nTime taken %d seconds %d milliseconds", msec/1000, msec%1000);
            return 1;
        }

        if (count > 20) {
            printf("Unable to solve the sudoku entered, will print current progress. Program will end. \n");
            printSudokuWithSugestions(p);
            printSudokuBig(p);
            return 1;
        }

        //Duplicate lines so each action happens both before and after all other actions so if one action is
        // dependant on another they are all completed until no more can be done using the simple solve systems.
        solveRows(p);
        solveColumns(p);
        solveBlocks(p);
        solveColumns(p);
        solveRows(p);

        solveSuggestionBlockLines(p);

        //TODO sudoku 7 column 8 rows 4 and 8 1 8 pair
        solvePairs(p);

        count++;

    }

    printf("Stage %i:\n\n", count);
    printSudoku(p);


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

    eliminatePossibleFromRow(p, column, row, number);
    eliminatePossibleFromColumn(p, column, row, number);
    eliminatePossibleFromBlock(p, column, row, number);

    for (int i = 1; i <= size; i++) {
        p[column][row][i] = (i == number);
    }

    p[column][row][0] = 1;
}

// Sets a value of the given cell to possible or imposable
void setIfPossible(int ***p, int column, int row, int number, int possible) {
        p[column][row][number] = possible;
}

// Eliminates a number from the column
void eliminatePossibleFromColumn(int ***p, int column, int row, int number) {
    for (int r = 0; r < size; r++) {
        // 0 represents impossible
        setIfPossible(p, column, r, number, 0);
    }

    setIfPossible(p, column, row, number, 1);
}

// Eliminates a number from the row
void eliminatePossibleFromRow(int ***p, int column, int row, int number) {
    for (int c = 0; c < size; c++) {
        // 0 represents impossible
        setIfPossible(p, c, row, number, 0);
    }

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

    setIfPossible(p, column, row, number, 1);
}

//TODO check for x cells in a row, column or block that have the same x numbers in each. Eleminaate those x numbers from all other cells in the row column or block.
void findPairCellsInRow(int ***p, int row) {

    for (int columnA = 0; columnA < size; columnA++) {
        for (int columnB = 0; columnB < size; columnB++) {
            // Only run each option once
            if (columnA < columnB) {

                // If both cells are not yet solved
                if (findFinalCellValue(p, columnA, row) == 0 && findFinalCellValue(p, columnB, row) == 0) {

                    int valuesAbsentTogether = 0;
                    int valuesPresentTogether = 0;
                    int valuesElsewhereCalculated[size + 1];
                    int valuesElsewhere[size + 1]; //True or false if a value can be in a place other then thease two cells

                    for (int option = 1; option <= size; option++) {
                        valuesElsewhereCalculated[option] = 0;
                        valuesElsewhere[option] = 0;
                    }

                    // for every option if both cells have that value possible add to values that match.
                    for (int option = 1; option <= size; option++) {
                        if ((p[columnA][row][option] == 1) && (p[columnB][row][option] == 1)) {
                            valuesPresentTogether++;
                            valuesElsewhereCalculated[option] = 1;

                            if (cellsWithSuggestionInRow(p, row, option) > 2) {
                                valuesElsewhere[option] = 1;
                            }
                        } else if ((p[columnA][row][option] == 0) && (p[columnB][row][option] == 0)) {
                            valuesAbsentTogether++;
                        }
                    }

                    // Cells are are identical
                    if (valuesPresentTogether == 2 && valuesAbsentTogether == 7) {

                        for (int option = 1; option <= size; option++) {
                            if (p[columnA][row][option] == 1 && p[columnB][row][option] == 1) {
                                eliminatePossibleFromRow(p, columnA, row, option);
                                setIfPossible(p, columnB, row, option, 1);
                            }
                        }
                    } else if (valuesPresentTogether >= 2) { //If values share 2 or more values but 2 of those values can only exist in that one place
                        int count = 0; //The number of values that must occupy one of these two cells

                        for (int option = 0; option <= size; option++) {
                            if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                                count++;
                            }
                        }

                        if (count == 2) {
                            for (int option = 1; option <= size; option++) {
                                if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) { // Remove these numbers from being suggestions elsewhere
                                    eliminatePossibleFromRow(p, columnA, row, option);
                                    setIfPossible(p, columnB, row, option, 1);
                                } else { // Remove suggestions for other values in these cells
                                    setIfPossible(p, columnA, row, option, 0);
                                    setIfPossible(p, columnB, row, option, 0);
                                }
                            }
                        }
                    }

                }
            }
        }
    }
}

void findPairCellsInColumn(int ***p, int column) {

    //TODO Change for block
    for (int rowA = 0; rowA < size; rowA++) {
        for (int rowB = 0; rowB < size; rowB++) {
            // Only run each option once
            if (rowA < rowB) {

                // If both cells are not yet solved
                if (findFinalCellValue(p, column, rowA) == 0 && findFinalCellValue(p, column, rowB) == 0) {

                    int valuesAbsentTogether = 0;
                    int valuesPresentTogether = 0;
                    int valuesElsewhereCalculated[size + 1];
                    int valuesElsewhere[size + 1]; //True or false if a value can be in a place other then thease two cells

                    for (int option = 1; option <= size; option++) {
                        valuesElsewhereCalculated[option] = 0;
                        valuesElsewhere[option] = 0;
                    }


                    // for every option if both cells have that value possible add to values that match.
                    for (int option = 1; option <= size; option++) {
                        if ((p[column][rowA][option] == 1) && (p[column][rowB][option] == 1)) {
                            valuesPresentTogether++;
                            valuesElsewhereCalculated[option] = 1;

                            if (cellsWithSuggestionInColumn(p, column, option) > 2) {
                                valuesElsewhere[option] = 1;
                            }
                        } else if ((p[column][rowA][option] == 0) && (p[column][rowB][option] == 0)) {
                            valuesAbsentTogether++;
                        }
                    }

                    // Cells are are identical
                    if (valuesPresentTogether == 2 && valuesAbsentTogether == 7) {

                        for (int option = 1; option <= size; option++) {
                            if (p[column][rowA][option] == 1 && p[column][rowB][option] == 1) {
                                eliminatePossibleFromColumn(p, column, rowA, option);
                                setIfPossible(p, column, rowB, option, 1);
                            }
                        }
                    } else if (valuesPresentTogether >= 2) { //If values share 2 or more values but 2 of those values can only exist in that one place
                        int count = 0; //The number of values that must occupy one of these two cells

                        for (int option = 0; option <= size; option++) {
                            if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                                count++;
                            }
                        }

                        if (count == 2) {
                            for (int option = 1; option <= size; option++) {
                                if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) { // Remove these numbers from being suggestions elsewhere
                                    eliminatePossibleFromColumn(p, column, rowA, option);
                                    setIfPossible(p, column, rowB, option, 1);
                                } else { // Remove suggestions for other values in these cells
                                    setIfPossible(p, column, rowA, option, 0);
                                    setIfPossible(p, column, rowB, option, 0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void findPairCellsInBlock(int ***p, int columnStart, int rowStart) {

    // Find row and column address for top left cell in block
    int r = rowStart - (rowStart % sizeRoot);
    int c = columnStart - (columnStart % sizeRoot);

    for (int columnAOffset = 0; columnAOffset < sizeRoot; columnAOffset++) {
        for (int rowAOffset = 0; rowAOffset < sizeRoot; rowAOffset++) {

            int columnA = c + columnAOffset;
            int rowA = r + rowAOffset;

            for (int columnBOffset = 0; columnBOffset < sizeRoot; columnBOffset++) {
                for (int rowBOffset = 0; rowBOffset < sizeRoot; rowBOffset++) {

                    int columnB = c + columnBOffset;
                    int rowB = r + rowBOffset;


                    if (columnA < columnB || rowA < rowB) {

                        // If both cells are not yet solved
                        if (findFinalCellValue(p, columnA, rowA) == 0 && findFinalCellValue(p, columnB, rowB) == 0) {

                            int valuesAbsentTogether = 0;
                            int valuesPresentTogether = 0;
                            int valuesElsewhereCalculated[size + 1];
                            int valuesElsewhere[size + 1]; //True or false if a value can be in a place other then thease two cells

                            for (int option = 1; option <= size; option++) {
                                valuesElsewhereCalculated[option] = 0;
                                valuesElsewhere[option] = 0;
                            }


                            // for every option if both cells have that value possible add to values that match.
                            for (int option = 1; option <= size; option++) {
                                if ((p[columnA][rowA][option] == 1) && (p[columnB][rowB][option] == 1)) {
                                    valuesPresentTogether++;
                                    valuesElsewhereCalculated[option] = 1;

                                    if (cellsWithSuggestionInBlock(p, columnA, rowA, option) > 2) {
                                        valuesElsewhere[option] = 1;
                                    }
                                } else if ((p[columnA][rowA][option] == 0) && (p[columnB][rowB][option] == 0)) {
                                    valuesAbsentTogether++;
                                }
                            }

                            // Cells are are identical
                            if (valuesPresentTogether == 2 && valuesAbsentTogether == 7) {

                                for (int option = 1; option <= size; option++) {
                                    if (p[columnA][rowA][option] == 1 && p[columnB][rowB][option] == 1) {
                                        eliminatePossibleFromBlock(p, columnA, rowA, option);
                                        setIfPossible(p, columnB, rowB, option, 1);
                                    }
                                }
                            } else if (valuesPresentTogether >=
                                       2) { //If values share 2 or more values but 2 of those values can only exist in that one place
                                int count = 0; //The number of values that must occupy one of thease two cells

                                for (int option = 0; option <= size; option++) {
                                    if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] == 1) {
                                        count++;
                                    }
                                }

                                if (count == 2) {
                                    for (int option = 1; option <= size; option++) {
                                        if (valuesElsewhere[option] == 0 && valuesElsewhereCalculated[option] ==
                                                                            1) { // Remove thease numbers from being suggestions elsewhere
                                            eliminatePossibleFromBlock(p, columnA, rowA, option);
                                            setIfPossible(p, columnB, rowB, option, 1);
                                        } else { // Remove suggestions for other values in thease cells
                                            setIfPossible(p, columnA, rowA, option, 0);
                                            setIfPossible(p, columnB, rowB, option, 0);
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

// Returns amount of cells in the column with the suggestion.
int cellsWithSuggestionInColumn(int ***p, int column, int number) {
    int possiblePlaces = 0;

    for (int row = 0; row < size; row++) {
        // Option is a possible value of this cell.
        if (findFinalCellValue(p, column, row) == number) {
            return 0;
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
            return 0;
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
                return 0;
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
        int temp = p[column][row][option];

        // Option is a possible value of this cell.
        if (temp == 1) {
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


void printSudokuWithSugestions(int ***p) {

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