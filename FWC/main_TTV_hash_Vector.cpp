#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Twitch_core.h"
#include "Fast_Windows_Console.h"
#include "Hash_Example.h"
#include "Stack_Str.h"
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;
#pragma warning(disable:4996)

int main(int argc, char **argv)
{
	//initialize network
	Twitch::startup();

	char *username = (char*)"thejuicyjames";
	//get this token from https://twitchapps.com/tmi/ and keep it safe
	//remove the oauth: part when you copy paste
	char *token = (char*)"ggj6co9kd5p7qzp0omhfojgxvvi3qe";

	//init connection data
	Twitch::Connection connection;
	Twitch::init(&connection, username, token);

	//make TCP connection to twitch and login
	Twitch::connect(&connection);

	//join a channel
	//pass channel name
	printf("connecting to channels\n");
	Twitch::join_Channel(&connection, "thejuicyjames");

	//initialize message table
	//will contaain incoming message list from all connected channels
	Twitch::Message::Table incoming;
	Twitch::Message::init(&incoming);

	//hash table (usernames)
	unsigned int prime_number = 200117;
	Closed_Hash_Linear_Probe_Modular_Indexing::Table ttv_username_table;
	Closed_Hash_Linear_Probe_Modular_Indexing::init(&ttv_username_table, prime_number);

	int array_size = 100; //stack (char*)user_messages

	//ofstream my_file; //create a file for the user if it does not exsist

	//my_file.open("_ttv_user_messages.txt");

	printf("chat log\n");
	for (;;)
	{
		unsigned int timestamp = clock();
		incoming.n_count = 0;

		//collect all messages from all channels
		Twitch::communicate(&incoming, &connection, timestamp);
		if (connection.active == false)
		{
			printf("connection was closed!\n");
			break;
		}
		
		//print received messages from all channels
		for (int i = 0; i < incoming.n_count; i++)
		{
			printf("%s@%s|(%.2f)-> %s\n", incoming.username[i], incoming.channel[i], (double)timestamp / CLOCKS_PER_SEC, incoming.message[i]);

			//my_file.open(std::string(incoming.username[i]) + "_ttv_user_messages.txt");

			Stack_Char::Stack_Str *ttv_user_messages = new Stack_Char::Stack_Str; //create new Stack_Str

			Stack_Char::init(ttv_user_messages, array_size);

			char *ttv_username = (char*)incoming.username[i]; //get username

			unsigned long long ttv_user_key = Encryption::encrypt_djb2(ttv_username, strlen(ttv_username)); //encrpyt username

			char *ttv_user = (char*)Closed_Hash_Linear_Probe_Modular_Indexing::get(&ttv_username_table, ttv_user_key); //get user from hash table (ttv_username_table)

			if (ttv_user == NULL)
			{
				Closed_Hash_Linear_Probe_Modular_Indexing::set(&ttv_username_table, ttv_user_key, &ttv_user_messages); //set username
			}

			char *ttv_message = (char*)incoming.message[i]; //get message

			char *temp = new char[strlen(ttv_message) + 1]; //create temp char array

			strcpy(temp, ttv_message); //copy the ttv_message

			Stack_Char::push(ttv_user_messages, temp); //store the temp message into the vec
			
			//my_file << temp << "\n"; //write mssage to file
		}//end of for(print)
	}//end of for (;;)
	//my_file.close(); //close user file
	getchar();
}