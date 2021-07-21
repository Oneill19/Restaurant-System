//hw3
//hw3 implementation file
#include "hw3.h"

void inst(FILE* menu, FILE* in, FILE* out, PMenu m, PTable t, int numT) {	//function using switch operation to execute the instructions from the input file
	int opt, temp_TableNumber, temp_Quantity, flag;	//declare variables
	char temp_DishName[DISH_NAME + 1], temp_char;
	while ((fscanf(in, "%d", &opt)) != EOF) {	//loop that runs until the program get to the end of the file and input the instruction from the input file
		switch (opt) {	//switch operation for the instructions
		case 1:	//option 1 - create the kitchen
			iniMenu(m);	//initialze the manager struct of the menu
			iniTable(t, numT);	//initialize the manger structs of the tables
			CreateProducts(t, m, menu, out);	//add the items from Manot.txt file to the kitchen
			break;
		case 2:	//option 2 - add portions to the dishes in the menu
			fscanf(in, "%s %d", temp_DishName, &temp_Quantity);	//input the dish name and quantity to add to the menu
			AddItems(out, m, temp_DishName, temp_Quantity);	//summon AddItems function
			break;
		case 3:	//option 3 - order items to a table
			fscanf(in, "%d %s %d", &temp_TableNumber, temp_DishName, &temp_Quantity);	//input table number, dish name and quantity of the order
			OrderItem(out, m, t, temp_TableNumber, temp_DishName, temp_Quantity, numT);	//summon OrderItem function
			break;
		case 4:	//option 4 - remove items from a table
			fscanf(in, "%d %s %d", &temp_TableNumber, temp_DishName, &temp_Quantity);	//input table number, dish name and quantity to remove from the table
			RemoveItem(out, m, t, temp_TableNumber, temp_DishName, temp_Quantity, numT);	//summon RemoveItem function
			break;
		case 5:	//option 5 - close table
			fscanf(in, "%d", &temp_TableNumber);	//input table number to close
			RemoveTable(out, m, t, temp_TableNumber, numT);	//summon RemoveTable function
			break;
		default:
			fprintf(out, "There is no such option\n");	//if opt is not a number between 1-5
			flag = 1;
			while (flag && (fscanf(in, "%c", &temp_char)) != EOF)	//skip the data from the Instructions.txt file to the next instruction
				flag = (temp_char == 10) ? 0 : 1;
		}
	}
}

void iniTable(PTable t, int numT) {	//function to initialize the tables struct managers
	int i;	//declare variables
	for (i = 0; i < numT; i++) {	//loop to initialize all the tables manager structs
		t[i].head = NULL;
		t[i].tail = NULL;
		t[i].size = 0;
		t[i].total = 0;
	}
}

void iniMenu(PMenu m) {	//function to initialize the menu struct manager
	m->head = NULL;
	m->tail = NULL;
	m->size = 0;
}

void CreateProducts(PTable t, PMenu m, FILE* menu, FILE* out) {	//function to create the menu from Manot.txt file
	PDish new_node, temp_node;	//declare variables
	char temp_name[DISH_NAME + 1];
	int price, quantity, name_flag;
	while ((fscanf(menu, "%s %d %d", temp_name, &quantity, &price)) != EOF) {	//loop that input the data of every dish in the menu and create the menu linked list
		if (price <= 0)	//if the price of the dish that received in negative print in the output file an error message
			fprintf(out, "Error! The price of %s is a negative number!\n", temp_name);
		else {
			if (quantity < 0)	//if the quantity of dish that recieved in negative print in the output file an error message
				fprintf(out, "Error! The Quantity of %s is a negative number!\n", temp_name);
			else {
				temp_node = m->head;
				name_flag = 1;
				while (temp_node != NULL) {	//check if there is already a dish with that name in the menu
					if (!strcmp(temp_node->name, temp_name))
						name_flag = 0;
					temp_node = temp_node->next;
				}
				if (!name_flag)	//if there is already a dish with that name in the menu print in the output file an error message
					fprintf(out, "There is already %s in the kitchen\n", temp_name);
				else {
					new_node = Dish_Node(temp_name, price, quantity);	//create a new node for the linked list using Dish_Node function
					if (new_node == NULL) {	//if the value of new_node is NULL
						FreeMenu(m);	//free the memory allocated for the linked list of the menu
						free(t);	//free the memory allocated for table array
						ErrorMSG(out, "Error allocating memory for the menu linked list node!");	//print an error message and exit the program
					}
					if (m->tail == NULL)	//if the linked list is empty
						m->head = new_node;	//make m->head to point on new_node
					else
						m->tail->next = new_node;	//else insert new_node to the end of the linked list
					m->tail = new_node;	//make m->tail to point on the new node inserted to the linked list
					m->size++;	//increase the number of dishes in the menu
				}
			}
		}
	}
	fprintf(out, "The kitchen was created\n");	//print to the output file that the kitchen was created
}

