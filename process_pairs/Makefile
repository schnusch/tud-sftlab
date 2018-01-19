cflags  = -O0 -g -Wall -Wextra -pedantic -Wshadow $(CFLAGS)
ldflags = $(LDFLAGS)

RM  ?= rm -f
CXX ?= c++

all: watch_dog

watch_dog: watch_dog.cc
	$(CXX) $(cflags) -o $@ $^ $(ldflags)

run: watch_dog
	./$< url.lst 2000 5000

clean:
	$(RM) watch_dog
