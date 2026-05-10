# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: laraus <laraus@student.42lisboa.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/10 14:05:43 by laraus            #+#    #+#              #
#    Updated: 2026/05/10 16:47:02 by laraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = gnl_test
BONUS_NAME = gnl_bonus_test

CC = cc
CFLAGS = -Wall -Wextra -Werror

BUFFER_SIZE ?= 42

INCLUDES = -Iincludes
BUFFER_FLAG = -D BUFFER_SIZE=$(BUFFER_SIZE)

SRC_DIR = srcs/
BONUS_DIR = srcs/bonus/
TEST_DIR = tests/

MANDATORY = \
	$(SRC_DIR)get_next_line.c \
	$(SRC_DIR)get_next_line_utils.c

BONUS = \
	$(BONUS_DIR)get_next_line_bonus.c \
	$(BONUS_DIR)get_next_line_utils_bonus.c

TEST_MANDATORY = $(TEST_DIR)main_gnl.c
TEST_BONUS = $(TEST_DIR)main_bonus.c

OBJS = $(MANDATORY:.c=.o)
BONUS_OBJS = $(BONUS:.c=.o)
TEST_OBJS = $(TEST_MANDATORY:.c=.o)
TEST_BONUS_OBJS = $(TEST_BONUS:.c=.o)

all: $(NAME)
	@echo "Build complete. Run 'make test' to execute tests."

%.o: %.c
	$(CC) $(CFLAGS) $(BUFFER_FLAG) $(INCLUDES) -c $< -o $@

generate_tests:
	BUFFER_SIZE=$(BUFFER_SIZE) ./tests/scripts/generate_tests.sh

$(NAME): $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(BUFFER_FLAG) $(INCLUDES) $(TEST_OBJS) $(OBJS) -o $(NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(TEST_BONUS_OBJS)
	$(CC) $(CFLAGS) $(BUFFER_FLAG) $(INCLUDES) $(TEST_BONUS_OBJS) $(BONUS_OBJS) -o $(BONUS_NAME)

test: generate_tests $(NAME)
	./$(NAME)

bonus: $(BONUS_NAME)
	@echo "Bonus build complete. Run 'make test_bonus' to execute tests."

test_bonus: generate_tests $(BONUS_NAME)
	./$(BONUS_NAME)

valgrind: generate_tests $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

valgrind_bonus: generate_tests $(BONUS_NAME)
	valgrind --leak-check=full --show-leak-kinds=all ./$(BONUS_NAME)

clean:
	rm -f $(OBJS) $(BONUS_OBJS) $(TEST_OBJS) $(TEST_BONUS_OBJS)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all generate_tests test bonus test_bonus clean fclean re valgrind valgrind_bonus