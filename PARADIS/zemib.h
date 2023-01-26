#define MOD 16
typedef unsigned int  ui;
ui findMax(ui *array);
void msdRadixSort(ui *array, ui mod);
void radixSort(ui *array, int l, int left, int right);
void display(ui *array, int l, int left, int right);

/* parallel function */
ui findMaxPar(ui *array);
void paradis(ui *array, ui argmod);
void radixParSort(ui *array, int l, int left, int right);