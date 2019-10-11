#
# makefile
#

all:
	(cd mountain; make)
	gcc -m64 -std=c99 transpose.c -o transpose
	gcc -m64 -std=c99 convert.c -o convert
	gcc -m64 -std=c99 verify_6.37.c -o verify_6.37

test:

.PHONY : clean
clean:
	(cd mountain; make clean)
	find . -type f -executable -print0 | xargs -0 rm --

# vim:ft=make
#
