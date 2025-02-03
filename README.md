# Minishell

![Terminal Icon](https://i.pinimg.com/736x/01/e5/f5/01e5f554c4a1f2fb13a039152a2e5b0b.jpg)

 
A minimal UNIX shell built as part of the **42 School** curriculum. Minishell supports command execution, redirections, pipes, and more, while adhering to strict coding and function usage rules.

---

## Table of Contents
1. [Introduction](#introduction)  
2. [Features](#features)  
3. [Example Usage](#example-usage)  
4. [42 School Rules & Restrictions](#42-school-rules--restrictions)   
   - [Allowed Functions](#allowed-functions)  
   - [Norminette Rules](#norminette-rules)  
5. [Contributors](#contributors)  
6. [Lexing (Tokenization)](#lexing-tokenization)  
7. [Parsing](#parsing)  
8. [Signal Handling](#signal-handling)  
9. [Environment Variable Expansion & Quotes](#environment-variable-expansion--quotes)  
10. [Readline & Memory Leak Prevention](#readline--memory-leak-prevention)  
11. [Interactive & Non-Interactive Mode](#interactive--non-interactive-mode)  
12. [Built-in Commands](#built-in-commands)  
    - [cd](#cd)  
    - [pwd](#pwd)  
    - [export](#export)  
    - [env](#env)  
    - [exit](#exit)  
    - [echo](#echo)  
    - [unset](#unset)  
13. [External Command Execution](#external-command-execution)  
14. [Pipes (`|`)](#pipes-)  
15. [Input Redirection (`<`)](#input-redirection-)  
16. [Output Redirection (`>` and `>>`)](#output-redirection--and-)  
17. [Heredoc (`<<`)](#heredoc-)  
18. [Memory Management & Leak Prevention](#memory-management--leak-prevention)  
19. [Process Management](#process-management)  
20. [How to Run & Compile](#how-to-run--compile)  

---

## Introduction
**Minishell** is a “mini” version of the Unix shell. It replicates fundamental features of `bash` (or other POSIX shells) such as command parsing, built-in commands, redirections, and job management, but does so under strict constraints regarding code style, permitted functions, and memory handling.

---

## Features
- **Interactive Prompt** – Custom shell prompt in interactive mode.  
- **Command Execution** – Executes commands via absolute or relative paths, and searches in `$PATH`.  
- **Built-in Commands** – Includes `cd`, `pwd`, `echo`, `env`, `export`, `unset`, and `exit`.  
- **Environment Variable Expansion** – Expands `$VAR` and `$?`.  
- **Redirections** – Supports `<`, `>`, `>>`, and heredoc `<<`.  
- **Pipelines** – Executes commands in parallel with the `|` operator.  
- **Signal Handling** – Manages signals for `Ctrl-C`, `Ctrl-D`, and `Ctrl-\`.  
- **Memory Management** – Prevents memory leaks and manages resources carefully.  

---

## Example Usage
Below are a few simple examples of commands you can run in Minishell:

```sh
minishell> echo "Hello, world!"
Hello, world!

minishell> ls -l | grep minishell
-rwxr-xr-x  1 user  staff  42000 Feb 1 12:00 minishell

minishell> echo "42 Vienna" > file.txt
minishell> cat file.txt
42 Vienna

minishell> echo "Enter text" | cat -e
Enter text$
```

---

## 42 School Rules & Restrictions

### Allowed Functions
Minishell is restricted to using only the following system functions:

```
readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay,
add_history, printf, malloc, free, write, access, open, read, close,
fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset,
sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink,
execve, dup, dup2, pipe, opendir, readdir, closedir, strerror,
perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr,
tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
```
Any other required functionality must be implemented manually.

### Norminette Rules
- **Function length**: max 25 lines.  
- **Single responsibility**: each function should do one thing.  
- **Max 5 function arguments**.  
- **No global variables** (except for handling signals).  
- **Header guards** in header files.  
- **Clean, readable code structure**.  
- **No memory leaks** – always free allocated memory.  

---

## Contributors
Minishell is a collaborative project developed as part of the **42 School** curriculum.

### Project Authors
- **Pouya** ([GitHub Profile](https://github.com/your-github))  
- **Mina** ([GitHub Profile](https://github.com/mina-houshmand))  

### Collaboration
This project was developed via **GitHub** collaboration.  
All contributions, including coding, debugging, and documentation, are tracked via GitHub commits and pull requests.


Special thanks to **42 Vienna** and the **42 community** for their support and feedback throughout development.

---

## Lexing (Tokenization)
**Lexing** (or lexical analysis) splits the user’s command line input into **tokens**. Each token might represent a command, argument, operator (`|`, `<`, `>`, etc.), or special symbol.

1. **Input** → `echo "Hello, 42 Vienna" > output.txt`  
2. **Lexer** → Produces a list of tokens:  
   ```
   [COMMAND: echo] [STRING: "Hello, 42 Vienna"] [REDIRECTION: >] [FILENAME: output.txt]
   ```
3. **Token List** → Passed to the parser for further processing.

### Common Token Types
- **Commands**: `ls`, `echo`, `grep`, …  
- **Arguments**: Flags or text after commands (e.g., `-l`, `file.txt`).  
- **Operators**: `|`, `<`, `>`, `>>`, `<<`.  
- **Quotes**: Single (`'`) for literal text, Double (`"`) for partial expansion.  
- **Environment Variables**: `$HOME`, `$?`, etc.  

---
