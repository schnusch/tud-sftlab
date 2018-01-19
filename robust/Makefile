cxxflags = -O0 -g -Wall -Wextra -pedantic -Wshadow -DFILETOOSMALL $(CXXFLAGS)
ldflags  = -lm -ldl $(LDFLAGS)

CXX ?= c++
RM  ?= rm -f

all: fputs_test
#	ldd /home/vmcheck/testhome/co/test2/result_enforcer.so

fputs_test: fputs_test.o stats.o
	$(CXX) -o $@ $^ $(ldflags)

%.o: %.cpp
	$(CXX) -c $(cxxflags) -o $@ $<

fputs_test.o: table.h tests.h verbose.h
tools.o: tools.h
stats.o: stats.h tests.h

clean:
	$(RM) fputs_test *.o vgcore.*
