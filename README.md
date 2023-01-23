
# Nexus Shell
The Nexus Shell is a command-line interface (CLI) that allows users to interact with the operating system using basic commands such as navigating the file system, viewing and editing files, and running programs. The project is implemented using C language and utilizes system calls to interact with the operating system. It also includes basic error handling and input validation to ensure proper functionality. The project uses Lex and Yacc tools to handle the lexical and syntactical analysis of the command line input.
## Getting Started 
To get started with the Nexus Shell, please follow these steps:
- Git clone the project to any directory by using 
```
git clone https://github.com/kyrillosishak/Nexus-Shell.git
```
- Then just cd to that directory
```
cd Nexus-Shell
```
- Build the program by typing make
```
make
```
- Run it by
```
./shell
```
## Project Architecture
The Nexus Shell project includes the following files and folders:
- examples folder which contains some code snippets
- Commands.cc and command.h is where the most of the C code
- Makefile no need to do any changes there
- shell.l and shell.y are lex and yacc configuration files

## Running the shell
The Nexus shell program implements a simple grammar:

```
cmd [arg]* [> filename]
```
or some complex grammar like
```
cmd [arg]* [ | cmd [arg]* ]* [ [> filename] [< filename] [>> filename] ]* [&]
```
## Part 1: Simple commands, file redirection and pipes
- For every simple command, created a new process using fork() and call execvp() to execute the corresponding executable.
- Then the file redirection. If any of the input/output/error is different than 0 in the Command struct, then create the files, and use dup2() to redirect file descriptors 0, 1, or 2 to the new files.
- Then the pipes. Use the call pipe() to create pipes that will interconnect the output of one simple command to the input of the next simple command. use dup2() to do the redirection.

## Part 2: Process creation, execution, file redirection, pipes and background
- For every simple command, create a new process using fork() and call execvp() to execute the corresponding executable. 
- If the _background flag in the Command struct is not set then your shell has to wait for the last simple command to finish using waitpid().
- Now do the file redirection. 
- If any of the inputoutput/error is different than 0 in the Command struct, then create the files, and use dup2() to redirect file descriptors 0, 1, or 2 to the new files.
- Now do the pipes. Use the call pipe() to create pipes that will interconnect the output of one simple command to the input of the next simple command. use dup2() to do the redirection.
### simple Idea
```
1. while (true) {
2.    print "Nexus Shell> "
3.    input = get user input
4.    if (input == "exit") {
5.        exit the program
6.    }
7.    lex_and_yacc(input)
8.    if (parsing_error) {
9.        print error message
10.   } else {
11.       create new process using fork()
12.       if (child process) {
13.           execute command using execvp()
14.       } else {
15.           if (background process) {
16.               wait for process to complete using waitpid()
17.           }
18.       }
19.   }
20.}

```
This pseudocode describes a basic loop that repeatedly prompts the user for input, parses the input using Lex and Yacc, and then forks a new process to execute the command. If the input is "exit", the program exits. If there is a parsing error, an error message is printed. If the command is to be run in the background, the parent process waits for the child process to complete using waitpid().
### Additional Features
- Background processes
- Input and output redirection
- Piping between commands
### Known issues
- Some commands might not work as expected
- Error handling needs to be improved

### Acknowledgments
- The Nexus Shell project was inspired by the Unix shell
- The project was developed as a lab assignment in the operating systems course.
## Authors

- [@KyrillosIshak](https://github.com/kyrillosishak)


## License
This project is licensed under the MIT License - see the [LICENSE](https://choosealicense.com/licenses/mit/) file for details.

## Demo

Insert gif or link to demo


![Logo](https://github.com/kyrillosishak/Nexus-Shell/blob/main/L.png?raw=true)

