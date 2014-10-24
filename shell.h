#pragma once
#include <signal.h>
void shutdown();
void destroy_background();
void strip(char* input);
void parse(char* input);
int nargs(char* input);
void destroy_background(int signum, siginfo_t* sip, void* context);
void exec(char* name, char* args[], int background);
