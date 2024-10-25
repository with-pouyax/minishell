# Minishell

## Overview

This project aims to create a simple shell, called **Minishell**, that mimics some functionalities of the popular Bash shell. The shell should accept and execute basic shell commands, handle signals, and support built-in features such as piping, redirections, and environment variables, while following specific constraints.

## External Functions Used

Minishell relies on the following external functions:

- `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`
- `printf`, `malloc`, `free`, `write`
- `access`, `open`, `read`, `close`
- `fork`, `wait`, `waitpid`, `wait3`, `wait4`
- `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`
- `exit`, `getcwd`, `chdir`
- `stat`, `lstat`, `fstat`, `unlink`
- `execve`
- `dup`, `dup2`, `pipe`
- `opendir`, `readdir`, `closedir`
- `strerror`, `perror`
- `isatty`, `ttyname`, `ttyslot`, `ioctl`
- `getenv`
- `tcsetattr`, `tcgetattr`
- `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`

## Features

### Shell Basics
- **Interactive Prompt**: The shell displays a prompt while waiting for user input.
- **Command History**: The shell maintains a command history using `readline()`.
- **Executable Search**: It searches for executables in the directories specified in the `PATH` variable or through absolute/relative paths.

### Global Variable Constraint
- The shell avoids using more than one global variable to handle signals.
- Global variables cannot provide data access to any main structures, adhering to strict norms.

### Command Handling
- **Quotes**: 
  - Single quotes (`'`) disable metacharacter interpretation within the quoted sequence.
  - Double quotes (`"`) prevent metacharacter interpretation, except for `$` (dollar sign).
- **Redirections**:
  - `<` : Input redirection.
  - `>` : Output redirection.
  - `<<` : Heredoc (read input until a specified delimiter is encountered, but doesn’t update the history).
  - `>>` : Output redirection in append mode.
- **Pipes** (`|`): Implemented to chain commands, where the output of one command is the input to the next.

### Special Characters
- The shell handles environment variables (e.g., `$VAR`) and expands them to their values.
- `$?` expands to the exit status of the most recently executed command.

### Signal Handling
- The shell behaves like Bash with respect to signal handling:
  - `ctrl-C`: Interrupts the current command and displays a new prompt.
  - `ctrl-D`: Exits the shell.
  - `ctrl-\`: Does nothing in interactive mode.

### Built-in Commands
The following built-in commands are implemented:
- `echo` (with `-n` option)
- `cd` (supports relative or absolute paths)
- `pwd` (no options)
- `export` (no options)
- `unset` (no options)
- `env` (no options or arguments)
- `exit` (no options)

### Memory Management
- The shell must be free of memory leaks (except for those caused by the `readline()` function).

## Constraints
- Stick strictly to the project’s requirements. If in doubt, take **Bash** as a reference.


