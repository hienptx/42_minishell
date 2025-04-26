# MINISHELL Project

## How to Use

### 1. Clone the Repository
```bash
git clone <repository_url>
````
### 2. Compile the project
```bash
make
````
### 3. Run the shell
```bash
./minishell
````
### 4. Execute commands
```bash
sort < Makefile | grep "pattern" | uniq > output.txt
````


## Overview

The **MINISHELL** project is a shell program built from scratch that simulates a Unix-like shell. The shell allows users to execute commands, handle pipes (`|`), redirections (`<`, `>`, `>>`), and support for command expansion and quoted strings. The project aims to recreate the functionality of a shell, implementing features like tokenization, syntax checking, parsing, and command execution.

---

## Key Features

- **Tokenization**: Parses user input into individual tokens.
- **Syntax Checking**: Verifies that the command structure is valid.
- **Expansion & Quotes Handling**: Handles environment variable expansion and quoted strings.
- **Abstract Syntax Tree (AST)**: Constructs an AST to represent the structure of commands and operators.
- **Command Execution**: Executes the commands with proper handling of pipes, redirections, and built-ins.
- **Built-in Commands**: Supports internal commands such as `cd`, `echo`, `exit`, etc.

---

## Steps of Implementation

### Step 1: Tokenizer

The tokenizer's role is to break down the input string into individual tokens. It handles operators (`<`, `>`, `|`), whitespace, and quoted strings. This process makes it easier to later analyze and parse the command structure.

**Functions involved:**

- **`get_tokens`**: Allocates and fills an array of tokens by iterating through the input string.
- **`count_tokens`**: Counts the number of tokens.
- **`cpy_str`**: Copies a token into the result array after processing spaces and operators.
- **`get_word`**: Skips over characters in a word until it encounters a space, operator, or quote.

**Example Input**:
```sh
sort < Makefile | grep "$USER" | uniq > output.txt
[0] sort
[1] <
[2] Makefile
[3] |
[4] grep
[5] "$USER"
[6] |
[7] uniq
[8] >
[9] output.txt
```
### Step 2: Syntax Checking

After tokenizing the input, we validate the syntax. This step ensures that the sequence of tokens is valid and adheres to shell grammar rules, such as proper placement of operators and no consecutive operators.

**Functions Involved**:
- **`check_syntax`**: Validates the sequence of tokens.
- **`invalid_syntax_sms`**: Prints an error message and returns `1` if a syntax error is found.

Example checks:
- No operator at the start.
- No consecutive operators.
- Operators are properly separated by arguments.

### Step 3: Expansion & Quotes Handling

This step handles expanding environment variables (e.g., `$USER`) and correctly parsing quoted strings (e.g., `"some text"` or `'some text'`). It ensures that environment variables are expanded before proceeding with token parsing and execution.

**Example Input (before expansion)**:
```bash
grep "$USER" Makefile
grep "NAME" Makefile
```

### Step 4: Recursive Parsing & Abstract Syntax Tree (AST)

Once the tokens are expanded, the next step is parsing them recursively and constructing an Abstract Syntax Tree (AST). The AST represents the structure of the input, breaking it down into commands, redirections, and pipes. 

- **Exec Command (EXEC)**: Represents a command with its arguments.
- **Redirection (REDIR)**: Handles input/output redirections (`<`, `>`, `>>`).
- **Pipe (PIPE)**: Represents pipes (`|`) between commands.

The recursive parsing ensures that the left and right parts of pipes are parsed correctly and that redirections are handled in the right order.

**AST Structure Example**:
            PIPE (Level 0)
      /                        \
PIPE (Level 1)              REDIR (>)
  /       \                        \
EXEC(sort)  EXEC(grep hipham)    "output.txt"
  |
REDIR(<)
  |
"Makefile"


**Functions Involved**:
- **`parse_cmd`**: Recursively parses the tokens to create command nodes.
- **`process_redir` & `parse_redir`**: Handle redirection operators (`<`, `>`, `>>`).
- **`construct_pipe`**: Constructs a PIPE node.
- **`construct_redir`**: Creates a REDIR node for handling redirection.
- **`construct_exec`**: Creates EXEC nodes for basic commands.

### Step 5: Command Execution

After the AST is constructed, the shell interprets and executes the commands. It handles the pipes and redirections as specified in the AST.

Execution involves:
- Running commands in sequence, as specified by the AST.
- Handling redirections by opening the correct file descriptors.
- Executing built-in commands when necessary.

### Step 6: Built-in Commands

The shell includes support for built-in commands. These commands do not require external programs and are handled directly by the shell. Common built-in commands include:
- **`cd`**: Changes the current directory.
- **`echo`**: Prints output to the terminal.
- **`exit`**: Exits the shell.

Each built-in command has a function that processes the command and performs the action.

