How does Shell execute a normal program (Linux, Shell, SH, and development)?

the shell scans the command line and breaks it up into tokens, making substitutions as it goes
the list of tokens is then built into parameter lists for a process. It then passes all or part of the list to a function to start the new process - I/O for stdin/stdout/stderr are setup as per the default (to the terminal)/redirection (a descriptor for the file)/pipe (the stdout pipe or stdin depending)
The shell then uses the fork system call to start another process running with the same environment the current shell has (including any token strings not used).

the NEW process then sets up stdin/stdout/stderr as directed by the tokens terminating the current list. It then uses the exec system call to replace the currently running memory image (the shell program) with the memory image of the targeted program (the first parameter in the command list).
The function repeats from step 3 until the list of tokens is completed, and then waits for the last process started to terminate (unless the list is ended with a “&” token to not wait.
Some shells (cshell for example) will set itself up to start the new processes with a lower priority if the token list is terminated by an “&”, indicating to put these in a background state.

If the user types in a control Z (an interrupt used to signal possible backgrounding), then the shell trap handler issues a “stop” signal to the last process, and returns to read another command - which is frequently “bg” to put the process in the background.

If the user tapes control C, (an interrpt with termination), the shell sends the interrupt signal to the last process. This gives the process a chance to handle it (doing cleanup/saving files) and exit. If the last process doesn’t handle it, then it is just terminated.

Now if you want details of this, you need to look into the specific shell you are interested in, as different shells will do things their own way. The above is just a general description of how a shell usually does this. Not all shells do all the steps shown above.

