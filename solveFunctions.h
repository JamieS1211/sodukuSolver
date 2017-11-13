//
// Created by Jamie on 27/10/2017.
//

void solveRows(int ***p);
void solveColumns(int ***p);
void solveBlocks(int ***p);

void solveSuggestionBlockLines(int ***p);

void solvePairs(int ***p);

void solveCellsInRow(int ***p, int row);
void solveCellsInColumn(int ***p, int column, int maxOrder);
void solvePairCellsInBlock(int ***p, int columnStart, int rowStart);

void solveXWing(int ***p);