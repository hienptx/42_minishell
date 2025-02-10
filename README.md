
# SUM-UP MINISHELL PROJECT

How do we do it?

**STEP 1 >>> ----TOKENIZER----**

Input string before tokenizing: "sort   < Makefile |grep "$USER" | uniq>output.txt"

Following functions break input command line into an array of tokens, handling operators (<, >, |), whitespace, and quoted words. It uses:
 
   - get_tokens:        Allocates and fills an array of tokens by iterating through the string.
   - count_tokens:      Counts the number of tokens, incrementing for each operator or word.
   - cpy_str:           Copies a token into the result array after processing spaces and operators.
   - get_word:          Skips over characters in a word until reaching a space, operator, or quote boundary.

These functions allow parsing of command-line input into tokens, handling operators and quoted strings.

Output string after tokenizing:

$$
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
$$


**STEP 2 >>> ----SYNTAX_CHECK----**

Functions explaination: 
This step checks for syntax errors in a command-line input by validating the sequence of operators and tokens:
   - check_syntax: Validates the sequence of tokens, ensuring no invalid operator placements 
   (e.g., no operator as the first token, no consecutive operators, etc.).

If any syntax error is found, an error message is printed; otherwise, the syntax is valid.
   - invalid_syntax_sms: Prints an error message and returns 1.


**STEP 3 >>> ----EXPANSION_&_QUOTES_HANDLING----**

After Expansion and Quotes are handled, array of tokens look like:

$$
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
$$


**STEP 4 >>> ----RECURSIVE_PARSE_TOKENS_&_CONSTRUCT_SYNTAX_TREE----**

This step provides a recursive parser that constructs a struct of Abstract Syntax Tree (AST) from a list of tokens. It handles commands, pipes, 
and redirections by creating various nodes in the AST.

The Syntax Tree looks like: 

                      PIPE (Level 0)
              /                           \
        PIPE (Level 1)                  REDIR (>)
       /             \                          \
       EXEC(sort)  EXEC(grep hipham)         "output.txt"
       |
       REDIR(<)
       |
       "Makefile"

***Implementation Logic behind AST:***

1. Detecting Operators: 
As the parser iterates through the token array, it looks for specific operators, such as pipes (|). When a pipe operator is encountered, it is set to NULL, dividing the command into a left part (before the pipe) and a right part (after the pipe).

2. Recursive Parsing:
The parser makes recursive calls to process both the left and right parts of the command. Each call handles its portion of the tokens independently, allowing for nested structures.
During the parsing of the left part, if redirection operators (like <, >, or >>) are detected, the parser immediately processes them. This means that redirection handling is integrated into the parsing flow.

3. Redirection Handling:
As each left part of the pipe is parsed, the parser checks for redirection operators. When a redirection operator is found, the parser adjusts the command structure accordingly, incorporating the file descriptors and target filenames into the command.
This is done through a separate function dedicated to processing redirections, which may involve adjusting the command's attributes (e.g., linking to a redirection structure or appending to a list of redirections).

4. Base Case:
The base case for the recursion occurs when there are no more pipes to process. At this point, any remaining tokens in the left part are treated as standalone commands or arguments.
If there are still redirection operators present, they are handled in the same manner as described above.

5. Constructing the AST:
Once the left part has been fully processed, including any redirections, the parser constructs a node in the abstract syntax tree (AST) representing the left command.
The process repeats for the right part, and if both sides of the pipe are successfully parsed, the parser creates a pipe node that links the two sub-command nodes.

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


**STEP 6 >>> ----EXECUTE_BUILTINS----**

