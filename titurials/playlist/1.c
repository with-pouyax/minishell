

- Terminal Emulator: The terminal emulator is a program that emulates old physical terminals. It’s the window where you input commands.

- Bash: Bash is a language that runs inside most terminal emulators by default. It’s different from the terminal emulator itself.

- How to know what do I run on my terminal I mean if it is bash or zsh or something else?
- by typing echo $0 you can see what shell you are using.


- Confusing Terms: People often confuse terminal emulators and bash. For example, asking "How do I copy and paste in bash?" is incorrect because copying and pasting happens in the terminal emulator, not the language (bash).

Shells: Bash and Zsh are different shells (languages) that can run in the terminal emulator. Bash is the default for many systems, but other shells like Zsh or even Python can be used.

Command Line: The command line is the area where you type commands. It can run different languages (e.g., bash, Python), depending on what is set up.

Prompt: The prompt (the text before the command line where you type) is customizable and set within the shell’s configuration, not the terminal emulator.

_____________________________________________________________________

- when we open a terminal in linux, we are actually opening a shell environment too.
- the shell is the programm that let us to type commands and it will make the computer to do something with them.
- bash is one of the most popular shell in linux.
- when I open a terminal, all I can see is a prompt, like this:
- user@hostname:~$
- but there is actually a lot of things happening behind the scenes.
- lets look at enviorment variables:
there are two ways to see the enviorment variables:

1- set | less
2- printenv | less
- we pipe the output to less because there are a lot of variables and let command let us to scroll through them.
-firsone printout all enviourments variables in order, the second one print unsorted. 

- for example HOME variable is storing the path of the home directory of the user.
- PATH variable is storing the path of the directories that the shell will look for the commands that we type.

- there is somethinfg else called aliases. aliases are shortcuts for commands.

- for example if I set an alias like this: alias ll='ls -l'
- now if I type ll in the terminal, it will be the same as typing ls -l.
- we set aliases in the .bashrc file in the home directory.