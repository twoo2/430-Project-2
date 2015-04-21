//CSE 430 - Operating Systems
//By: Tyler Woo - 1203788009
//Project 2 - Final Implementation
//Description: Implementation of pseudocode 
//Disclaimer: Used assignment #2 sample code files to help implement semaphores and 
//design the layout of the program. Used Cplusplus.com to help with struct information
/*
Description and Analysis:
1.	The project turned out much more poorly than expected. Initially during the trial runs 
without the key information the semaphores acted as intended by running through all the 
necessary iterations. However, once the commands were implemented it became clear that the 
second thread would not run and therefore would ‘drop off’. Although it compiled correctly 
the program does not continue due to either timing out or a mismatch in semaphore signaling.

2.	The data structure used to maintain the repository was a simple 2D array. The column 
portion of the array was the user that the tweet was assigned to, the row portion of the 
array was the tweet itself. The array was allocated to [NUMBER OF USERS][150]. 150 was set 
because it was the estimated max size of any tweet.

3. The methodology undertaken for the implementation of my pseudocode was to first 
define what needed to be read in by the pthread. By first understanding how the text 
files were laid out it became easier to decide the layout of the command readings.
*/

#include	<pthread.h>
#include 	<stdio.h>
#include 	<stdlib.h>
#include	<unistd.h>
#include 	<semaphore.h>
#include	<iostream>
#include	<string>
#include	<sstream>
#include	<fstream>

#define USERST 10 //total amount of users possible
#define TWEETL 200 //ultimate length of tweet

using namespace std;

const int TAGS = 5;
const int HANDLES = 8;
const int TWEET = 10;
int usernum; //amount of users
int userid; //number of user
sem_t mutex, usergo, usergostart, usergofollow, tweetergostart, tweetergofollow, streamergo, readgo, notfoundgo; 
char tweets[USERST][TWEETL]; //star or no star?
int index; //index used for the location of the characters in the tweet
int j; //used for traversing through whole tweet
int k = 0; //used for location in tag array
int n = 0; //used for location in tweet array
int loc; //location of tweet
int count = 0;
int tempj; //temporary j to preserve its value
int tempk;
bool quitter = false;

struct information
{
	int handlelength; 
	int tweetlength; 
	int hashtaglength; 
	char handle[20]; //ESTABLISH HERE OR LATER?
	char tweet[150];
	char hashtag[20];
	string shandle[HANDLES];
	string shashtag[TAGS];
	string stweet[TWEET];
	string strtwetemp; //maybe these shouldn't be in the struct
	string strtagtemp;
	string strhantemp;
} info;

//mutex is a signal to preserve a section of execution
//usergo signals the user thread to begin and/or end
//tweetergo signals the tweeter thread to begin and/or end
//streamergo signals the tweeter thread to begin and/or end
//done signals that the streamer has completed its operation

