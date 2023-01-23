
#ifndef command_h
#define command_h

// Command Data Structure
struct SimpleCommand {
	// Available space for arguments currently preallocated
	int _numberOfAvailableArguments;

	// Number of arguments
	int _numberOfArguments;
	char ** _arguments;
	

	SimpleCommand();
	void insertArgument( char * argument );
	void expandwildcards(char *arg);
};

struct Command {
	int _numberOfAvailableSimpleCommands;
	int _numberOfSimpleCommands;
	SimpleCommand ** _simpleCommands;
	char * _outFile;
	char * _inputFile;
	char * _errFile;
	int _background;
	int flag = 0;

	void prompt();
	void print();
	void execute(int silent);
	void clear();
	//void exit(int n);
	
	Command();
	void insertSimpleCommand( SimpleCommand * simpleCommand );
	
	static Command _currentCommand;
	static SimpleCommand *_currentSimpleCommand;
};

#endif
