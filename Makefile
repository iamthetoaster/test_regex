NAME = test_regex

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJECTS = $(subst .c,.o,$(SOURCES))

DEBUG_OBJECTS = $(addprefix dbg_,$(OBJECTS))

CC = clang

COMPILER_FLAGS = -Wall -Wextra -Wpedantic
LINKER_FLAGS =

DEBUG_FLAGS = -O0 -Og -g -D'DEBUG_MODE'
RELEASE_FLAGS = -O2 -Werror

.PHONY: clean format

all : $(NAME)

$(NAME) : $(OBJECTS)
	$(CC) $(LINKER_FLAGS) -o $@ $^

dbg_$(NAME) : $(DEBUG_OBJECTS)
	$(CC) $(LINKER_FLAGS) -o $@ $^

%.o : %.c
	$(CC) $(COMPILER_FLAGS) $(RELEASE_FLAGS) -o $@ -c $<

dbg_%.o : %.c
	$(CC) $(COMPILER_FLAGS) $(DEBUG_FLAGS) -o $@ -c $<

clean :
	rm -f $(NAME) *.o dbg_*

format :
	clang-format --style=file -i $(SOURCES) $(HEADERS)
