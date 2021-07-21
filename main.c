//hw3
//restaurant system
#include "hw3.h"

int main() {
	FILE* in, * out, * menu;	//declare variables
	Menu m;
	PTable t;
	int numT, i;
	out = fopen("output.txt", "wt");	//open output.txt file for write
	if (out == NULL) {	//if the file did not opened print an error message and exit the program
		printf("Cannot open output.txt text file!\n");
		exit(1);
	}
	menu = fopen("Manot.txt", "rt");	//open Manot.txt file for read only
	if (menu == NULL)	//if the file did not opened print an error message to the output file and exit the program
		ErrorMSG(out, "Cannot open Manot.txt text file!");
	in = fopen("Instructions.txt", "rt");	//open Instructions.txt file for read only
	if (in == NULL)	//if the file did not opened print an error message to the output file and exit the program
		ErrorMSG(out, "Cannot open Instructions.txt text file!");
	fscanf(in, "%d", &numT);	//input the number of tables from the instructions file
	t = (PTable)malloc(numT * sizeof(Table));	//allocate memory for the table array in the size of numT
	if (t == NULL)	//if the allocation did not succeed print an error message and exit the program
		ErrorMSG(out, "Error allocating memory for Table array!");
	inst(menu, in, out, &m, t, numT);	//summon inst function to execute the instructions from the instructions file
	FreeMenu(&m);	//free the memory allocated for the menu linked list
	for (i = 0; i < numT; i++)	//check if there is still memory allocated for a table linked list
		if (t[i].head != NULL)
			FreeTable(&t[i]);
	fclose(menu);	//close Manot.txt file
	fclose(in);		//close Instructions.txt file
	fclose(out);	//close output.txt file
	return 0;
}