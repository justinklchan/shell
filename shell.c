#include "process.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#define MAX_COMMAND_LEN 1000
#define MAX_PATH_LENGTH 1000
#define PATH ""

void shutdown()
{
	destroy();
	exit(0);
}

int main(int argc, char* argv[])
{
	struct sigaction sig_struct;
	sig_struct.sa_sigaction = destroy_background;
	sigfillset (&sig_struct.sa_mask);
  	sig_struct.sa_flags = SA_SIGINFO;
  	sigaction(SIGCHLD, &sig_struct, NULL);

  	signal(SIGINT, shutdown);

	while(1)
	{
		printf("jsh> ");

		//get input, strip of newline
		char input[MAX_COMMAND_LEN];
		memset(input, 0, MAX_COMMAND_LEN);
		fgets(input, MAX_COMMAND_LEN, stdin);
		
		//take care of eof
		if(feof(stdin))
		{
			shutdown();
			clearerr(stdin);
			exit(0);
		}
		strip(input);
		if(strlen(input) > 0)
		{
			parse(input);
		}
	}
	return EXIT_SUCCESS;
}

void strip(char* input)
{
	//trailing newline
	input[strlen(input)-1] = 0;
	
	//trailing spaces
	for(int i = strlen(input)-1; i >= 0; i--)
	{
		if(input[i] == ' ')
		{
			input[i] = 0;
		}
		else
		{
			break;
		}
	}
}

void parse(char* input)
{
	int background = 0;
	//check if background
	if(input[strlen(input)-1] == '&')
	{
		background = 1;
		input[strlen(input)-1] = 0;
	}

	//check for misplaced &
	for(int i = 0; i < strlen(input)-1; i++)
	{
		if(input[i] == '&')
		{
			printf("error\n");
			return;
		}
	}

	// printf("start parsing\n");
	char** args = calloc(nargs(input)+2, sizeof(char*));
	char* name = "";
	int argc = 0;
	char* token = strtok(input, " ");
	while(token != NULL)
	{
		if(strcmp(token, "quit") == 0)
		{
			exit(0);
		}
		else if(strcmp(token, "jobs") == 0)
		{
			list_processes();
		}
		else
		{
			if(argc == 0)
			{
				args[0] = calloc(strlen(PATH)+strlen(token), sizeof(char));
				sprintf(args[0], "%s%s", PATH, token);

				name = calloc(strlen(token), sizeof(char));
				strcpy(name, token);
			}
			else
			{
				args[argc] = calloc(strlen(token), sizeof(char));
				strcpy(args[argc], token);
			}
			argc += 1;
		}
		token = strtok(NULL, " ");
	}
	args[argc] = 0;
	// for(int i = 0; i < argc; i++)
	// {
	// 	printf("%s,%lu\n", args[i], strlen(args[i]));
	// }
	exec(name, args, background);
}

void exec(char* name, char* args[], int background)
{
	// printf("bg %d, %s\n", background, name);
	pid_t pid;
	if((pid = fork()) == 0)
	{
		execv(args[0], args);
		exit(0);
	}
	else
	{
		int status;
		if(background == 0)
		{
			waitpid(pid, &status, 0);
		}
		else
		{
			process* p = calloc(1, sizeof(process));
			p->name = calloc(strlen(name), sizeof(char));
			strcpy(p->name, name);
			p->pid = pid;
			int jid = add_process(p);
			printf("[%d] %d\n", jid, pid);
			free(p->name);
			free(p);
		}
	}
}

void destroy_background(int signum, siginfo_t* sip, void* context)
{
	int status;
	pid_t pid = waitpid(sip->si_pid, &status, WNOHANG);
	if(sip->si_pid == pid)
	{
		process* p = get_proc(sip->si_pid);
		if(p != NULL)
		{
			char *name = calloc(strlen(p->name), sizeof(char));
			strcpy(name, p->name);
			int jid = p->jid;
			remove_process(sip->si_pid);
			printf("[%d]\t+ %d done\t%s\n", jid, sip->si_pid, name);
		}
	}
}

int nargs(char* input)
{
	int args = 0;
	for(int i = 0; i < strlen(input); i++)
	{
		if(input[i] == ' ')
		{
			args += 1;
		}
	}
	return args;
}