PDish Dish_Node(char* name, int price, int quantity) {	//function to create a node of a one-way linked list of the menu dishes
	PDish new_node = (PDish)malloc(sizeof(Dish));	//declare variables and allocate memory for a dish node for the menu linked list
	if (new_node) {	//if the allocation succeed
		new_node->name = (char*)malloc((strlen(name) + 1) * sizeof(char));	//allocate memory for the name of the dish
		if (new_node->name == NULL) {	//if the name allocation did not succeed
			free(new_node);	//free the memory allocated of the node
			return NULL;	//return NULL
		}
		strcpy(new_node->name, name);	//copy the string from name to new_node->name
		new_node->price = price;	//the price of the dish
		new_node->quantity = quantity;	//number of portions of the dish
		new_node->nOrders = 0;	//number of orders of that dish
		new_node->next = NULL;	//initialize the next to NULL
	}
	return new_node;	//return new_node
}

void AddItems(FILE* out, PMenu m, char* ProductName, int Quantity) {	//function to add portions to an exsits dish
	PDish temp = m->head;	//declare variables
	if (Quantity <= 0) {	//if the quantity of potion to add is negative print in the output file an error message
		fprintf(out, "The quantity of %s to add to the kitchen is negative!\n", ProductName);
		return;	//end the function
	}
	while (temp != NULL) {	//while loop
		if (!strcmp(temp->name, ProductName)) {	//if the names are identical add to that dish the quantity of potions and print a message in the output file
			temp->quantity += Quantity;
			fprintf(out, "%d %s were added to the kitchen\n", Quantity, ProductName);
			return;	//end the function
		}
		temp = temp->next;
	}
	fprintf(out, "There is no %s in the kitchen!\n", ProductName);	//if there is not such dish in the menu print and error message
}

void OrderItem(FILE* out, PMenu m, PTable t, int TableNumber, char* ProductName, int Quantity, int NumOfTables) {	//function to order a dish from the menu the table
	int i, name_flag = 0;	//declare variables
	PPlate new_node, temp_node;
	PDish d_temp;
	if (TableNumber > NumOfTables || TableNumber < 0) {	//if the table number in not in range
		fprintf(out, "Table number Error!\n");
		return;	//end the function
	}
	d_temp = m->head;	//d_temp point on m->head
	while (d_temp != NULL && !name_flag) {	//if not the while loop search for the right dish
		if (!strcmp(d_temp->name, ProductName))
			name_flag = 1;
		else
			d_temp = d_temp->next;
	}
	if (!name_flag) {	//if there is no such dish in the menu print a message and end the function
		fprintf(out, "We don't have %s, sorry!\n", ProductName);
		return;
	}
	if (Quantity <= 0) {	//in the quantity to order in negative print and error message and end the function
		fprintf(out, "The quantity of %s to order is negative!\n", ProductName);
		return;
	}
	if (Quantity > d_temp->quantity) {	//if the quantity to order is bigger than the quantity of the dish in the kitchen print an error message and end the function
		fprintf(out, "We don't have enough %s, sorry!\n", d_temp->name);	
		return;
	}
	temp_node = t[TableNumber - 1].head;
	while (temp_node != NULL) {	//check if there is already a dish like this in the table
		if (!strcmp(temp_node->dish_name, ProductName)) {	//if there is a dish like that in the table
			d_temp->quantity -= Quantity;	//update the quantity of that dish in the kitchen
			d_temp->nOrders += Quantity;	//update the number of orders of that dish
			temp_node->orders += Quantity;	//update the number of that dish in the table
			t[TableNumber - 1].total += d_temp->price * Quantity;	//update to total bill of that table
			fprintf(out, "%d %s were added to table number %d\n", Quantity, ProductName, TableNumber);
			return;	//end the function
		}
		else
			temp_node = temp_node->next;	//move to the next element in the linked list
	}
	new_node = Plate_Node(d_temp, ProductName, Quantity);	//create a new node for the linked list unsing Plate_Node function
	if (new_node == NULL) {	//if the value of new_node is NULL
		for (i = 0; i < NumOfTables; i++)	//free the memory allocated for all the tables
			FreeTable(&t[i]);
		free(t);	//free the memory allocated for the table array
		FreeMenu(m);	//free the memory allocated for the menu
		ErrorMSG(out, "Error allocating memory for the table linked list node!");	//print an error message and exit the program
	}
	if (t[TableNumber - 1].head == NULL) {	//if the table linked list is empty
		t[TableNumber - 1].tail = new_node;	//make the tail to point on new_node
		t[TableNumber - 1].head = new_node;	//make the head to point on new_node
	}
	else {
		new_node->next = t[TableNumber - 1].head;	//insert new_node in the start of the linked list
		t[TableNumber - 1].head->prev = new_node;	//make the prev of the previous head to point on new_node
		t[TableNumber - 1].head = new_node;	//make new_node the new head of the linked list
	}
	t[TableNumber - 1].total += new_node->dish_price * Quantity;	//update to total bill of the table
	fprintf(out, "%d %s were added to table number %d\n", Quantity, ProductName, TableNumber);	//print the dish name and quantity that added to the table to the output file
}

