# Makefile für myfind.c
all : myfind
myfind : myfind.c
	gcc -Wall -O -o myfind myfind.c
clean :
	rm -f myfind