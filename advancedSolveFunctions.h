//
// Created by Jamie on 14/11/2017.
//

/**
 * Organises runs high order column, row and block solving for the given order and returns the number of changes made
 *
 * @param p             Sudoku pointer
 * @param order         Order of solutions to look for
 * @return              Number of changes made
 */
int solveHighOrderLinkedCells(int ***p, int order);