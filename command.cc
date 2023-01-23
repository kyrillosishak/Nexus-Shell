
/*
 * CS354: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */
//#include <fildes.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <pwd.h>
#include <assert.h>
#include <time.h>

#define MAXFILENAME 1024

char **array = NULL;
int nEntries = 0;
int maxEntries = 20;
#include "command.h"

SimpleCommand::SimpleCommand()
{
	// Creat available space for 5 arguments
	_numberOfAvailableArguments = 5;
	_numberOfArguments = 0;
	_arguments = (char **)malloc(_numberOfAvailableArguments * sizeof(char *));
}

void SimpleCommand::insertArgument(char *argument)
{
	if (_numberOfAvailableArguments == _numberOfArguments + 1)
	{
		// Double the available space
		_numberOfAvailableArguments *= 2;
		_arguments = (char **)realloc(_arguments,
									  _numberOfAvailableArguments * sizeof(char *));
	}

	_arguments[_numberOfArguments] = argument;

	// Add NULL argument at the end
	_arguments[_numberOfArguments + 1] = NULL;

	_numberOfArguments++;
}

Command::Command()
{
	// Create available space for one simple command
	_numberOfAvailableSimpleCommands = 1;
	_simpleCommands = (SimpleCommand **)
		malloc(_numberOfSimpleCommands * sizeof(SimpleCommand *));

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
}

void Command::insertSimpleCommand(SimpleCommand *simpleCommand)
{
	if (_numberOfAvailableSimpleCommands == _numberOfSimpleCommands)
	{
		_numberOfAvailableSimpleCommands *= 2;
		_simpleCommands = (SimpleCommand **)realloc(_simpleCommands,
													_numberOfAvailableSimpleCommands * sizeof(SimpleCommand *));
	}

	_simpleCommands[_numberOfSimpleCommands] = simpleCommand;
	_numberOfSimpleCommands++;
}

void Command::clear()
{
	for (int i = 0; i < _numberOfSimpleCommands; i++)
	{
		for (int j = 0; j < _simpleCommands[i]->_numberOfArguments; j++)
		{
			free(_simpleCommands[i]->_arguments[j]);
		}

		free(_simpleCommands[i]->_arguments);
		free(_simpleCommands[i]);
	}

	if (_outFile)
	{
		free(_outFile);
	}

	if (_inputFile)
	{
		free(_inputFile);
	}

	if (_errFile)
	{
		free(_errFile);
	}

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
}

void Command::print()
{
	printf("\n\n");
	printf("              COMMAND TABLE                \n");
	printf("\n");
	printf("  #   Simple Commands\n");
	printf("  --- ----------------------------------------------------------\n");

	for (int i = 0; i < _numberOfSimpleCommands; i++)
	{
		printf("  %-3d ", i);
		for (int j = 0; j < _simpleCommands[i]->_numberOfArguments; j++)
		{
			printf("\"%s\" \t", _simpleCommands[i]->_arguments[j]);
		}
		printf("\n");
	}

	printf("\n\n");
	printf("  Output       Input        Error        Background\n");
	printf("  ------------ ------------ ------------ ------------\n");
	printf("  %-12s %-12s %-12s %-12s\n", _outFile ? _outFile : "default",
		   _inputFile ? _inputFile : "default", _errFile ? _errFile : "default",
		   _background ? "YES" : "NO");
	printf("\n\n");
}
void sigcatcher(int sig)
{

	FILE *f = fopen("log", "w");
	time_t mytime = time(NULL);
    char * time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
	int pid = waitpid(-1, NULL, WNOHANG);
	fprintf(f, " [%s] END CHILD.\n", time_str);
	fclose(f);
}
void sigcatcherc(int ret)
{
	FILE *f = fopen("log", "w");
	fprintf(f, "PID [%d] OPEN CHILD.\n", ret);
	fclose(f);
}
#define MAX_CLIENT_COUNT 1000
#include <poll.h>
struct process_table {
    pid_t clientpids[MAX_CLIENT_COUNT];
    struct pollfd clientfds[MAX_CLIENT_COUNT];
} PT;
void initialize_table() {
    for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
        PT.clientfds[i].fd = -1;
    }
}
int get_next_available_entry() {
    for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
        if (PT.clientfds[i].fd == -1) {
            return i;
        }
    }
    return -1;
}
void add_process_to_table(int i, pid_t pid, int fd) {
    PT.clientpids[i] = pid;
    PT.clientfds[i].fd = fd;
}

