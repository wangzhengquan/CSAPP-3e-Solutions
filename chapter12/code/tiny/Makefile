CC = gcc
CFLAGS = -g -Wall -I .

# This flag includes the Pthreads library on a Linux box.
# Others systems will probably require something different.
LDLIBS = -lpthread

PROGS = tiny\
				tiny2\
				tiny_process\
				tiny_select\
				tiny_pool\
				tiny_thread

all: $(PROGS) cgi

$(PROGS): csapp.o sbuf.o

cgi:
	(cd cgi-bin; make)

clean:
	rm -f *.o tiny *~
	(cd cgi-bin; make clean)
