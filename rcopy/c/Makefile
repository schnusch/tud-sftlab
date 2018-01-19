cflags = -std=c99 -O0 -g -Wall -Wextra -pedantic -Wshadow \
		-Werror=implicit-function-declaration -Werror=vla $(CFLAGS)
ldflags = $(LDFLAGS)

CC ?= cc

all: rcopy

rcopy: rcopy.c
	$(CC) $(cflags) -o $@ $^ $(ldflags)

clean:
	$(RM) rcopy
