# Test Documentation

These tests are small local validation programs for the `get_next_line` project.

They are included for GitHub, learning, debugging, and portfolio purposes.
They are **not part of the official 42 subject requirements** and should not be
treated as mandatory project files for evaluation.

## Purpose

The tests validate `get_next_line` behavior across:

* multiple buffer sizes
* various line endings (with and without `\n`)
* edge cases (empty files, only newlines, long lines)
* boundary conditions (buffer-exact lines, buffer-1, buffer+1)
* invalid file descriptors
* memory leaks with Valgrind

The goal is not to replace a full tester, but to keep a small readable
test suite that can quickly confirm whether the function behaves as expected.

## Test File Generation

The `scripts/generate_tests.sh` script creates test files dynamically
based on the current `BUFFER_SIZE`.

### Generated Files

| File | Description |
|------|-------------|
| `empty.txt` | Empty file (0 bytes) |
| `one_char.txt` | Single character, no newline |
| `one_char_newline.txt` | Single character with newline |
| `no_newline.txt` | Text without trailing newline |
| `one_newline.txt` | Single line with newline |
| `multiple_lines.txt` | Three lines with newlines |
| `only_newlines.txt` | Four consecutive newline characters |
| `mixed.txt` | Mixed content with empty lines and final line without newline |
| `one_line.txt` | Single line for bonus FD limit stress test |
| `long_line.txt` | 10,000 characters followed by newline |
| `long_no_newline.txt` | 10,000 characters without newline |
| `buffer_exact.txt` | Exactly `BUFFER_SIZE` characters + newline |
| `buffer_minus_one.txt` | `BUFFER_SIZE - 1` characters + newline |
| `buffer_plus_one.txt` | `BUFFER_SIZE + 1` characters + newline |
| `two_buffer_exact_lines.txt` | Two lines, each exactly `BUFFER_SIZE` characters |
| `around_buffer_mixed.txt` | Lines of varying sizes around buffer boundary |

### Regenerating Test Files

```bash
make generate_tests
# or
BUFFER_SIZE=1024 ./tests/scripts/generate_tests.sh
```

## Test Mains

### `main_gnl.c`

Tests mandatory functionality:
- Invalid file descriptor handling
- All generated test files
- Compares actual line count against expected

### `main_bonus.c`

Tests bonus functionality:
- Multiple file descriptor interleaving
- Preserved read state per file descriptor
- Large FD limit (100 simultaneously open file descriptors)

## Bonus Tests

The bonus implementation is tested for:

- **Same FD sequential** - Baseline correctness (same as mandatory)
- **Interleaved FDs** - Reading from FD 3, then FD 4, then FD 5, then back to FD 3
- **Large FD limit** - 100 simultaneously open file descriptors
- **State preservation** - Each FD maintains its own read position independently

All bonus tests pass with Valgrind confirming no memory leaks.

## Output Format

Each test displays a summary with aligned columns:

```text
============================================================
GET_NEXT_LINE TESTS
============================================================
Invalid file descriptor (-1)                       | ✓ (returned NULL)
tests/files/empty.txt                              | 0 lines (expected 0) ✓
tests/files/one_char.txt                           | 1 lines (expected 1) ✓

============================================================
TOTAL TESTS: 18
PASSED:      18
FAILED:      0
============================================================
✓ ALL TESTS PASSED
```

## Running Tests and Valgrind

From the project root:

```bash
make test              # runs mandatory tests
make test_bonus        # runs bonus tests
make valgrind          # mandatory tests with memory leak detection
make valgrind_bonus    # bonus tests with memory leak detection
```

To test with a custom buffer size:

```bash
make test BUFFER_SIZE=1
make test_bonus BUFFER_SIZE=9999
```

Expected successful Valgrind output:

```txt
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

Valgrind helps detect:
- memory leaks
- invalid reads
- invalid writes
- use-after-free bugs

## Covered Test Cases

| Category | Cases |
|----------|-------|
| Edge | Empty file, only newlines, no newline at EOF |
| Boundary | Buffer exact, buffer-1, buffer+1 |
| Long lines | 10,000 chars with and without newline |
| Mixed content | Empty lines + regular lines |
| Error handling | Invalid file descriptor (-1) |
| Large buffer | `BUFFER_SIZE=9999`, `BUFFER_SIZE=1000000` |
| Small buffer | `BUFFER_SIZE=1` |
| Bonus | Interleaved FDs, 100 concurrent FDs |

## 42 Evaluation Note

The `get_next_line` implementation follows all functional requirements of the subject.
The professional directory structure (`srcs/`, `includes/`) is a portfolio enhancement and does not affect correctness.

The `tests/` folder is extra local tooling.
It is useful for GitHub and personal validation, but it should not be
considered part of the official submitted project.

## Adding New Tests

To add a new test file:

1. Add file creation logic to `scripts/generate_tests.sh`
2. Add corresponding test function in `tests/main_gnl.c` or `tests/main_bonus.c`
3. Update the expected line count

Example:

```c
check_file("tests/files/new_test_file.txt", expected_line_count);
```
