//
// Created by Jamie on 27/10/2017.
//

void solveRows(int ***p);
void solveColumns(int ***p);
void solveBlocks(int ***p);

void solveSuggestionBlockLines(int ***p);

void solvePairs(int ***p);

void solvePairCellsInRow(int ***p, int row);
void solvePairCellsInColumn(int ***p, int column);
void solvePairCellsInBlock(int ***p, int columnStart, int rowStart);