void *user(void *arg)
{
	for(int i = 0; i < usernum; i++) //cycle through amount of users set
	{

		sem_wait(&usergo); //start user thread
		sem_wait(&mutex);

		userid = i; //set user id
		index = 0; //reset index to 0
		string st0 = "user";
		string att = ".txt";
		stringstream ss; //begin stringstream
		ss << i; //set file limit
		string filename = st0 + ss.str() + att; //set filename with appropriate value
		ifstream infile;
		infile.open( filename.c_str(), ios::in ); //open file to character string
		cout << filename << "\n"; //print out appropriate filename
		
		while (!infile.eof()) //reads through whole file
		{
			infile >> tweets[i][index]; //set characters to 2D array
			cout << tweets[i][index]; //print out character element

			index++; //index should be the size of the full tweet
		}
		infile.close(); //close file open

		info.handlelength = 8; //location of handle name set to two characters after @ 
		while(!isupper(tweets[i][info.handlelength])) //checks for capital letter (stops after capital is read)
		{
			info.handle[0] = tweets[i][7]; //always sets first letter of handle to spot regardless of capital
			info.handle[count+1] = tweets[i][info.handlelength]; //set tweet char to handle locations (should start at 9 and go up)
			count++;
			info.handlelength++; //increment handle length (whole thing should end once captial is found)
		}
		cout << "\n"; //whole thing should be printed
		info.handlelength = info.handlelength - 7; //should revert it to actual size of handle 
		cout << "Handle length is: " << info.handlelength << "\n";


		string str1(info.handle); //set string to the handle that was read into
		info.strhantemp = str1;
		info.shandle[userid] = info.strhantemp;
		
		cout << "User " << userid << " is instantiated with Handle: " << info.shandle[userid] << "\n";

		j = 0; //reset j to 0

		if(tweets[i][info.handlelength + 7] == 'S') //should it be 6 or 7 (trying to point to character right after handle name)
		{
			j = j + info.handlelength + 7 + 6; //should point to spot right after #			
			sem_post(&streamergo);
		}
		if(tweets[i][info.handlelength + 8] == 'F') //checks for follow
		{
			j = j + info.handlelength + 7 + 7; //should point to spot right after #
			sem_post(&tweetergofollow);
		}

		sem_wait(&usergostart); //coming back from tweeter start (tweet set to location n)

		for(int a = j; a < index; a++) 
		{
			if(tweets[i][a] == 'S' && tweets[i][a+1] == 't' && tweets[i][a+2] == 'a' && tweets[i][a+3] == 'r'&& tweets[i][a+4] == 't') 
			{
				a = a + 6; //if its pointing at 's' how much should I add to get it to point at the first 
				j = a;
				cout << "Later Start has been seen\n";
				sem_post(&streamergo);
			}
			if(tweets[i][a] == 'F' && tweets[i][a+1] == 'o' && tweets[i][a+2] == 'l' && tweets[i][a+3] == 'l' && tweets[i][a+4] == 'o'&& tweets[i][a+5] == 'w') 
			{
				a = a + 7;
				j = a;
				cout << "Later Follow has been seen\n";
				cout << "j is: " << j << "\n"; 
				sem_post(&tweetergofollow);
			}
			sem_wait(&readgo);
			if(tweets[i][a] == 'R') 
			{
				cout << info.stweet[loc] << "\n";
				a = a + 4; 
				j = a;
				cout << "Command: Read is read to user " << userid << " is reading...Pausing system\n";
				sleep(5);
				cout << "Continuing system for user " << userid << "\n";
				if(tweets[i][a] == 'S' && tweets[i][a+1] == 't' && tweets[i][a+2] == 'a' && tweets[i][a+3] == 'r'&& tweets[i][a+4] == 't') 
				{
					a = a + 6;
					j = a;
					cout << "Late late Start has been seen\n";
					sem_post(&streamergo);
				}
				if(tweets[i][a] == 'F' && tweets[i][a+1] == 'o' && tweets[i][a+2] == 'l' && tweets[i][a+3] == 'l' && tweets[i][a+4] == 'o'&& tweets[i][a+5] == 'w') 
				{
					a = a + 7;
					j = a;
					cout << "Late late Follow has been seen\n";
					cout << "j is: " << j << "\n"; //86 for user0
					// 
					sem_post(&tweetergofollow);
				}
			}
		}
		
		sem_wait(&usergofollow);
		cout << "here\n";
		sem_post(&mutex);
		cout << info.stweet[loc] << "\n";
	}
	printf("User thread ending...\n");
	pthread_exit(NULL); //end user thread
}

void *tweeter(void *arg)
{
	while(true)
	{
		//STREAM SERVICE FROM START
		sem_wait(&tweetergostart); //start tweeter

		cout << "Entered Tweeter: Streaming in tweet from Streamer\n";

		string str2(info.hashtag); //set tag array to tag string
		info.strtagtemp = str2;
		info.shashtag[k] = info.strtagtemp; //set tag to specific location in tag array
		cout << "Tag: #" << info.shashtag[k] << " saved to location '" << k << "' in repository\n";

		string str3(info.tweet); //set tweet array to tweet string
		info.strtwetemp = str3;
		info.stweet[n] = info.strtwetemp;
		cout << "The Tweet is: " << info.stweet[n] << "\n";
		cout << "Tweet: '" << n << "'" << " saved to repository\n";
		loc = n;
		k++;
		n++; //increment to next slot

		if(tweets[userid][j] == 'E')
		{
			cout << "Command: Exit is read to user " << userid << "\n";

			printf("User thread ending...\n");
			printf("Tweeter thread ending...\n");
			printf("Streamer thread ending...\n");
			pthread_exit(NULL);

			sem_post(&usergofollow);
		}
		
		sem_post(&usergostart);
		//END STREAM SERVICE FROM START

		//STREAM SERVICE FROM FOLLOW
		sem_wait(&tweetergofollow); //start tweeter

		tempk = k; //preserve k value
		tempj = j; //preserve j value

		printf("Command: Follow is read for user %d\n", userid);

		//PROMPT TWEETER TO TRANSFER COMPLETE TWEETS THAT MATCH TAG
		for(int y = 0; y < info.hashtaglength; y++)
		{
			info.hashtag[y] = '\0';
		}
		info.hashtaglength = 0;
		while(!isupper(tweets[userid][tempj])) //still needs to read in tag to check it
		{
			info.hashtag[info.hashtaglength] = tweets[userid][tempj]; //set tag to tag array
			info.hashtaglength++; //increment location
			tempj++; //increment location
		}
		string str4(info.hashtag); //set tag array to tag string
		info.strtagtemp = str4;
		
		cout << "Looking for Tag: #" << info.strtagtemp << ", signaling tweeter\n";

		for(int x = 0; x < tempk; x++) //search through tag repository
		{
			if(info.strtagtemp == info.shashtag[x])
			{
				cout << "Tag: #" << info.shashtag[x] << " found for user " << userid << ", streaming tweet to user\n";
				k = tempk; //re-establish k value
				j = tempj; //re-establish j value
				
				if(tweets[userid][j] == 'R') 
				{
					cout << info.stweet[loc] << "\n";
					j = j + 4; //if its pointing at 's' how much should I add to get it to point at the first 
					cout << "Command: Read is read to user " << userid << " is reading...Pausing system\n";
					sleep(5);
					cout << "Continuing system for user " << userid << "\n";
					if(tweets[userid][j] == 'S') 
					{
						j = j + 6; 
						cout << "Late late Start has been seen\n";
						sem_post(&streamergo);
					}
					if(tweets[userid][j] == 'F') 
					{
						j = j + 7; 
						cout << "Late late Follow has been seen\n";
						sem_post(&tweetergofollow);
					}
				}
			}
		}
		//END STREAM SERVICE FROM FOLLOW
		cout << "Tag not found, returning to user thread\n";

		k = tempk;
		j = tempj + 4; //re-establish j value (including Read characters)

		if(tweets[userid][j] == 'S') 
		{
			j = j + 6; 
			cout << "Late late Start has been seen\n";
			sem_post(&streamergo);
		}
		if(tweets[userid][j] == 'F') 
		{
			j = j + 7; 
			cout << "Late late Follow has been seen\n";
			sem_post(&tweetergofollow);
		}

		if(quitter) //checks if no more users
		{
			printf("Tweeter thread ending...\n");
			pthread_exit(NULL); //end tweeter thread
		}
	}
}

