# Shell - *nash*

## Usage

```bash
make
./nash
```

## Assumptions

* Max character limit of hostname is 256 characters.
* Max character limit of every command is not more than 4096 characters.
* History file, `history.txt` will not be deleted during execution of the executable `./nash` 
* It is recommended to run the command `jobs` before the usage of `sig` to view the jobs and their corresponding job numbers.  

## Features

### ; separated commands

- Run multiple commands in a single line by separating them with a `;`  

### commands ending with &  

- Commands can be ran with the character `&` at the end of the command to run the process in the background.  

### echo

- `echo` prints the arguments as a string on console, replacing multiple tabs(`\t`) and spaces as a single space character, `' '` in the output string.   

### ls

- Lists the contents of the given directory, multiple directories and folders.  
- If arguments has files as well as folders, files are displayed first in sorted order followed by the contents of folders in sorted order of names.  
- Add `-l` to display the details of each file and folder,`-a` to display the hidden files and folders, `-la` or `-al` for both features.   
- The directories, files and flags can be mentioned in any order after `ls`.  

### cd

- Changes the current working directory to the relative or the absolute path specified in the argument.  
- `~` to go back to the shell executable directory.  
- If no argument is passed, it goes to the executable (home) directory.  
- `..` to go to the previous directory.   
- `-` will change your directory to the previous directory you were in. If no directories were changed before this command, it prints error message.   

### pwd

- Prints the present working directory `pwd`.  

### History

- Displays the last used commands with most recent ones appearing at the bottom of the list.
- stores uptill 20 history commands.

### Repeat   

- ```bash 
  repeat <number> [command]
  ```  
  This repeatedly executes the [command] for any number of times as specified. 
- `repeat` itself can be used a [command] in the argument for another `repeat`.  
  For example, 
  ```bash
  repeat 2 repeat 5 echo hello  
  ```
  This will print hello for 5 times which will then repeat the whole process for 2 times. That is, in total, the command `echo hello` will be called for 2 * 5 = 10 times.  
  

### Input and Output redirection

- Input and output can be redirected using:  
  -  `<` for input 
  -  `>` for output
  -  `>>` for appendding.

### Piping

- `|` redirects the output of the command on its left to the command on its right.   
- Multiple pipes can be used.  
- Also, piping with `<`, `>`, `>>` can be used.  

### Jobs

- Prints all the currently running and stopped processes in alphabetical order of the command name as a job number is assigned to every process. This job process does not change unless the process is finished.  
- `-s` flag is added to only view the stopped processes.   
- `-r` flag is added to only view the running processes.  

### Sig 

- ```bash
    sig <job number> <signal number>
    ```
    This sends a signal associated with the specified signal number to the given job number. 
- `jobs` should be run first to avoid sending signals to jobs which do not exist, and view all the processes and their corresponding job numbers.  

### fg

- ```bash
  fg <job number>
  ```
  This brings the stopped background job associated with the job number specified to the foreground. 

### bg

- ```bash
  bg <job number>
  ```
  This brings the stopped background job associated with the job number specified to run in the background.  

### Signal handling

- `CTRL + C` : This key interrupts any currently running foreground job by sending it the `SIGINT` signal.
- `CTRL + Z` : This puts any foreground to background and stops it by sending it the `SIGTSTP` signal. This process which is then stopped and put to background, can be brought to run in foreground again using `fg` or `sig`.  
- `CTRL + D` : This logs you out of the shell.  

### exit/quit

- `exit` or `quit` also logs you out of the shell.  
