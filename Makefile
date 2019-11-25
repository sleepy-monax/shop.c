SOURCES=$(wildcard source/*.c) $(wildcard source/**/*.c)
INCLUDES=$(wildcard source/*.h) $(wildcard source/**/*.h)

shop.out: $(SOURCES) $(INCLUDES)
	gcc -Isource -g -flto -Wall -Werror -Wextra -fsanitize=address -fsanitize=undefined $(SOURCES) -o shop.out

test: shop.out
	./shop.out

clean:
	rm ./shop.out

.PHONY: test
