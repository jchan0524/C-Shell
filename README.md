# Shell Program

This is a simple shell program implemented in C. It provides a basic command-line interface where users can execute various commands and navigate through directories.

## Files

The shell program consists of the following files:

1. `sh.c`: This file contains the main shell function `sh` that serves as the entry point of the program. It handles command parsing, executes built-in commands, and delegates execution to external programs. The file also includes functions for handling signals and wildcard characters.

2. `get_path.h` and `get_path.c`: These files define the `get_path` function, which retrieves the PATH environment variable and creates a linked list of directories. The `get_path` function is used by the shell program to locate executable files.

3. `main.c`: This file contains the main function, which sets up signal handlers, ignores certain signals, and calls the `sh` function to start the shell program.

## Usage

To compile and run the shell program, follow these steps:

1. Navigate to the directory of the program
2. Compile the source files using a C compiler: `make`
3. Run the compiled program: `./mysh`
4. 3. The shell program will start, and you can enter commands at the prompt (`>>`). Use common shell commands such as `cd`, `pwd`, `ls`, etc., to navigate and interact with the system.
