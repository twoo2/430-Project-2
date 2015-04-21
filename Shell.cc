//CSE 430 - Operating Systems
//By: Tyler Woo - 1203788009
//Project 2 - Milestone Implementation
//Description: Basic implementation of pseudocode without the final components.
//Proof of concept program.
//Disclaimer: Used assignment #2 sample code files to help implement semaphores and 
//design the layout of the program.

#include	<pthread.h>
#include 	<stdio.h>
#include 	<stdlib.h>
#include	<unistd.h>
#include 	<semaphore.h>
#include	<iostream>

using namespace std;

int usernum; //amount of users
int userid; //number of user
sem_t mutex, usergo, tweetergo, streamergo, done; 
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
		sem_wait(&mutex); //start mutex preserve
		userid = i; //set user id
		printf("User has %d instantiated\n", userid); 
		printf("Commands read for user %d\n", userid);
		printf("Setting Handle...");
		sleep(0.5);
		printf("Start of Tweet...");
		sleep(0.5);
		printf("End of Tweet...");
		sleep(0.5);
		printf("Follow Tweet...Searching Tweeter\n");
		sem_post(&mutex); //end mutex preserve
		sem_post(&tweetergo); //signal tweeter to begin

		sleep(1);

		sem_wait(&done); //begin done if signaled
		sem_wait(&mutex); //start mutex preserve
		printf("Tweet printed, user reading...");
		sleep(0.5);
		printf("Start of Tweet 2...");
		sleep(0.5);
		printf("End of Tweet 2...");
		sleep(0.5);
		printf("Exiting user %d...\n", userid);
		sleep(0.5);
		sem_post(&mutex); //end mutex preserve
		sem_post(&done); //end done

	}
	printf("User thread ending...\n");
	pthread_exit(NULL); //end user thread
}

void *tweeter(void *arg)
{
	while(true)
	{
		sem_wait(&tweetergo); //start tweeter
		sem_wait(&mutex); //start mutex preserve
		printf("User %d checking Tweeter for tag\n", userid);
		sleep(0.5);
		printf("Sending request to Streamer for full tweet\n");
		sleep(0.5);
		sem_post(&mutex); //end mutex preserve
		sem_post(&streamergo); //signal streamer to begin
		if(userid == usernum - 1) //checks if no more users
		{
			printf("Tweeter thread ending...\n");
			pthread_exit(NULL); //end tweeter thread
		}
	}
}

void *streamer(void *arg)
{
	while(true)
	{
		sem_wait(&streamergo); //start streamer
		sem_wait(&mutex); //start mutex preserve
		printf("User %d has requested to check Streamer for tweet\n", userid);
		printf("Tweet found, printing tweet\n");
		sem_post(&mutex); //end mutex preserve
		sem_post(&usergo); //signal user to begin
		sem_post(&done); //signal done to begin
		if(userid == usernum - 1) //checks if no more users
		{
			sem_post(&tweetergo);
			printf("Streamer thread ending...\n");
			pthread_exit(NULL); //end streamer thread
		}
	}
}

int main()
{
	pthread_t idU, idT, idS; //create threads

	printf("Enter number of users: \n");
	scanf("%d", &usernum); //read in amount of users
	printf("Got here\n");
	printf("Entered number was: %d\n", usernum);

	sem_init(&usergo, 0, 1); //set usergo to 1
	sem_init(&tweetergo, 0, 0); //set tweetergo to 0
	sem_init(&streamergo, 0, 0); //set streamergo to 0
	sem_init(&mutex, 0, 1); //set mutex to 1
	sem_init(&done, 0, 0); //set done to 0

	pthread_create(&idU, NULL, user, NULL); //create user thread
	pthread_create(&idT, NULL, tweeter, NULL); //create tweeter thread
	pthread_create(&idS, NULL, streamer, NULL); //create streamer thread

	pthread_join(idU, NULL); //close user thread
	pthread_join(idT, NULL); //close tweeter thread
	pthread_join(idS, NULL); //close streamer thread

	pthread_exit(NULL); //exit program


}