// Ncurses Libraries 
#include <ncurses.h>
#include <curses.h>
#include <string.h>
#include <stdio.h>

// OpenSplice Libraries
#include <signal.h>
#include <assert.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include "DDSEntityManager.h"
#include "ccpp_SuperChat.h"

#include "chatroom_data.h"
#include "message_data.h"
#include "user_data.h"

// This code is derived from the PrismTech HelloWorld examples
using namespace DDS;
using namespace SuperChat;
using namespace std;
// these classes abstract out the sending and recieving of topics
// perhaps with some cleverness, generics could be used to eliminate
// some of the code 


// Function Prototypes
void homepage(WINDOW **chatrooms, char* username);
void enter_chatroom(int chatroom);
void find_user(char* username);
void updateUsers(WINDOW *usersOnline, char* newUser, int chatroomIndex);
void * openSplice(void *data);
void updateMessages(int chatroom, char* message, char* username);
void * updateMessageBoard(void * data);

int whatever;

char chatUsers[10][15][8];
int numberOfUsers[10];
int newUserFlag = 0;

char chatroomNames[10][CHATROOM_NAME_MAX] = {"PUBLIC", "Chatroom A", "Chatroom B", "Chatroom C", "Chatroom D", "Chatroom E", "Chatroom F", "Chatroom G", "Chatroom H", "Chatroom I"};
int chatroomNameChangeFlag = -1;
int messageCounts[10];

char chatroomCurrentMessages[10][4][145];
char messageUsers[10][4][8];

int currentChatroom = -1;

// NEED TO MOVE FROM GLOBAL created for testing purposes
// found in function "enter_chatroom"
WINDOW *messageBoard; 	
char username[8] = "Alex123";
char username1[8] = "Devy78";
char username2[8] = "Tham98";
char username3[8] = "Ody56";
int messageCount = 0;
int boardRows, boardCols;
//WINDOW *inputField;

// Threads definition and setup
//pthread_t tid;			// thread ID
//pthread_attr_t attr; 	// Create attributes 
//pthread_attr_init(&attr); // initialize attributes 

// Global flags
bool isMessageEmpty;

// instantiate classes
chatroom_data chatRoom ( (char*) "chatroom" );
user_data User ( (char*) "user" );
message_data Message ( (char*) "msg" );

// SuperChat Topics
message msg;	// create a message topic
chatroom chat;
user user1;

// Stores a list of messages 
message_list_t  messageList;
chatroom_list_t chatroomList;
user_list_t userList;

int main(int argc, char *argv[])
{


    char username[20];
    int ch, i;


    WINDOW *chatrooms[10];
    WINDOW *publicwindow;
    char chatroom_names[9][24];


    initscr();                      /* Start curses mode            */


    /* Create the homepage */
	keypad(stdscr, TRUE);	/* We get F1, F2 etc..		*/
	cbreak();		

    /* Looks for users info in a file, If the file does not exist the user is new and a file is created */
    find_user(username);

	refresh();


	homepage(chatrooms, username);


	//enter_chatroom(2);	
	doupdate();

	// Threade declaration
  	pthread_t tid1;      // thread ID

  	pthread_create(&tid1, NULL, openSplice, &whatever);    // create a thread for opensplice

	do
	{
		isMessageEmpty = 1;			// turns on message forever until user leave chat room

		ch = getch();

		for(int i = 0; i < 10; i++)
		{

			if(ch == KEY_F(i))
			{
				enter_chatroom(i-2);
				break;
			}

		}
		homepage(chatrooms, username);

	}while(ch != KEY_F(10));

	//pthread_exit(0); // this tells pthread_join that the thread is finish 
	endwin();
	
	return 0;
}
void homepage(WINDOW **chatrooms, char* username)
{


	int i, counter = 0;
	int height = (LINES/3)-2;
    int width = (COLS/5)-2;
	char title[]="SuperChat";


	clear();

	mvprintw(0,0,"%s",title);
	mvprintw(3, 0 , "Welcome, %s", username);
	mvprintw((LINES/3)-1,0,"%s","Chatrooms:");



			/* Create the lines  */
	mvhline(LINES/3, 0, ACS_HLINE, COLS);
	mvhline((LINES/3)*2, 0, ACS_HLINE, COLS);

	mvvline((LINES/3)+1, COLS/5, ACS_VLINE, LINES);
	mvvline((LINES/3)+1, 2*COLS/5, ACS_VLINE, LINES);
	mvvline((LINES/3)+1, 3*COLS/5, ACS_VLINE, LINES);
	mvvline((LINES/3)+1, 4*COLS/5, ACS_VLINE, LINES);

	refresh();

	

	for(i=0; i<5; i++)
	{

		chatrooms[i] = newwin((LINES/3)-2, (COLS/5)-1, (LINES/3)+1, (i*COLS/5)+1);
		//box(chatrooms[i], 0, 0);
		wrefresh(chatrooms[i]);

		if(i==0)
			mvwprintw(chatrooms[i], 0, 0, "PUBLIC\n<F2>");

		else
			mvwprintw(chatrooms[i], 0, 0, "%s\n<F%d>", chatroomNames[i], i+2);

		wrefresh(chatrooms[i]);

	}	

	
	for(i=5; i<10; i++)
	{

		chatrooms[i] = newwin((LINES/3)-2, (COLS/5)-1, (2*LINES/3)+1, (counter*COLS/5)+1);
		//box(chatrooms[i], 0, 0);
		wrefresh(chatrooms[i]);

		mvwprintw(chatrooms[i], 0, 0, "%s\n<F%d>", chatroomNames[i], i+2);

		wrefresh(chatrooms[i]);
		counter++;

	}

	doupdate();




	return;

}


