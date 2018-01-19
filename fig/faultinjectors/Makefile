cflags = -std=c99 -O0 -g -Wall -Wextra -pedantic -Wshadow \
		-Werror=implicit-function-declaration -Werror=vla $(CFLAGS)
ldflags = -ldl $(LDFLAGS)

injectors = fi_read_EINTR.so fi_read_EIO.so fi_write_ENOSPC.so fi_write_EIO.so fi_select_ENOMEM.so fi_malloc_ENOMEM.so

CC ?= cc
RM ?= rm -f

all: $(injectors)

clean:
	$(RM) $(injectors)

%.so: %.c
	$(CC) -shared -fPIC $(cflags) -o $@ $^ $(ldflags)
