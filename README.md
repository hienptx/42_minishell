
# SUM-UP MINISHELL PROJECT

How do we do it?

**STEP 1 >>> ----TOKENIZER----**

Input string before tokenizing: "sort   < Makefile |grep "$USER" | uniq>output.txt"

Follwoing functions breaks a command line input into an array of tokens, handling operators (<, >, |), whitespace, and quoted words. It uses:

Functions explaination: 
   - get_tokens:        Allocates and fills an array of tokens by iterating through the string.
   - count_tokens:      Counts the number of tokens, incrementing for each operator or word.
   - cpy_str:           Copies a token into the result array after processing spaces and operators.
   - get_word:          Skips over characters in a word until reaching a space, operator, or quote boundary.

These functions allow parsing of command-line input into tokens, handling operators and quoted strings.

Output string after tokenizing:

   [0] sort\0 
   [1] <\0
   [2] Makefile\0
   [3] |\0
   [4] grep\0
   [5] "$USER\0"
   [6] |\0
   [7] uniq\0
   [8] >\0
   [9] output.txt\0


**STEP 2 >>> ----SYNTAX_CHECK----**

Functions explaination: 
This step checks for syntax errors in a command-line input by validating the sequence of operators and tokens:
   - check_syntax: Validates the sequence of tokens, ensuring no invalid operator placements 
   (e.g., no operator as the first token, no consecutive operators, etc.).

If any syntax error is found, an error message is printed; otherwise, the syntax is valid.
   - invalid_syntax_sms: Prints an error message and returns 1.


**STEP 3 >>> ----EXPANSION_&_QUOTES_HANDLING----**

After Expansion and Quotes are handled, array of tokens look like:

   [0] sort\0 
   [1] <\0
   [2] Makefile\0
   [3] |\0
   [4] grep\0
   [5] hipham\0
   [6] |\0
   [7] uniq\0
   [8] >\0
   [9] output.txt\0


**STEP 4 >>> ----RECURSIVE_PARSE_TOKENS_&_CONSTRUCT_SYNTAX_TREE----**

This step provides a recursive parser that constructs a struct of Abstract Syntax Tree (AST) from a list of tokens. It handles commands, pipes, 
and redirections by creating various nodes in the AST.

The Syntax Tree looks like: 

                   PIPE (Level 1)
           /                        \
        PIPE (Level 2)               REDIR (>)
       /    \                          \
 EXEC(sort)  EXEC(grep hipham)         "output.txt"
   |
REDIR(<)
   |
"Makefile"

***Implementation Logic behind AST:***

Parser will look for the first PIPE(|) operator present in command line and set it to NULL. As next the left part of the Pipe will be saved as a string of char in t_pipe struct bellow. In a recursive manner, if parser detects the next PIPE(|) in the right part of the first PIPE(|), it will repeat the same process, set PIPE->NULL and parse the left and right to the struct untill there is no more PIPE(|) is found.



If there are no special operators present in input at all, parser will call exec_constructor to 
According to the tree, memory for Pipe Struct will be created first when operator (|) presents in command line.

Populate tokens to following struct of AST:
   ```c
   `typedef enum cmd_type
   {
      EXEC,
      REDIR,
      PIPE
   }					t_type;

   typedef struct exec
   {
      char			**arg;
   }					t_exec;

   typedef struct redir
   {
      t_exec			*cmd;
      char			*file_name;
      int				fd;
      struct redir	*next;
   }					t_redir;

   typedef struct pipe
   {
      void			*left;
      void			*right;
   }					t_pipe;

   typedef struct s_cmd
   {
      t_type			type;
      union
      {
         t_pipe		*pipe;
         t_redir		*redir;
         t_exec		*exec;
      } u_cmd;
   }					t_cmd;`
   ```

Functions explaination: 
   - parse_cmd:                           Recursively parses commands, creating PIPE nodes for each pipe (|), and calling parse_redir if no pipes are found.
   - process_redir and parse_redir:       Handle redirection tokens (<, >, >>, <<). process_redir creates redirection nodes, while parse_redir iterates through tokens, constructing redirection nodes if redirection operators are found.
   - parse_exec:                          Creates an EXEC command node in the AST, storing arguments for a basic command.
   - construct_pipe:                      Builds a PIPE node in the AST, linking the left and right command branches.
   - construct_redir:                     Creates a REDIR node, calling append_lst to manage redirections.
   - append_lst:                          Adds a new redirection to the list for REDIR nodes, storing the file descriptor and filename.
   - construct_exec:                      Populates an EXEC node’s argument list by copying tokens into a t_exec structure.


Overall, the functions work together to build an AST that represents commands, pipes, and redirections, providing a structured way to parse and execute command-line input.

Print the AST in Terminal: 
   Left pipe, level 0 
      Redirection file name: Makefile
      Fd: 0
         Exec Command: sort 
   Right pipe, level 0 
      Left pipe, level 1 
         Exec Command: grep hipham 
      Right pipe, level 1 
         Redirection file name: output.txt
         Fd: 1
         Exec Command: uniq

**STEP 5 >>> ----EXECUTION----**


STEP 6 >>> ----EXECUTE_BUILTINS----

