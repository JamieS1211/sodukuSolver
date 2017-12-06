//
// Created by Jamie on 27/10/2017.
//

/**
 * Solves hidden singles in all rows of the given sudoku and returns the amount of changes made
 *
 * @param p         Sudoku pointer
 * @return          Number of changes made
 */
int solveRows(int ***p);

/**
 * Solves hidden singles in all columns of the given sudoku and returns the amount of changes made
 *
 * @param p         Sudoku pointer
 * @return          Number of changes made
 */
int solveColumns(int ***p);

/**
 * Solves hidden singles in all blocks of the given sudoku and returns the amount of changes made
 *
 * @param p         Sudoku pointer
 * @return          Number of changes made
 */
int solveBlocks(int ***p);

/** Finds cell suggestions to the order entered constrained in certain blocks and columns or rows and eliminates suggestions that are in disallowed places then returns the number of changes made
 *
 * @param p             Sudoku pointer
 * @param order         Order of solutions to find
 * @return              Number of changes made
 */
int solveSuggestionBlockLines(int ***p, int order);