PPlate Plate_Node(PDish m, char* ProductName, int Quantity) {	//function to create a node of a two-way linked list of the dishes in the table
	PPlate new_node = (PPlate)malloc(sizeof(Plate));;	//declare variables and allocate memory for the new node
	PDish d_temp = m;
	if (new_node) {	//if the allocation succeed
		new_node->dish_name = (char*)malloc((strlen(d_temp->name) + 1) * sizeof(char));	//allocate memory for the name of the dish
		if (new_node->dish_name == NULL) {	//if the name allocation did not succeed
			free(new_node);	//free the memory allocated of the node
			return NULL;	//return NULL
		}
		strcpy(new_node->dish_name, d_temp->name);	//copy the string from d_temp->name to new_node->dish_name
		new_node->dish_price = d_temp->price;	//price of the dish
		d_temp->quantity -= Quantity;	//update the quantity of that dish in the kitchen
		d_temp->nOrders += Quantity;	//update the number of orders of that dish 
		new_node->orders = Quantity;	//number of that dish that ordered to the table
		new_node->next = NULL;	//initialize next to NULL
		new_node->prev = NULL;	//initialize pre to NULL
	}
	return new_node;	//return new_node
}

void RemoveItem(FILE* out, PMenu m, PTable t, int TableNumber, char* ProductName, int Quantity, int NumOfTables) {	//function to remove a certain dishes from the table
	PPlate temp_table = t[TableNumber - 1].head;	//declare variables
	if (TableNumber >= NumOfTables || TableNumber <= 0) {	//if the table number is not in range
		fprintf(out, "Table number Error!\n");
		return;	//end the function
	}
	while (temp_table != NULL && strcmp(temp_table->dish_name, ProductName))	//search the dish from the table to remove
		temp_table = temp_table->next;
	if (temp_table == NULL) {	//if there is no such dish in the table
		fprintf(out, "There is no %s in the table to return!\n", ProductName);
		return;
	}
	if (Quantity <= 0 || Quantity > temp_table->orders) {	//if the quantity to remove is not in range
		fprintf(out, "The quantity to return is not in range!\n");
		return;
	}
	t[TableNumber - 1].total -= Quantity * temp_table->dish_price;	//update the total bill of that table
	if (Quantity == temp_table->orders) {	//if the quantity to remove equal to the quantity of that dish in the table
		if (temp_table == t[TableNumber - 1].head) {	//if the node is the head of the linked list update the head to the next node
			temp_table->next->prev = temp_table->prev;
			t[TableNumber - 1].head = temp_table->next;
		}
		else if (temp_table == t[TableNumber - 1].tail) {	//if the node is the tail of the linked list update the tail to the previous node
			temp_table->prev->next = temp_table->next;
			t[TableNumber - 1].tail = temp_table->prev;
		}
		else {
			temp_table->prev->next = temp_table->next;	//connect the prev of temp_table with the next of temp_table
			temp_table->next->prev = temp_table->prev;	//connect the prev of the next of temp_table with the prev of temp_table
		}
		free(temp_table->dish_name);	//free the memory allocated for the name of the dish
		free(temp_table);	//free the memory allocated for the node
	}
	else
		temp_table->orders -= Quantity;	//update the quantity of that dish in the table
	fprintf(out, "%d %s was returned to the kitchen from table number %d\n", Quantity, ProductName, TableNumber);	//print the dish name and quantity that removed from the table to the output file
}

