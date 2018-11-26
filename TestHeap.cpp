#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
	#include "HeapInfo.h"
}


using namespace std;


int main()
{
	//Variables for testing HP_GetAllEntries()
	char id[] = "id";
	char name[] = "surname";
	char surname[] = "name";
	char city[] = "city";
	char all[] = "all";
	char nameTest[] = "Antonio";
	char surnameTest[] = "Llorca";
	char cityTest[] = "Nikaia";
	int myInt = 1193;
	int * idTest = &myInt;
	

	//Opening text files for reading the records
	char fileName[] = "myFileTest";
	char fileName2[] = "myFileHeap";
	int desc, desc2;
	Record myRecord;
	ifstream heapValues , heapValues2;
	heapValues.open("100.txt");
	heapValues2.open("100.txt");


//_____________________________________________________________________________________________________________________

	
	//FIRST FILE MANAGEMENT 
	cout << endl;
	cout << "  *** FILE ONE ***  " << endl << endl;
	HP_CreateFile(fileName);
	desc = HP_OpenFile(fileName);

	while (!heapValues.eof())
	{
		heapValues >> myRecord.id;
		heapValues >> myRecord.name;
		heapValues >> myRecord.surname;
		heapValues >> myRecord.city;

		HP_InsertEntry(desc, myRecord);
	}

	
	HP_GetAllEntries(desc, all, idTest);
	HP_GetAllEntries(desc, name, (void*)nameTest);
	HP_GetAllEntries(desc, id, idTest);
	

	heapValues.close();
	
	//Closing first file
	if (HP_CloseFile(desc) == 0)
	{
		cout << "File : " << fileName << " closed." << endl << endl;
	}
	
//_____________________________________________________________________________________________________________________

	
	//SECOND FILE MANAGEMENT
	cout << endl;
	cout << "  *** FILE TWO ***  " << endl << endl;
	HP_CreateFile(fileName2);
	desc2 = HP_OpenFile(fileName2);

	while (!heapValues2.eof())
	{
		heapValues2 >> myRecord.id;
		heapValues2 >> myRecord.name;
		heapValues2 >> myRecord.surname;
		heapValues2 >> myRecord.city;

		HP_InsertEntry(desc2, myRecord);
	}

	
	HP_GetAllEntries(desc2, surname, (void*)surnameTest);
	HP_GetAllEntries(desc2, city,  (void*)cityTest);


	heapValues2.close();
	
	//Closing first file
	if (HP_CloseFile(desc2) == 0)
	{
		cout << "File : " << fileName2 << " closed." << endl;
	}
	

	cout << "Exiting main function, press ENTER... " << endl;
	cin.get();
	return 0;
}