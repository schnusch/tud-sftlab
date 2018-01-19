cflags = -std=c99 -O3 -march=native -mtune=native -g -Wall -Wextra -pedantic -Wshadow \
		-Werror=implicit-function-declaration -Werror=vla $(CFLAGS)
ldflags = -ldl $(LDFLAGS)

CC  ?= cc
CXX ?= c++
RM  ?= rm -f

all: fputs_wrapper.so

test: fputs_test
	./fputs_test

clean:
	$(MAKE) -C ../robust $@
	$(RM) fputs_wrapper.so filetail filetail.h fputs_test libfputswrap.so vgcore.*

fputs_wrapper.so: fputs_wrapper.c
	$(CC) -shared -fPIC $(cflags) -o $@ $< $(ldflags)

filetail: filetail.c
	$(CC) $(cflags) -o $@ $^
filetail.h: filetail
	./filetail > $@

init: init.c libinit.so
	$(CC) $(cflags) -o $@ $^ -Wl,-rpath=. -L. -linit
libinit.so: libinit.c
	$(CC) -shared -fPIC $(cflags) -o $@ $^

fputs_test: ../robust/fputs_test.o ../robust/stats.o ../robust/tools.o libfputswrap.so
	$(CXX) -o $@ $^ -Wl,-rpath=. -L. -lfputswrap
libfputswrap.so: fputs_wrapper.so
	ln -s $< $@
../robust/*.o:
	$(MAKE) -C ../robust $(patsubst ../robust/%,%,$@)
