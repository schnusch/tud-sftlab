cflags   = -std=c99 -O0 -g -Wall -Wextra -pedantic -Wshadow $(CFLAGS)
cxxflags = -O0 -g -Wall -Wextra -pedantic -Wshadow $(CXXFLAGS)
ldflags  = -lm -ldl $(LDFLAGS)

CC  ?= cc
CXX ?= c++
RM  ?= rm -f

all: fputs_test

fputs_test: fputs_test.o stats.o #tools.o
	$(CXX) -o $@ $^ $(ldflags)

filetoosmall: filetoosmall.c
	$(CC) $(cflags) -o $@ $^

%.o: %.cpp
	$(CXX) -c $(cxxflags) -o $@ $<

fputs_test.o: table.h tests.h verbose.h
tools.o: tools.h
stats.o: stats.h tests.h

clean:
	$(RM) filetoosmall fputs_test *.o vgcore.*