void enter_chatroom(int chatroom)
{

	int ch;
	//int ch, boardRows, boardCols, messageCount;
	char myMessage[144];
	int lines = (LINES - (3*LINES/10)) -1;
	int cols = (3*COLS/5) ;
	pthread_t tid2; 

	currentChatroom = chatroom;

	//WINDOW *messageBoard;
	WINDOW *inputField;
	WINDOW *usersOnline;
	

	/* Temporary Variables */
	//char username[8] = "Alex123";
	//char username1[8] = "Devy78";
	//char username2[8] = "Tham98";
	//char username3[8] = "Ody56";


	clear();


	mvvline(0, 3*COLS/5, ACS_VLINE, LINES);
	mvhline(LINES/10, 0, ACS_HLINE, 3*COLS/5);
	mvhline(4*LINES/5, 0, ACS_HLINE, 3*COLS/5);

	refresh();


	


	pthread_create(&tid2, NULL, updateMessageBoard, &whatever);    // create a thread for opensplice


	/* Side window containing the current online users */
	usersOnline = newwin(LINES, COLS-(3*COLS/5)-1, 0, (3*COLS/5)+1);
	mvwprintw(usersOnline, (4*LINES/5), 0, "%s", "F1 = Enter message");
	mvwprintw(usersOnline, (4*LINES/5)+1, 0, "%s", "F2 = Change chatroom name");
	mvwprintw(usersOnline, (4*LINES/5)+2, 0, "%s", "F5 = Exit chatroom");


	wrefresh(usersOnline);



	doupdate();

	messageCount = 0;
	int messageLength;

/* Loop constantly checking what action the user wants to take */
	do
	{

		inputField = newwin((LINES/5), (3*COLS/5), (4*LINES/5)+1, 0);
		wprintw(inputField, "Press F4 to Enter a Message...");
		wrefresh(inputField);

		doupdate();

		ch = getch();

		switch(ch)
		{
			/* Case that allows the User to post a message to the chatroom's message board. 
			   Pushes the messages up
			*/
			case KEY_F(4):

				wclear(inputField);
				wgetstr(inputField, myMessage);
				wclear(inputField);

				messageLength = strlen(myMessage);
				myMessage[messageLength] = '\0';

				strcpy(msg.message, myMessage);
				msg.chatroom_idx = chatroom;
        		
        		Message.send(msg);

				break;


			case KEY_F(2):
			{
				char newName[15];
				mvprintw(1, 0, "%s", "New chatroom name: ");
				getstr(newName);

				strcpy(chat.chatroom_name, newName);
				chat.chatroom_idx = chatroom;

				chatRoom.send(chat);

				

				


				doupdate();
				break;
			}

		}




	}while(ch != KEY_F(5));

	isMessageEmpty = 0;

	currentChatroom = -1;
	chatroomNameChangeFlag = -1;
	pthread_join(tid2, NULL);
	clear();
	refresh(); 


	return;

}

/*Function that looks for "superchatinfo.txt" file with user info. 
  If one isnt found, then one is created */
