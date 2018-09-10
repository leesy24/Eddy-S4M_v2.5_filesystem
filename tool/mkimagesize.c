#include "include/sb_include.h"
//=============================================================================
int main (int argc, char *argv[])
{
unsigned char chsz;
int fd, sz;
   
	fd = open (argv[1], O_RDWR);
	if (fd <= 0) 
		{
		printf ("%s File Not found\n", argv[1]);	
		return 0;
		}
	sz = lseek (fd, 0l, 2);
	chsz = (unsigned char)(sz / 20480) + 1;
//	printf ("File Size = %d,  Size / 20K = %u\n", sz, chsz);
	lseek (fd, 0x3f, 0);
	write (fd, &chsz, 1);
	close (fd);
}
