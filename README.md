*This project has been created as part of the 42 curriculum by laraus.*

# Get Next Line

## Description

`get_next_line` is a C function that reads and returns one line at a time from a file descriptor.

The project focuses on low-level programming concepts including:

- persistent state using static variables
- dynamic memory management
- incremental reading with the `read()` system call
- string manipulation without the standard library
- handling partial data across multiple function calls

The function prototype is:

```c
char	*get_next_line(int fd);
```

Each call returns the next line from the file descriptor (including the trailing `\n` if present), or `NULL` when EOF or an error is reached.

## Algorithm Design and Justification

### Persistent Storage

The core challenge of `get_next_line` is preserving data between function calls. When `read()` fetches more bytes than needed for a single line, the excess must be retained for the next call.

The implementation uses a `static` buffer:

```c
static char buff[BUFFER_SIZE + 1];
```

This buffer retains its content across invocations without requiring heap allocation. The buffer size is fixed at compile time via `BUFFER_SIZE`, making dynamic allocation unnecessary.

### Static vs. Heap Allocation

| Approach | Compliant | Why |
|----------|-----------|-----|
| `static char buff[BUFFER_SIZE + 1]` | ✅ Correct | Persistent, fixed size, no allocation overhead. The buffer size is known at compile time. |
| `static char *buff = malloc(BUFFER_SIZE + 1)` | ❌ Fails project requirements | Unnecessary heap allocation. The pointer is static but the memory doesn't need to be on the heap. Adds failure points (malloc can return NULL) with zero benefit. |
| `static char *buff[BUFFER_SIZE + 1]` | ❌ Wrong | Array of uninitialized pointers, not a character buffer. Causes undefined behavior. |
| Heap allocation without static | ❌ Fails | Data lost between function calls. Impossible to return the next line correctly. |

**Why heap for the buffer fails the project scope:**

The subject requires persistent storage between calls. Heap allocation without `static` loses data. A `static` pointer to heap memory works technically but violates the spirit of the project because:

1. The buffer size `BUFFER_SIZE` is fixed at compile time - no dynamic sizing needed
2. `malloc` introduces unnecessary failure points
3. The subject expects students to understand `static` storage duration
4. Heap allocation adds complexity with no requirement or benefit

The heap is correctly used only for the returned `char *line`, whose length cannot be known until reading completes.

### Line Extraction Flow

1. Check if static buffer contains data from previous calls
2. If buffer is empty, call `read()` to fill it
3. Extract characters until newline or EOF
4. Join extracted characters into a heap-allocated line
5. Remove consumed portion from static buffer
6. Return line (caller responsible for freeing)

## Bonus: Multiple File Descriptors

The bonus requirement extends `get_next_line` to handle multiple file descriptors simultaneously without losing track of each read state.

### Implementation

The mandatory single static buffer becomes a 2D array:

```c
static char buff[FD_MAX][BUFFER_SIZE + 1];
```

Each file descriptor receives its own dedicated buffer, preserving independent read states without global variables.

## Repository Structure

```
get_next_line/
├── Makefile
├── README.md
├── .gitignore
├── docs/
│   └── en.subject.pdf
├── includes/
│   ├── get_next_line.h
│   └── get_next_line_bonus.h
├── srcs/
│   ├── get_next_line.c
│   ├── get_next_line_utils.c
│   └── bonus/
│       ├── get_next_line_bonus.c
│       └── get_next_line_utils_bonus.c
└── tests/
    ├── README.md
    ├── main_gnl.c
    ├── main_bonus.c
    ├── files/
    └── scripts/
```

### Source Organization

| Directory | Contents |
|-----------|----------|
| `includes/` | Header files with function prototypes and type definitions |
| `srcs/` | Mandatory source files |
| `srcs/bonus/` | Bonus source files (multi-file descriptor support) |
| `tests/` | Test suite, test files, and generation scripts |
| `docs/` | Project subject PDF |

## Instructions

### Compilation and Testing

```bash
make                # Builds mandatory executable
make test           # Builds (if needed) and runs mandatory tests
make bonus          # Builds bonus executable
make test_bonus     # Builds (if needed) and runs bonus tests
make valgrind       # Runs mandatory tests with memory leak detection
make valgrind_bonus # Runs bonus tests with Valgrind
make clean          # Removes object files
make fclean         # Removes object files and executables
make re             # Rebuilds from scratch
```

To specify a custom buffer size:

```bash
make test BUFFER_SIZE=1
make test_bonus BUFFER_SIZE=9999
```

### Makefile Targets

| Target | Description |
|--------|-------------|
| `make` or `make all` | Builds mandatory executable only |
| `make test` | Builds (if needed) and runs mandatory tests |
| `make bonus` | Builds bonus executable only |
| `make test_bonus` | Builds (if needed) and runs bonus tests |
| `make valgrind` | Runs mandatory tests with Valgrind |
| `make valgrind_bonus` | Runs bonus tests with Valgrind |
| `make clean` | Removes object files |
| `make fclean` | Removes object files and executables |
| `make re` | Rebuilds from scratch |

### Usage Example

```c
#include "get_next_line.h"
#include <fcntl.h>

int main(void)
{
    int fd = open("file.txt", O_RDONLY);
    char *line;

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

## Testing

Comprehensive tests are located in the `tests/` directory. See `tests/README.md` for detailed test documentation.

Test coverage includes:

- empty files
- single characters (with and without newline)
- multiple lines
- only newline characters
- lines exactly matching buffer boundaries (BUFFER_SIZE, BUFFER_SIZE±1)
- long lines (10,000+ characters)
- invalid file descriptors
- standard input reading
- multiple interleaved file descriptors (bonus)
- large FD limit stress test (100 FDs, bonus)

## Resources

### Documentation

- Linux man pages: [https://man7.org/linux/man-pages/](https://man7.org/linux/man-pages/)
- `read(2)` manual
- `malloc(3)` / `free(3)` manual
- GNU Make documentation: [https://www.gnu.org/software/make/manual/](https://www.gnu.org/software/make/manual/)
- C language and libc references: [https://en.cppreference.com/w/c](https://en.cppreference.com/w/c)

### References

- 42 project subject (included in `docs/`)
- C standard library documentation

## AI Usage

AI tools were used for:

- conceptual clarification of static variable behavior
- debugging assistance for edge cases
- memory management analysis
- boundary condition review (BUFFER_SIZE extremes)
- test case generation
- documentation structure refinement

All implementation, integration, testing, and validation were completed manually.

## Notes

Additional testing details are available in `tests/README.md`.

This repository was developed as part of the 42 curriculum and is maintained for educational and portfolio purposes.
