#include "Fast_Windows_Console.h"
#include "Stack_Str.h"
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;
#pragma warning(disable:4996)

int main()
{
	char *file_text = (char*)"Hello World";
	int array_size = 20;
	Stack_Char::Stack_Str c;
	Stack_Char::init(&c, array_size);
	ifstream my_file;
	my_file.open("datafile.txt");

	if (!my_file)
	{
		cerr << "Unable to open file datafile.txt";
		exit(1); //call system to stop
	}

	while (my_file >> file_text)
	{
		char *temp = new char[strlen(file_text) + 1];
		strcpy(temp, file_text); //copy the ttv_message
		Stack_Char::push(&c, temp);
	}

	my_file.close();

	cout << Stack_Char::pop(&c) << endl;

	getchar();
}