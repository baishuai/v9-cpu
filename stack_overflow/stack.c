/* stack.c */
/* This program has a buffer overflow vulnerability. */
/* Our task is to exploit this vulnerability */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int bof(char *str)
{
	char buffer[12];

	/* The following statement has a buffer overflow problem */
	strcpy(buffer, str);

	return 1;
}

int main(int argc, char **argv)
{
	char str[517];
	FILE *badfile;
	badfile = fopen("badfile", "r");
	fread(str, sizeof(char), 517, badfile);
	bof(str);
	printf("Returned Properly\n");
	return 1;
}

//(gdb) i r $esp
//esp            0xffffd320	0xffffd320


//(gdb) i r $esp
//esp            0xffffd300	0xffffd300

//0xffffd31c