void RemoveTable(FILE* out, PMenu m, PTable t, int TableNumber, int NumOfTables) {	//function to close a table
	int i, counter = 0;	//declare variables
	PTable t_temp = &t[TableNumber - 1];
	if (TableNumber > NumOfTables || TableNumber <= 0) {	//if the table number is not in range
		fprintf(out, "Table number Error!\n");
		return;	//end the function
	}
	if (t[TableNumber - 1].head == NULL) {	//if that table did not ordered yet
		fprintf(out, "The table number %d is not ordered yet\n", TableNumber);
		return;	//end the function
	}
	fprintf(out, "Table Number %d bill: ", TableNumber);	//print the dishes ordered and the total bill
	while (t_temp->head != NULL) {
		fprintf(out, "%d %s ", t_temp->head->orders, t_temp->head->dish_name);
		t_temp->head = t_temp->head->next;
	}
	fprintf(out, ". %d NIS please!\n", t_temp->total);
	FreeTable(&t[TableNumber - 1]);	//free the memory allocated for that table linked list
	for (i = 0; i < NumOfTables; i++)	//count the number of closed tables in the restaurant
		if (t[i].head == NULL)
			counter++;
	if (counter == NumOfTables)	//if all the tables are closed print the the most popular dishes and the number that dishes were ordered
		maxDish(out, m);
}

void maxDish(FILE* out, PMenu m) {	//funtion to print to the output file the most sold dishes
	int max = 0;
	PDish temp = m->head;
	while (temp != NULL) {
		max = (temp->nOrders > max) ? temp->nOrders : max;	//save the number of the highest number of orders in max
		temp = temp->next;
	}
	fprintf(out, "The most popular dish/dishes today is/are: ");
	temp = m->head;
	while (temp != NULL) {	//print to the output file all the dishes that sold max times
		if (temp->nOrders == max)
			fprintf(out, "%s, ", temp->name);
		temp = temp->next;
	}
	fprintf(out, "sold %d time!\n", max);
}

void ErrorMSG(FILE* out, char* msg) {	//funtion to print an error message and exit the program
	fprintf(out, "%s\n", msg);	//prints msg
	exit(1);	//exit the program
}

void FreeTable(PTable t) {	//function to free the memory allocated for a table two-sided linked list
	PPlate temp;	//declare variables
	while (t->head != NULL) {	//loop until the end of the linked list
		temp = t->head;	//save the first element in the linked list in temp
		t->head = t->head->next;	//move the linked list head to the next element
		free(temp->dish_name);	//free the memory allocated for the name of the dish
		free(temp);	//free the memory allocated for the node
	}
	t->tail = NULL;	//initialize the tail to NULL
	t->size = 0;	//initialize the size
	t->total = 0;	//initialize the total bill
}

void FreeMenu(PMenu m) {	//function to free the memory allocated for the menu one-sided linked list
	PDish temp;	//declare variables
	while (m->head != NULL) {	//loop until the end of the linked list
		temp = m->head;	//save the fisrt element of in the linked list in temp
		m->head = m->head->next;	//move the linked list head to the next element
		free(temp->name);	//free the memory allocated for the name of the dish
		free(temp);	//free the memory allocated for the node
	}
	m->tail = NULL;	//initialize the tail to NULL
	m->size = 0;	//initialize the size
}