void find_user(char* username)
{
	FILE* oldfile;
	FILE* newfile;
	char buffer[20];

	if ( (oldfile = fopen("superchatinfo.txt", "r" )) == NULL )
	{	
		newfile = fopen("superchatinfo.txt", "w");
		mvprintw(0, 0, "New User\n");
		mvprintw(1, 0, "%s", "Enter a username: ");
		getstr(username);

   		fprintf(newfile, "%s\n", username);

/* More user information will go here, for now just keep the username in this file. */
   		fclose(newfile);

	}

	else
	{
		fgets(buffer, 20, oldfile);
		strcpy(username, buffer);
		fclose( oldfile );		
	}


}



/* Function that updates the list of Online users in the current chatroom */
void updateUsers(WINDOW *usersOnline, char* newUser, int chatroomIndex)
{
	wclear(usersOnline);
	mvwprintw(usersOnline, 0, 0 ,"%s", "Users In Chat:");

	strcpy(chatUsers[chatroomIndex][numberOfUsers[chatroomIndex]],newUser);

	numberOfUsers[chatroomIndex]++;

	for(int i = 0; i < numberOfUsers[chatroomIndex]; i++)
		mvwprintw(usersOnline, i+1 , 0 ,"%s", chatUsers[chatroomIndex][i]);



	return;

}



void updateMessages(int chatroom, char* message, char* username)
{


	if(messageCounts[chatroom] < 4)
		messageCounts[chatroom]++;

	for(int i=(messageCounts[chatroom]-1); i > 0; i--)
	{	
		strcpy(chatroomCurrentMessages[chatroom][i], chatroomCurrentMessages[chatroom][i-1]);
		strcpy(messageUsers[chatroom][i], messageUsers[chatroom][i-1]);
	}

	strcpy(chatroomCurrentMessages[chatroom][0], message);
	strcpy(messageUsers[chatroom][0], username);

	return;
}


void * updateMessageBoard(void * data)
{
	/* Window where the messages will be posted */
	int boardRows, boardCols;
	int lines = (LINES - (3*LINES/10)) -1;
	int cols = (3*COLS/5) ;
	WINDOW *messageBoard;
	WINDOW *titleWin;

	/* Top window containing the chatroom name */
	titleWin = newwin((LINES/10), cols, 0, 0);
	wprintw(titleWin, "%s", chatroomNames[currentChatroom]);
	wrefresh(titleWin);

	messageBoard = newwin( lines , cols , (LINES/10)+1, 0);
	getmaxyx(messageBoard, boardRows, boardCols);
	wrefresh(messageBoard);


	while(currentChatroom != -1)
	{	

		for(int i = 0; i < messageCounts[currentChatroom]; i++)
		{
			
			if(i == 0)
				mvwprintw(messageBoard, 4*boardRows/5, 1, "%-8s: %s", messageUsers[currentChatroom][i], chatroomCurrentMessages[currentChatroom][i]);
			if(i == 1)
				mvwprintw(messageBoard, 3*boardRows/5, 1, "%-8s: %s", messageUsers[currentChatroom][i], chatroomCurrentMessages[currentChatroom][i]);
			if(i == 2)
				mvwprintw(messageBoard, 2*boardRows/5, 1, "%-8s: %s", messageUsers[currentChatroom][i], chatroomCurrentMessages[currentChatroom][i]);
			if(i == 3)
				mvwprintw(messageBoard, boardRows/5, 1, "%-8s: %s", messageUsers[currentChatroom][i], chatroomCurrentMessages[currentChatroom][i]);
				
		}

		wrefresh(messageBoard);
		doupdate();


		if(currentChatroom == chatroomNameChangeFlag)
		{

			wclear(titleWin);
			wprintw(titleWin, "%s", chatroomNames[currentChatroom]);
			wrefresh(titleWin);

			doupdate();

			chatroomNameChangeFlag = -1;


		}

	}

	pthread_exit(0);


}


// this is the  thread that will be running opensplice 
void * openSplice(void *data)
{
	
	// need to pass right data; we could create "data" as a struct with all the information opensplice needs
	//int *d = (int *)data;
	//int da = *d;
	wclear(messageBoard);
	while(1)
	{
		// receive all the messages sent. chatroomList stores all messages 
		Message.recv(&messageList);
		while(messageList.size() != 0)
		{

			updateMessages(messageList[0].chatroom_idx, messageList[0].message, "Alex123");
			messageList.erase(messageList.begin());

		}


		chatRoom.recv(&chatroomList);
		while(chatroomList.size() != 0)
		{
			chatroomNameChangeFlag = chatroomList[0].chatroom_idx;
			strcpy(chatroomNames[chatroomList[0].chatroom_idx], chatroomList[0].chatroom_name);
			chatroomList.erase(chatroomList.begin());


		}

	}
}





















