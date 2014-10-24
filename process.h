#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node
{
	struct node* next;
	char* name;
	pid_t pid;
	int jid;
} process;

int add_process(process* p);
int remove_process(pid_t pid);
void list_processes();
void destroy();
process* get_proc(pid_t pid);
