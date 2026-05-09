# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    generate_tests.sh                                  :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: laraus <laraus@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/09 15:54:31 by laraus            #+#    #+#              #
#    Updated: 2026/05/09 16:17:32 by laraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/usr/bin/env bash

mkdir -p tests/files

BS=${BUFFER_SIZE:-42}

: > tests/files/empty.txt

printf "a" > tests/files/one_char.txt
printf "a\n" > tests/files/one_char_newline.txt

printf "hello" > tests/files/no_newline.txt
printf "hello\n" > tests/files/one_newline.txt

printf "hello\nworld\n42\n" > tests/files/multiple_lines.txt
printf "\n\n\n\n" > tests/files/only_newlines.txt
printf "first line\n\nsecond line\nthird line without newline" > tests/files/mixed.txt

python3 - << EOF
from pathlib import Path

bs = int("$BS")
files = Path("tests/files")
files.mkdir(parents=True, exist_ok=True)

def write(name, text):
    (files / name).write_text(text)

write("long_line.txt", "a" * 10000 + "\\n")
write("long_no_newline.txt", "b" * 10000)

write("buffer_exact.txt", "E" * bs + "\\n")
write("buffer_minus_one.txt", "M" * max(bs - 1, 0) + "\\n")
write("buffer_plus_one.txt", "P" * (bs + 1) + "\\n")

write("two_buffer_exact_lines.txt",
      "A" * bs + "\\n" + "B" * bs + "\\n")

write("around_buffer_mixed.txt",
      "X" * max(bs - 1, 0) + "\\n" +
      "Y" * bs + "\\n" +
      "Z" * (bs + 1) + "\\n")
EOF

echo "Generated test files in tests/files/ with BUFFER_SIZE=$BS"