void *streamer(void *arg) //J IS CORRECT HERE
{
	while(true)
	{
		//READING IN TAG AND TWEET
		sem_wait(&streamergo); //start streamer

		tempj = j; //preserve j value

		printf("Command: Start is read for user %d\n", userid);
		for(int y = 0; y < info.hashtaglength; y++)
		{
			info.hashtag[y] = '\0';
		}
		info.hashtaglength = 0;
		while(!isupper(tweets[userid][tempj])) //checks for uppercase to set tag
		{
			info.hashtag[info.hashtaglength] = tweets[userid][tempj]; //set tag to tag array
			info.hashtaglength++; //increment location
			tempj++; //increment location (should end up at the start of the tweet)
		}

		info.tweetlength = 0;
		while(tweets[userid][tempj] != 'E')
		{
			info.tweet[info.tweetlength] = tweets[userid][tempj]; //set tag to tag array
			info.tweetlength++; //increment location
			tempj++; //increment location (should end up at the start of End)
		}

		tempj = tempj + 4 + info.hashtaglength; //maybe should be 3 or 4 (trying to point after whole hashtag) //MIGHT BE OKAY, DOUBLE CHECK
		printf("Command: End is read for user %d, streaming tweet to tweeter\n", userid);

		j = tempj; //re-establish j

		sem_post(&tweetergostart); //signal tweeterstart to begin
		//END READING IN TAG AND TWEET

		if(quitter) //checks if no more users
		{
			printf("Streamer thread ending...\n");
			pthread_exit(NULL); //end streamer thread
		}
	}
}

int main()
{
	pthread_t idU, idT, idS; //create threads

	printf("Enter number of users (8 or less): \n");
	scanf("%d", &usernum); //read in amount of users
	if(usernum > 8)
	{
		cout << "Too many users entered please try again\n";
		printf("Enter number of users (8 or less): \n");
		scanf("%d", &usernum);
	}
	printf("Entered number was: %d\n", usernum);

	sem_init(&usergo, 0, 1); //set usergo to 1
	sem_init(&usergostart, 0, 0); //set usergostart to 0
	sem_init(&usergofollow, 0, 0); //set usergofollow to 0
	sem_init(&tweetergostart, 0, 0); //set tweetergostart to 0
	sem_init(&tweetergofollow, 0, 0); //set tweetergofollow to 0
	sem_init(&streamergo, 0, 0); //set streamergo to 0
	sem_init(&readgo, 0, 0);
	sem_init(&notfoundgo, 0, 0);
	sem_init(&mutex, 0, 1); //set mutex to 1

	pthread_create(&idU, NULL, user, NULL); //create user thread
	pthread_create(&idT, NULL, tweeter, NULL); //create tweeter thread
	pthread_create(&idS, NULL, streamer, NULL); //create streamer thread

	pthread_join(idU, NULL); //close user thread
	pthread_join(idT, NULL); //close tweeter thread
	pthread_join(idS, NULL); //close streamer thread

	pthread_exit(NULL); //exit program


}