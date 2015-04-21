//CSE 430 - Operating Systems
//By: Tyler Woo - 1203788009
//Project 2 - Milestone Pseudocode
//Description: Pseudocode of the final program, basic depiction of how the program
//will ultimately function in the end.
//Disclaimer: Used assignment #2 sample code files to help present semaphores and 
//design the layout of the program.

#include	<pthread.h>
#include 	<stdio.h>
#include 	<stdlib.h>
#include	<unistd.h>
#include 	<semaphore.h>
#include	<String.h>

#define BUFFER_SIZE 1400; //set to large enough size for 10 tweets
using namespace std;

semaphore mutex, usergo, tweetergo, streamergo, done;
//mutex is a signal to preserve a section of execution
//usergo signals the user thread to begin and/or end
//tweetergo signals the tweeter thread to begin and/or end
//streamergo signals the tweeter thread to begin and/or end
//done signals that the streamer has completed its operation
int usernum; //integer signifying which user is in operation
String buf[BUFFER_SIZE]; //buffer to store tweets

void *user() //read in user and all relevant information
{
	for(int i = 0; i < usernum; i++) //cycle through users
	{
		wait(usergo); //begin user
		wait(mutex); //start mutex preserve

		getline(cin, str, '\n'); //getline and store into buffer
		buf = str; //set the read string into appropriate buffer space
		if(Handle()) //checks if handle is read
		{
			do(set handle);
		}
		if(Start()) //checks if start is read
		{
			do(store topic to tweeter);
		}
		if(End()) //checks if end is read
		{
			do(signal end of tweet);
		}
		if(Follow()) //checks if follow is read
		{
			do(retrieve topic tweets from tweeter);
		}

		post(mutex); //end mutex preserve
		post(tweetergo); //signal tweeter to begin

		wait(done); //start section if done is signaled
		wait(mutex); //start mutex preserve

		if(Read()) //checks if read is read
		{
			do(sleep system);
		}
		if(Start()) //checks if start is read
		{
			do(store topic to tweeter)
		}
		if(End()) //checks if end is read
		{
			do(signal end of tweet);
		}
		if(Exit()) //checks if exit is read
		{
			do(end user tweet);
		}

		post(mutex); //end mutex preserve
		post(done); //end done section
	}
	parexit(NULL); //exit thread
}

void *tweeter() //repository for categorized #hashtags
{
	while(true)
	{
		wait(tweetergo); //start tweeter
		wait(mutex); //start mutex preserve

		if(Follow()) //checks if follow is read
		{
			do(find tweet associated with topic)
		}
		if(Start()) //checks if start is read
		{
			do(store topic and link with tweet)
		}

		post(mutex); //end mutex preserve
		post(streamergo); //end streamer
	}
}

void *streamer() //tweet inherited from user is sent to streamer (only one at a time per user)
{
	while(true)
	{
		wait(streamergo); //start streamer
		wait(mutex); //start mutex preserve

		parse(buf); //for every user entered tweet
		parse(buf_followed); //buffer for followed tweets

		if(Follow()) //checks if follow read
		{
			do(print tweet associated with topic)
		}
		if(Start()) //checks if start is read
		{
			do(store tweet in buffer)
		}

		post(mutex); //end mutex preserve
		post(user); //reset user
		post(done); //set done to ready
	}
}

int main()
{
	read(usernum); //read in amount of users

	init(usergo, 1); //set usergo to 1
	init(tweetergo, 0); //set tweetergo to 0
	init(streamergo, 0); //set streamergo to 0
	init(done, 0); //set done to 1
	init(mutex, 1); //set mutex to 1

	parbegin(user(),tweeter(),streamer()); //begin threads

	parjoin(user(),tweeter(),streamer()); //chlose threads
	parexit(NULL); //exit program
}