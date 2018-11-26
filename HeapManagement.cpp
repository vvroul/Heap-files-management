#include <iostream>
#include <string>

extern "C"
{
	#include <cstdio>
	#include <cstdlib>
	#include <cstring>
	#include <string.h>
	#include "BF.h"
	#include "HeapInfo.h"
}

using namespace std;


//_____________________________________________________________________________________________________________________


int HP_CreateFile(char* fileName)
{
	int heapDesc, blockCount;
	void *block;
	char info[] = "HEAP_FILE";

	BF_Init();

	if (BF_CreateFile(fileName) < 0)
	{
		BF_PrintError("Error creating heap file!");
		return -1;
	}
	else
	{
		cout << "File :  " <<  fileName  << " , created successfully!" << endl;
	}

	if ((heapDesc = BF_OpenFile(fileName)) < 0)
	{
		BF_PrintError("Error opening file!");
		return -1;
	}

	if (BF_AllocateBlock(heapDesc) < 0)
	{
		BF_PrintError("Error allocating heap block!");
		return -1;
	}

	if ((blockCount = BF_GetBlockCounter(heapDesc)) < 0)
	{
		BF_PrintError("Error getting block counter!");
		return -1;
	}

	if (BF_ReadBlock(heapDesc, blockCount - 1, &block) < 0)
	{
		BF_PrintError("Error while reading heap file!");
		return -1;
	}

	memcpy(block, info, BLOCK_SIZE);

	if (BF_WriteBlock(heapDesc, blockCount -1 ) < 0)
	{
		BF_PrintError("Error while writing to heap file!");
		return -1;
	}

	if (BF_CloseFile(heapDesc) < 0)
	{
		BF_PrintError("Error while closing the heap file!");
		return -1;
	}


	return 0;
}


//_____________________________________________________________________________________________________________________


int HP_OpenFile(char* fileName)
{
	int heapDesc, blockCount;
	void* block;
	char element;
	char info[] = "HEAP_FILE";

	if (heapDesc = BF_OpenFile(fileName) < 0)
	{
		BF_PrintError("Error while opening heap file!");
		return -1;
	}
	else
	{
		cout << "File : " << fileName << " , is open." << endl << endl;
	}

	if ((blockCount = BF_GetBlockCounter(heapDesc)) < 0)
	{
		BF_PrintError("Error getting block counter!");
		return -1;
	}

	if (BF_ReadBlock(heapDesc, blockCount - 1 , &block) < 0)
	{
		BF_PrintError("Error while reading heap file!");
		return -1;
	}

	if ( memcmp (block ,info , sizeof(BLOCK_SIZE)) == 0)		//check for heap file
	{
		return heapDesc;
	}
	else
	{
		return -1;
	}
}


//_____________________________________________________________________________________________________________________


int HP_CloseFile(int fileDesc)
{

	if (BF_CloseFile(fileDesc) < 0)
	{
		BF_PrintError("Error while closing the heap file!");
		return -1;
	}

	return 0;
}


//_____________________________________________________________________________________________________________________


int HP_InsertEntry(int fileDesc, Record record)
{
	int blockCount;
	void* block;
	Record* myRecord = &record;
	static int num = 0;

	if ((blockCount = BF_GetBlockCounter(fileDesc)) < 0)
	{
		BF_PrintError("Error getting block counter!");
		return -1;
	}

	cout << "Block Number : "  <<  blockCount << endl;

	if (BF_ReadBlock(fileDesc, blockCount - 1, &block) < 0)
	{
		BF_PrintError("Error while reading a block!");
		return -1;
	}

	num++;
	cout << " Record Number : " << num << endl;
	/*
	if (num == 20)
	{
		cout << " Block is full! " << endl;
	}
	*/
	block = static_cast<char*>(block) + (num * sizeof(Record));

	if ((sizeof(Record) * num >= BLOCK_SIZE))
	{
		num = 0;
		if (BF_AllocateBlock(fileDesc) < 0)
		{
			BF_PrintError("Error allocating heap block!");
			return -1;
		}

		HP_InsertEntry(fileDesc, record);
	}
	else
	{
		memcpy(block, myRecord, sizeof(Record));

		if (BF_WriteBlock(fileDesc, blockCount - 1) < 0)
		{
			BF_PrintError("Error while writing to heap file!");
			return -1;
		}

		cout << "Entry inserted successfully! " << endl << endl;
	}

	return 0;
}


//_____________________________________________________________________________________________________________________


void HP_GetAllEntries(int fileDesc, char* fieldName, void* value)
{
	void* block;
	int readCount, blockCount , num;

	Record testRecord;
	Record * myRecord = &testRecord;
	char city[] = "city";
	char name[] = "surname";
	char surname[] = "name";
	char id[] = "id";
	char all[] = "all";

	if ((blockCount = BF_GetBlockCounter(fileDesc)) < 0)
	{
		BF_PrintError("Error getting block counter!");
		return;
	}


	for (int i = blockCount; i > 0; i-- )
	{

		num = 1;

		while (num * sizeof(Record) <= BLOCK_SIZE)
		{

			if (BF_ReadBlock(fileDesc,  blockCount - i , &block) < 0)
			{
				BF_PrintError("Error while reading a block!");
				return;
			}

			block = static_cast<char*>(block) + (num * sizeof(Record));
			num++;
			memmove(myRecord, block, sizeof(Record));
			if (strcmp(fieldName, id) == 0)
			{
				if (memcmp(&myRecord->id, value, sizeof(value)) == 0)
				{
					cout << "Printing id field of the record... " << endl;
					cout << " Record Number : " << num << "  of block number : " << blockCount - i << endl;
					cout << myRecord->id << endl << endl;
				}
			}
			else if (strcmp(fieldName, name) == 0)
			{
				if (strcmp(myRecord->name, (char*)value) == 0)
				{
					cout << "Printing name field of the record (id included)... " << endl;
					cout << " Record Number : " << num << "  of block number : " << blockCount - i << endl;
					cout << myRecord->name << endl;
					cout << myRecord->id << endl << endl;
				}
			}
			else if (strcmp(fieldName, surname) == 0)
			{
				if (strcmp(myRecord->surname, (char*)value) == 0)
				{
					cout << "Printing surname field of the record (id included)... " << endl;
					cout << " Record Number : " << num << "  of block number : " << blockCount - i << endl;
					cout << myRecord->surname << endl;
					cout << myRecord->id << endl << endl;
				}
			}
			else if (strcmp(fieldName, city) == 0)
			{
				if (strcmp(myRecord->city, (char*)value) == 0)
				{
					cout << "Printing city field of the record (id included)... " << endl;
					cout << " Record Number : " << num << "  of block number : " << blockCount - i << endl;
					cout << myRecord->city << endl;
					cout << myRecord->id << endl << endl;
				}
			}
			else if (strcmp(fieldName, all) == 0)
			{
				if (memcmp(&myRecord->id, value, sizeof(value)) == 0)
				{
					cout << "Printing all the records of the block! " << endl;
					cout << " Record Number : " << num << "  of block number : " << blockCount - i << endl;
					cout << myRecord->id << endl;
					cout << myRecord->name << endl;
					cout << myRecord->surname << endl;
					cout << myRecord->city << endl << endl;
				}
			}
			else
			{
				cout << "Wrong fieldName!! " << endl;
				return;
			}
		}
	}
}