void remove_process_from_table(int i) {
    PT.clientfds[i].fd = -1;
}
void reap_zombie_processes() {
    int p = poll(PT.clientfds, MAX_CLIENT_COUNT, 0);

    if (p > 0) {
        for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
            /* Has the pipe closed? */
            if ((PT.clientfds[i].revents & POLLHUP) != 0) {
                 printf("[%d] done\n", PT.clientpids[i]);
                waitpid(PT.clientpids[i], NULL, 0);
                remove_process_from_table(i);
            }
        }
    }
}
void Command::execute(int silent)
{
	int ret;
	// Don't do anything if there are no simple commands
	if (_numberOfSimpleCommands == 0)
	{
		prompt();
		return;
	}

	if (silent == 0)
	{
		// Print contents of Command data structure
		print();
	}
	if(!strcmp( _simpleCommands[0]->_arguments[0], "cd" ))
	  {
	    int ret;
	    if(_simpleCommands[0]->_numberOfArguments == 1)
	      ret = chdir(getenv("HOME"));
	    else
	      ret = chdir(_simpleCommands[0]->_arguments[1]);
	    if(ret < 0)
	      perror("cd");
	    clear();
	    prompt();
	    return;
	  }
	// Add execution here
	// For every simple command fork a new process
	// Setup i/o redirection
	// and call exec
	int tmpin = dup(0);
	int tmpout = dup(1);
	int fdin;
	if (_inputFile)
		fdin = open(_inputFile, O_RDONLY,0666);
	else
		fdin = dup(tmpin);

	int fdout;
	for (int i = 0; i < _numberOfSimpleCommands; i++)
	{
		dup2(fdin, 0);
		close(fdin);
		int fdpipe[2];
		if (i == _numberOfSimpleCommands - 1)
		{
				if (_outFile){
					if(flag ==1)fdout = open(_outFile, O_WRONLY| O_APPEND| O_CREAT,0666);
					else fdout = open(_outFile, O_WRONLY | O_CREAT | O_TRUNC,0666);
				}else
					fdout = dup(tmpout);
		}
		else
		{
			
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}
		dup2(fdout, 1);
		ret = fork();
		if (ret != 0){
			
		       close(fdout);
           	      int i = get_next_available_entry();
            		add_process_to_table(i, ret, fdpipe[0]);
            		reap_zombie_processes();
		}if (ret == 0)
		{
			close(fdin);
			execvp(_simpleCommands[i]->_arguments[0], _simpleCommands[i]->_arguments);
			perror("execvp");
			_exit(1);
		}
	}
	dup2(tmpin, 0);
	dup2(tmpout, 1);
	close(tmpin);
	close(tmpout);
	if (!_background)
	{
		waitpid(ret, NULL, 0);
	}

	if (silent == 0)
	{
		// Clear to prepare for next command
		clear();
	}
	prompt();
	// Print new prompt
}

int compare(const void *str1, const void *str2)
{

	return strcmp(*(char *const *)str1, *(char *const *)str2);
}


void SimpleCommand::expandwildcards(char *arg){
	  
	char *reg = (char *)malloc(2*strlen(arg)+10);
	char *a = arg;
	char *r = reg;
	*r = '^'; r++;
	while(*a){
		if(*a == '*'){
			*r = '.';
			r++;
			*r = '*';
			r++;
		}else if (*a == '?'){
			*r = '.';
			r++;
		}else if (*a == '.'){
			*r = '\\';
			r++;
			*r = '.';
			r++;
		}else{
			*r = *a;
			r++;
		}
		a++;
	}
	*r='$'; r++; *r=0;
	regex_t re;	
	int result = regcomp( &re, reg,  REG_EXTENDED|REG_NOSUB);
	if (result!=0) {
		perror("regcomp");
		return;
	}
	DIR * dir = opendir(".");
	if (dir == NULL) {
		perror("oopendir");
		return;
	}
	struct dirent * ent;
	int ff= 0;
	while ( (ent = readdir(dir))!= NULL) {
		regmatch_t match;
		 if (regexec(&re , ent-> d_name,1, &match,0 ) ==0 ) {
			 Command::_currentSimpleCommand->insertArgument(strdup(ent-> d_name));
			 ff=1;
 		}
 	}
 	closedir(dir);
 	if(strcmp(arg, "*") && strcmp(arg, "?")){
 		if(ff==1) return;
		Command::_currentSimpleCommand->insertArgument(arg);
		return;
	}
}

// Shell implementation

void Command::prompt()
{
	// fflush(stdout);
	printf("\nSHELL>");
	fflush(stdout);
}
void INThandler(int sig)
{
	char c;

	signal(sig, SIG_IGN);
	printf("OUCH, did you hit Ctrl-C?\n"
		   "Do you really want to quit? [y/n] ");
	c = getchar();
	if (c == 'y' || c == 'Y')
		exit(0);
	else
		signal(SIGINT, INThandler);
	getchar(); // Get new line character
}

Command Command::_currentCommand;
SimpleCommand *Command::_currentSimpleCommand;

int yyparse(void);

int main()
{

	signal(SIGCHLD, sigcatcher);
	signal(SIGINT, INThandler);
	Command::_currentCommand.prompt();

	yyparse();
	return 0;
}
