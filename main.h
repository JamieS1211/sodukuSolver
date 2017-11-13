int invalid(int ***p);
int isComplete(int ***p);


void fillPossible(int ***p, int column, int row);
void setOnlyPossible(int ***p, int column, int row, int number);
void setIfPossible(int ***p, int column, int row, int number, int possible);
void eliminatePossibleFromRow(int ***p, int column, int row, int number);
void eliminatePossibleFromColumn(int ***p, int column, int row, int number);
void eliminatePossibleFromBlock(int ***p, int column, int row, int number);

int cellsWithSuggestionInRow(int ***p, int row, int number);
int cellsWithSuggestionInColumn(int ***p, int column, int number);
int cellsWithSuggestionInBlock(int ***p, int column, int row, int number);

int findFinalCellValue(int ***p, int column, int row);

int areCellsIdenticle(int ***p, int columnA, int rowA, int columnB, int rowB);


void * xmalloc(size_t n);