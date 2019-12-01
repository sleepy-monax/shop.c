PROJECT=shop
SOURCES = $(wildcard source/*.c) $(wildcard source/*/*.c)
OBJECTS = $(SOURCES:.c=.o)

CC = gcc
LDFLAGS = -lm
CFLAGS = -g \
		 -std=c11 \
		 -MD \
		 -Isource \
		 -Wall \
		 -Wextra \
		 -Werror \
		 -fsanitize=address \
		 -fsanitize=undefined 
 
$(PROJECT).out: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean test

all: $(PROJECT).out

clean:
	rm -f $(OBJECTS) $(SOURCES:.c=.d) $(PROJECT).out $(wildcard data/*.saved)

test: $(PROJECT).out
	./$(PROJECT).out

-include $(SOURCES:.c=.d)
