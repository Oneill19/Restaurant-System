//hw3
//hw3 header file
#ifndef hw3
#define hw3
#define _CRT_SECURE_NO_WARNINGS
#define DISH_NAME 50	//max length of a dish name in 50
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structs
typedef struct Dish {	//one-sided linked list of the dishes in the menu
	char* name;
	int price, quantity, nOrders;
	struct Dish* next;
}Dish, * PDish;

typedef struct Menu {	//manager struct of the dishes in the menu
	Dish* head, * tail;
	int size;
}Menu, * PMenu;

typedef struct Plate {	//two-sided linked list of the dishes in the table
	char *dish_name;
	int dish_price, orders;
	struct Plate* next, * prev;
}Plate, * PPlate;

typedef struct Table {	//manager struct of the dishes in the table
	Plate* head, * tail;
	int size, total;
}Table, * PTable;

//function prototypes
void inst(FILE* menu, FILE* in, FILE* out, PMenu m, PTable t, int numT);	//function using switch operation to execute the instructions from the input file
void iniTable(PTable t, int numT);	//function to initialize the tables struct managers
void iniMenu(PMenu m);	//function to initialize the menu struct manager
void CreateProducts(PTable t, PMenu m, FILE* menu, FILE* out);	//function to create the menu from Manot.txt file
PDish Dish_Node(char* name, int price, int quantity);	//function to create a node of a one-way linked list of the menu dishes
void AddItems(FILE* out, PMenu m, char* ProductName, int Quantity);	//function to add portions to an exsits dish
void OrderItem(FILE* out, PMenu m, PTable t, int TableNumber, char* ProductName, int Quantity, int NumOfTables);	//function to order a dish from the menu the table
PPlate Plate_Node(PDish m, char* ProductName, int Quantity);	//function to create a node of a two-way linked list of the dishes in the table
void RemoveItem(FILE* out, PMenu m, PTable t, int TableNumber, char* ProductName, int Quantity, int NumOfTables);	//function to remove a certain dishes from the table
void RemoveTable(FILE* out, PMenu m, PTable t, int TableNumber, int NumOfTables);	//function to close a table
void maxDish(FILE* out, PMenu m);	//funtion to print to the output file the most sold dishes
void ErrorMSG(FILE* out, char* msg);	//funtion to print an error message to the output file and exit the program
void FreeTable(PTable t);	//function to free the memory allocated for a table two-sided linked list
void FreeMenu(PMenu m);	//function to free the memory allocated for the menu one-sided linked list
#endif // !hw3