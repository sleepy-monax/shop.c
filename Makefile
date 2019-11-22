SOURCES=$(wildcard *.c)
INCLUDES=$(wildcard *.h)

shop.out: $(SOURCES) $(INCLUDES)
	gcc -Wall -Werror -Wextra -fsanitize=address -fsanitize=undefined $(SOURCES) -o shop.out
