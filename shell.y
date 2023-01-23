
/*
 * CS-413 Spring 98
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */

%token	<string_val> WORD

%token 	NOTOKEN GREAT NEWLINE GREATGREAT LESS PIPE AMPERSAND GREATGREATAMPERSAND EXIT

%union	{
		char   *string_val;
	}

%{
extern "C" 
{
	int yylex();
	void yyerror (char const *s);
	
}
#define yylex yylex
#include <stdio.h>
#include <signal.h>
#include <cstring>
#include "command.h"
%}

%%

goal:	
	commands
	;

commands: 
	command
	| commands command 
	;

command: simple_command 
        ;

simple_command:	
	commands_list iomodifier_opt_list NEWLINE{
		printf("   Yacc: Execute commands \n");
		Command::_currentCommand.execute(0);
	}
	| NEWLINE 
	| error NEWLINE { yyerrok; }

	;
iomodifier_opt_list :
	iomodifier_opt_list iomodifier_opt
	|
	;
commands_list :
	commands_list PIPE command_and_args
	|command_and_args
	;
	
command_and_args:
	command_word arg_list {
		Command::_currentCommand.
			insertSimpleCommand( Command::_currentSimpleCommand );
	}	
	;

arg_list:
	arg_list argument
	| /* can be empty */
	;

argument:
	WORD {
               printf("   Yacc: insert argument \"%s\"\n", $1);
				Command::_currentSimpleCommand->expandwildcards($1);
	}
	;

command_word:
	WORD {
               printf("   Yacc: insert command \"%s\"\n", $1);
	       if(!strcmp($1, "exit")){
			printf("\n    BYE     \n\n");
			return 0;
		}
	       Command::_currentSimpleCommand = new SimpleCommand();
	       Command::_currentSimpleCommand->insertArgument( $1 );
	}
	;

iomodifier_opt: 
	GREATGREAT WORD{
		printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand.flag =1;
		Command::_currentCommand._outFile = $2;
	}
	|
	GREAT WORD LESS WORD{
		printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = $2;	
		printf("   Yacc: insert input \"%s\"\n", $4);
		Command::_currentCommand._inputFile = $4;
	}
	|
	LESS WORD GREAT WORD{
		printf("   Yacc: insert output \"%s\"\n", $4);
		Command::_currentCommand._outFile = $4;	
		printf("   Yacc: insert input \"%s\"\n", $2);
		Command::_currentCommand._inputFile = $2;
	}
	| 
	GREAT WORD{
		printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand.flag =0;
		Command::_currentCommand._outFile = $2;	
	}
	|
	LESS WORD {
		printf("   Yacc: insert input \"%s\"\n", $2);
		Command::_currentCommand._inputFile = $2;
	}
	|
	AMPERSAND{
		printf("   Yacc: background \"\"\n");
		Command::_currentCommand._background = 1;
	}
	|
	GREATGREATAMPERSAND WORD{
		printf("   Yacc: insert output \"%s\"\n", $2);
		
		Command::_currentCommand._errFile = $2;
		printf("   Yacc: background \"\"\n");
		//Command::_currentCommand._background = 1;
	}
	;


%%

void
yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}


#if 0
main()
{

	yyparse();
}
#endif
