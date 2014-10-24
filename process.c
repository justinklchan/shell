#include "process.h"

process* head = NULL;
process* tail = NULL;
int count = 1;

int add_process(process* p)
{
	if(head == NULL && tail == NULL)
	{
		head = calloc(1, sizeof(process));
		head->name = calloc(strlen(p->name), sizeof(char));
		strcpy(head->name, p->name);
		head->pid = p->pid;
		head->jid = count;
		tail = head;
	}
	else
	{
		process* temp = calloc(1, sizeof(process));
		temp->name = calloc(strlen(p->name), sizeof(char));
		strcpy(p->name, temp->name);
		p->jid = count;
		temp->pid = p->pid;
		if(head == tail)
		{
			tail = temp;
			head->next = tail;
		}
		else
		{
			tail->next = temp;
			tail = temp;
		}
	}
	count += 1;
	return count-1;
}

int remove_process(pid_t pid)
{
	if(head == NULL)
	{
		return 1;
	}
	if(head == tail)
	{
		free(head->name);
		free(head);
		head = NULL;
		tail = NULL;
	}
	else if(head->pid == pid)
	{
		process* temp = head;
		head = head->next;
		free(temp->name);
		free(temp);
	}
	else
	{
		process* p = head;
		while(p->next != NULL && pid != p->next->pid)
		{
			p = p->next;
		}
		if(p == tail)
		{
			return 1;
		}
		process* next = p->next->next;
		free(p->next->name);
		free(p->next);
		p->next = next;
		if(p->next == NULL)
		{
			tail = p;
		}
		else if(p->next->next == NULL)
		{
			tail = p->next;
		}
	}
	count -= 1;
	return 0;
}

void destroy()
{
	printf("destroy\n");
	process* p = head;
	while(p != NULL)
	{
		process* temp = p->next;
		free(p->name);
		free(p);
		p = temp;
	}
}

process* get_proc(pid_t pid)
{
	process* p = head;
	while(p != NULL)
	{
		if(p->pid == pid)
		{
			return p;
		}
		p = p->next;
	}
	return NULL;
}

void list_processes()
{
	// printf("list\n");
	process* p = head;
	while(p != NULL)
	{
		printf("[%d]  + %d running\t%s\n", p->jid, p->pid, p->name);
		p = p->next;
	}
}
