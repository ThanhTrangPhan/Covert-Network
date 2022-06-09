/* A dirty and undocumented hack to encode data as sequence number values so that
 * CCEAP can use them signaling a file over a covert channel with the MESSAGE (PDU)
 * ORDER pattern. Incremental sequence numbers are used.
 * License: GPLv3, see `LICENSE' file.
 *
 * This script can be used as a parameter for the CCEAP client.
 *
 * Use the following form:
 * $ ./client -s `./seq_encode [input file] [max_value] 2`
 * e.g.:
 * $ ./client -t `./seq_encode message.txt 256 2`
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "main.h"

#define PRINT_COMMA putchar(',');
#define CHK_BIT(x)                            \
                                              \
	if (x)                                    \
	{                                         \
		randomNumber = 'A' + (random() % 26); \
		_itoa(randomNumber, buf, 10);         \
		printf("%s", buf);                    \
	}                                         \
	else                                      \
	{                                         \
		randomNumber = 'a' + (random() % 26); \
		_itoa(randomNumber, buf, 10);         \
		printf("%s", buf);                    \
	}

int main(int argc, char *argv[])
{
	char buf[5];
	long randomNumber;
	FILE *fp;
	char byte[1];
	int maxval = 256;	 /* A header field has a max. value it can contain,
						  * in CCEAP it is 8 bits for the sequence no. field. */
	int num_symbols = 2; /* A covert channel needs at least 2 symbols. */
	int first = 1;
	extern char *__progname;

	if (argc <= 1)
	{
		fprintf(stderr, "usage: %s [input filename]\n", __progname);
		exit(1);
	}
	if ((fp = fopen(argv[1], "r")) == NULL)
	{
		perror("fopen");
		exit(1);
	}

	while (fread(byte, 1, 1, fp))
	{
		if (first)
			first = 0;
		else
			PRINT_COMMA /* comma behind every new byte */

			CHK_BIT(byte[0] & 0x01) PRINT_COMMA
			CHK_BIT(byte[0] & 0x02) PRINT_COMMA
			CHK_BIT(byte[0] & 0x04) PRINT_COMMA
			CHK_BIT(byte[0] & 0x08) PRINT_COMMA
			CHK_BIT(byte[0] & 0x10) PRINT_COMMA
			CHK_BIT(byte[0] & 0x20) PRINT_COMMA
			CHK_BIT(byte[0] & 0x40) PRINT_COMMA
			CHK_BIT(byte[0] & 0x80)
	}
	fclose(fp);

	return 0;
}
