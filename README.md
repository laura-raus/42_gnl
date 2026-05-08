*This project has been created as part of the 42 curriculum by laraus.*

# Get Next Line

## Description

`get_next_line` is a C function that reads and returns one line at a time from a file descriptor.

The project focuses on one of the most important concepts in low-level programming:

* persistent state using static variables
* dynamic memory management
* incremental reading with `read()`
* string manipulation without the standard library
* handling partial data across multiple function calls

Unlike higher-level languages, C does not provide built-in line readers.
This project rebuilds that functionality manually while respecting strict memory and style constraints.

The function prototype is:

```c
char	*get_next_line(int fd);
```

Each call returns:

* the next line from the file descriptor
* including the trailing `\n` if it exists
* or `NULL` when EOF or an error is reached



# Project Goals

This project was designed to teach:

* how static variables persist between function calls
* how buffered reading works internally
* how to safely manage heap allocations
* how to avoid memory leaks
* how to split logic into maintainable helper functions
* how operating systems expose file reading through file descriptors

The real difficulty of the project is not the algorithm itself, but ownership and lifetime of memory.

Questions constantly asked during development included:

* Who owns this allocation?
* When should this pointer be freed?
* What happens if `malloc` fails?
* What happens if `read` returns `-1`?
* What happens when EOF is reached without `\n`?
* What if `BUFFER_SIZE` is `1`?
* What if the line is larger than the buffer?



# How It Works

The implementation follows this general flow:

```text
read()
↓
append to stash
↓
newline found?
↓
extract line
↓
preserve leftovers
↓
repeat
```

A static variable is used to preserve unread leftovers between calls.

Example:

```text
File:
Hello\nWorld\n
```

If `BUFFER_SIZE = 4`, reads may happen like:

```text
"Hell"
"o\nWo"
"rld\n"
```

After detecting `\n`, the function returns:

```text
"Hello\n"
```

while preserving:

```text
"Wo"
```

for the next function call.

This persistent storage mechanism is the core concept of the project.



# File Structure

```text
get_next_line/
├── get_next_line.c
├── get_next_line_utils.c
├── get_next_line.h
├── Makefile
└── README.md
```



# Function Overview

## Main Functions

### `get_next_line`

Public function responsible for:

* validating the file descriptor
* reading data
* returning one line at a time
* preserving remaining data between calls

### `cut_line`

Extracts the next line from the saved data and updates the remaining content.



## Utility Functions

### `ft_strlen`

Calculates string length.

### `ft_strchr`

Searches for a character inside a string.

Used mainly to detect `\n`.

### `ft_strdup`

Duplicates a string into newly allocated memory.

### `ft_strjoin`

Concatenates two strings while preserving dynamic memory correctness.



# Compilation

Compile manually:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
	get_next_line.c get_next_line_utils.c main.c
```

Or use the Makefile:

```bash
make
```



# Testing

Important edge cases tested:

* empty file
* invalid file descriptor
* file ending without `\n`
* multiple consecutive `\n`
* very small `BUFFER_SIZE`
* very large `BUFFER_SIZE`
* stdin reading
* large files
* long single-line files

Memory leak testing:

```bash
valgrind --leak-check=full ./a.out
```



# Challenges Encountered

The most difficult part of the project was not reading files, but correctly handling memory ownership and leftover data.

Several implementation approaches were explored:

* large multi-helper architectures
* compact minimal-function versions
* substring-based extraction
* direct partial duplication approaches

Tradeoffs between readability, modularity, Norminette compliance, and allocation complexity became an important part of the learning process.



# Technical Notes

## Allowed Functions

Only the following external functions were allowed:

* `read`
* `malloc`
* `free`

## Forbidden

* libft
* global variables
* `lseek`



# AI Usage

AI was used as a supplementary learning and review tool during development.

It was primarily used for:

* discussing architecture tradeoffs
* reviewing memory management logic
* understanding static variable behavior
* exploring different helper-function organizations
* generating edge cases and testing ideas
- clarification of memory behavior
- understanding edge cases
- README structure assistance

All final implementation decisions, debugging, testing, and adaptation to 42 Norm constraints were manually reviewed and adjusted.

# Lessons

This project reinforced several important low-level programming concepts:

* memory ownership matters
* persistent state is difficult to manage safely
* small helper functions improve debugging
* edge cases dominate systems programming
* reading incrementally is fundamentally different from processing full data at once

It also highlighted how much infrastructure higher-level languages hide from developers.

# Resources

* 42 subject PDF
* Linux `read(2)` manual
* `man 2 read`
* `man 3 malloc`
* GNU C documentation
* Valgrind documentation
* 42 peer discussions and evaluations


All code was reviewed, tested, and adapted manually.

⚠️ This project was completed as part of 42 School.  
It is provided for portfolio/review/learning purposes only. Please do not copy for coursework.
