all: g3db_dump

g3db_dump:  g3db_dump.c
	gcc -O2 -o g3db_dump g3db_dump.c
