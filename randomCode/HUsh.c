
/*
Question2
Mudasir Hanif Shaikh (ms03831) and Kainat Abbasi (ka04051)
CS 2021, Habib University
Assignment 2, OS, Fall 2019
*/


#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int getStrSize(char** str){
	int size = 0;
	while (str[size] != 0) size++;
    size++; //(for null termination)
    return size;
}


int getSizeCharArray(char* str){
	int size = 0;
	while (str[size] != 0) size++;
    size++; //(for null termination)
    return size;
}


int getCountOfSubstrings1(char** const str, const char* inpDelimiter){
	int cDelimiter = 0;
	int i;
	int strSize = getStrSize(str);
	for (i = 0; i < strSize; i++) {
    	if (str[i] == inpDelimiter) cDelimiter++;
    } return cDelimiter + 2; // "A & b" : delimiter count = 1, substrings = 2, additional 1 for null terminating character; 
}

int getCountOfSubstrings(char* const str, const char inpDelimiter){
	int cDelimiter = 0;	int i;
	int strSize = getSizeCharArray(str);
	for (i = 0; i < strSize; i++) {
    	if (str[i] == inpDelimiter) cDelimiter++;
    } return cDelimiter + 2; // "A & b" : delimiter count = 1, substrings = 2, additional 1 for null terminating character; 
}


char** split(char* str, const char inpDelimiter, int spaces)
{
    char delimiter[3];
    delimiter[0] = inpDelimiter;
    delimiter[1] = '\n';
    delimiter[2] = 0;
    int substrings = getCountOfSubstrings(str, inpDelimiter);
    int sizeChar = sizeof(char*);
    char ** substr = malloc(substrings*sizeChar); 
    int i;
	substr[0] = strtok(str, delimiter);
	//printf("%s \nsubtr0 ", substr[0]);
	for (i = 1; i < substrings; i++)
	{
		substr[i] = strtok(NULL, delimiter);
		
	}
	return substr;
}


char** split1(char* str, const char * inpDelimiter, int spaces)
{
    int substrings = getCountOfSubstrings1(&str, inpDelimiter);
    int sizeChar = sizeof(char*);
    char ** substr = malloc(substrings*sizeChar); 
    int i;
	substr[0] = strtok(str, inpDelimiter);
	for (i = 1; i < substrings; i++)
	{
		substr[i] = strtok(NULL, inpDelimiter);
	}
	return substr;
}

struct node {
   int PID;
   char* name;
   struct node *next;
};


struct node *head = NULL;


void add(int PID, char* name) {
   struct node *newProcess = (struct node*) malloc(sizeof(struct node));
   newProcess->PID = PID;
   newProcess->name = name;
   newProcess->next = head;	
   head = newProcess;
}


void printRunningProcesses(){
	struct node *temp = head;
	while (temp!=NULL){
		int a = kill(temp->PID, 0);
		if (a == 0 && errno != ESRCH ){
			printf("name: %s, PID: %i\n", temp->name, temp->PID);
		}
		temp = temp->next;
	}
}

void killAllProcesses(){
	struct node *temp = head;
	while (temp!=NULL){
		kill(temp->PID, SIGTERM);
		temp = temp->next;
	}
}

int runCommand(char* command, int wt, int ad){
	
	int rc = fork();
	if (rc < 0) {
		printf("fork failed\n");
		exit(1);
	} else if (rc == 0) {
		char name[strlen(command) + 1];
		strcpy(name, command);
		int i = 0;
		int redirection = 0;
		char **myargs = split(name, ' ', 0);
			
		while( myargs[i] != 0 )
		{
			if( strncmp(myargs[i], ">>", 2) == 0 ) 
			{
				redirection = 2;
				close(STDOUT_FILENO);
    			open(myargs[i+1], O_CREAT | O_WRONLY | O_APPEND , S_IRWXU);
				break;
			}
			else if( strncmp(myargs[i], ">", 1) == 0 )  {
				redirection = 1; 
				close(STDOUT_FILENO);
    			open("a.txt", O_CREAT | O_WRONLY, 0777);
				break;
			}
			else {
				redirection = 0;
			}
			i++;
		}

		int size = 0;
		while(myargs[size] != 0)
		{
			size++;
		}
		//if( strncmp(myargs[i], ">>", 2)
		if (redirection > 0) size--;
		char * myargs1[ size + 1];
		i = 0;
		int y = 0;
		for( i = 0; i < size; i ++)
		{
			if( strncmp(myargs[i], ">>", 2)  == 0 ||  strncmp(myargs[i], ">", 1)  == 0){
				continue;
			}
			myargs1[y] = strdup(myargs[i]);
			y++;
		}
		myargs1[y] = 0;
		free(myargs);
		if (execvp(myargs1[0], myargs1) == -1) {
            printf("Error executing command\n");
        }
		
	} else {
		if (ad != 0) add(rc, command);
		if (wt){
			int rcWait = wait(NULL);
			if (rcWait < 0){
				printf("Error while executing the wait command\n");
			}
		} 
		
	} return 0;
}






