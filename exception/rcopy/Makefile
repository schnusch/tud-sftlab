cflags  = -O0 -g -Wall -Wextra -pedantic -Wshadow $(CFLAGS)
ldflags = $(LDFLAGS)

RM  ?= rm -f
CXX ?= c++

all: rcopy

rcopy: rcopy.cc Buffer.o Exception.o File.o
	$(CXX) $(cflags) -o $@ $^ $(ldflags)

%.o: %.cc *.h
	$(CXX) -c $(cflags) -o $@ $<
File.o: io.h

clean:
	$(RM) rcopy *.o
