SOURCES=$(wildcard *.c)
INCLUDES=$(wildcard *.h)

shop.out: $(SOURCES) $(INCLUDES)
	gcc -g -flto -Wall -Werror -Wextra -fsanitize=address -fsanitize=undefined $(SOURCES) -o shop.out

test: shop.out
	./shop.out

clean:
	rm ./shop.out

.PHONY: test