int callExit(){
	killAllProcesses();
	printf("%s\n", "Exiting program in 3....2.....1 \nExit Successful");
	return 0;
}

int main(int argc, char * argv[], char *envp[]) {
	char * path = getenv("PATH");
	strcat(path, ":.");
	setenv("PATH", path, 1);
	char *input = NULL; 
	size_t buffer = 120; 
	int ampercents = 0;
	int i = 0;
	printf("*********************************** \n %s \n***********************************\n", "Welcome to Habib University Shell");
	while (1) {
		input = NULL;
		printf("%s", "(<--HUsh-->)>>> ");
		getline(&input, &buffer, stdin);
		ampercents = 0;
		int wt = 1;
		for(i = 0; input[i] != '\0'; ++i)
		{
		   if('&' == input[i]) ampercents++;
		}

		char ** listOfCommands = split(input, '&', 0);
		int commands = 0;
				
		while(listOfCommands[commands] != 0)
		{
			commands++;
		}

		if (ampercents == 1 && commands == 1){
			wt = 0;
		}

		for(int i = 0; i < ampercents + 1; i++) {

			if (listOfCommands[i] != 0){
				char commandCopy[strlen(listOfCommands[i]) + 1];
				strcpy(commandCopy, listOfCommands[i]);
				char ** strs = split(commandCopy, ' ', 0);
				int sizeCommand = 0;
				
				while(strs[sizeCommand] != 0)
				{
					sizeCommand++;
				}

				if ((strstr(strs[0], "cd")) != NULL){
					
					if (sizeCommand < 2) {
						printf("%s\n", "Nothing to change, please specify a directory");
						continue;
					}

					if (chdir(strs[1]) < 0 ){
						perror("Error occured while changing directory.\n");
					}
					else{
						printf("directory change successful\n");
					}
				}

				else if ((strstr(strs[0], "pwd")) != NULL){
					char cwd[PATH_MAX];
					//getting current directory
					if (getcwd(cwd, sizeof(cwd))) {
						printf("Dear user, your current working directory is %s\n", cwd);
					}
					else {
						perror("Error occured while printing current directory.\n");
					}
				}

				else if ((strstr(strs[0], "mylsenv")) != NULL){
					int en = 0;
					while(envp[en] != 0)
					{
						printf("%s\n", envp[en]); en++;
					}
				}

				else if ( (strstr(strs[0], "myps")) != NULL ){
					printRunningProcesses();
				}

				else if ( (strstr(strs[0], "showVAR")) != NULL ){
					char commandCopy[strlen(listOfCommands[i]) + 1];
					strcpy(commandCopy, listOfCommands[i]);
					char ** tokens = split(commandCopy, ' ', 0);
					int numTokens = 0;
				
					while(tokens[numTokens] != 0)
					{
						numTokens++;
					}
					if (numTokens < 2) {
						printf("%s\n", "Can't show var, not enough arguments");
						continue;
					}
					else {
						char * environment = getenv(tokens[1]);
						if (environment) printf("%s \n", environment);
						else printf("The command was unsuccesful in fetching environment vars for %s ", tokens[1]);
					}
					free(tokens);
				}

				else if ((strstr(listOfCommands[i], "=")) != NULL ) {
					char commandCopy[strlen(listOfCommands[i]) + 1];
					strcpy(commandCopy, listOfCommands[i]);
					char ** tokens = split(commandCopy, '=', 0);
					int numTokens = 0;
					
					while(tokens[numTokens] != 0)
					{
						numTokens++;
					}
					if (numTokens == 2){
						int rc = setenv(tokens[0], tokens[1], 1);
						printf("%s%s\n", tokens[0], tokens[1]);
						if ( rc < 0)
						{
							printf("%s\n", "Error while setting environment variable");
							continue;
						}
					}
					free(tokens);
				}

				else if (strstr(listOfCommands[i], "exit") != NULL) {
					callExit();
					free(listOfCommands);
					free(input);
					return 0;
				}

				else{
					printf("%s\n", listOfCommands[i]);
					runCommand(listOfCommands[i], wt, 1);
				}

			free(strs);
			}
		}

		if (strstr(input, "exit") != NULL) {
			callExit();
			free(listOfCommands);
			free(input);
			return 0;
		}
		free(listOfCommands);
		free(input);
	}
	return 0;
}

/* 
refs:
OSTEP
string concat: https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-concatenation
getcwd, chdir: https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program/
PATH_MAX: https://www.systutorials.com/241453/maximum-allowed-file-path-length-for-c-programming-on-linux/
Kill(2): http://man7.org/linux/man-pages/man2/kill.2.html;
how to find if process is running: https://stackoverflow.com/questions/11785936/how-to-find-if-a-process-is-running-in-c
*/