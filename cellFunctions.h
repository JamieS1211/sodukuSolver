//
// Created by Jamie on 05/12/2017.
//

/**
 * Returns if a cell can be written to or not
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @return              If cell can be written to
 */
int canCellWright(int ***p, Cell cell);

/**
 * Initialy sets up a cell to be calculated
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 */
void setInitialUnknown(int ***p, Cell cell);

/**
 * Sets all values of the given cell to possible and returns 1 if found naked single
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @return              If naked single was found
 */
int setupSuggestions(int ***p, Cell cell);

/**
 * Sets cell to initial value with wright protection
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @param number        Cell value
 */
void setInitialValue(int ***p, Cell cell, int number);

/**
 * Sets cell to final calculated value with wright protection
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @param number        Cell value
 */
void setFinalCalculatedValue(int ***p, Cell cell, int number);

/**
 * Clears the cell of other value
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @param number        Cell value
 */
void clearCellOfOtherValues(int ***p, Cell cell, int number);

/**
 * Clears the column, row and block the cell is in of the number (should only be used on final values)
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @param number        Cell value
 */
void clearRegionOfFinalValue(int ***p, Cell cell, int number);

/**
 * Updates a block with the block column and row values using a pointer to the block and a cell in the block
 *
 * @param block         Block pointer
 * @param cell          Cell in block
 */
void setBlockForCell(Block *block, Cell cell);

/**
 * Removes a value as being suggested in a cell
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @param number        Number altering
 */
void eliminatePossibleFromCell(int ***p, Cell cell, int number);

/**
 * Returns the number of suggestions for a cell. If the cell has a final value returns -1 and sets wright protection;
 *
 * @param p             Sudoku pointer
 * @param cell          Cell
 * @return              Number of suggestions for the cell
 */
int suggestionsInCell(int ***p, Cell cell);

/**
 * Returns the number of cells in the column with this suggestion. Returns -1 if a cell has this value.
 *
 * @param p             Sudoku pointer
 * @param column        Column to check
 * @param number        Number to check
 * @return              Number of cells in row with this suggestion
 */
int cellsWithSuggestionInColumn(int ***p, int column, int number);

/**
 * Returns the number of cells in the row with this suggestion. Returns -1 if a cell has this value.
 *
 * @param p             Sudoku pointer
 * @param row           Row to check
 * @param number        Number to check
 * @return              Number of cells in row with this suggestion
 */
int cellsWithSuggestionInRow(int ***p, int row, int number);

/**
 * Returns the number of cells in the block with this suggestion. Returns -1 if a cell has this value.
 *
 * @param p             Sudoku pointer
 * @param block         Block
 * @param number        Number to check
 * @return              Number of cells in block with this suggestion
 */
int cellsWithSuggestionInBlock(int ***p, Block block, int number);

/**
 * Returns the value of a cell if known and 0 if cell is not yet known.
 *
 * @param p         Sudoku pointer
 * @param cell      Cell
 * @return          Value of this cell
 */
int findFinalCellValue(int ***p, Cell cell);