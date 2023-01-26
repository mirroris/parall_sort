#define MOD 16
typedef unsigned int  ui;
ui findMax(ui *array);
void msdRadixSort(ui *array, ui mod);
void radixSort(ui *array, int l, int left, int right);
void display(ui *array);

/* parallel function */
ui findMaxPar(ui *array);
void msdParRadixSort(ui *array, ui argmod);
void paradis(ui *array, int l, int left, int right);