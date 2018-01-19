cflags = -std=c99 -O0 -g -Wall -Wextra -pedantic -Wshadow \
		-Werror=implicit-function-declaration -Werror=vla $(CFLAGS)
ldflags = $(LDFLAGS)

CC ?= cc

all: rdiff

rdiff: rdiff.c
	$(CC) $(cflags) -o $@ $^ $(ldflags)

clean:
	$(RM) rdiff
