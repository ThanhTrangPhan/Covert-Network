/*
 * CCEAP: An Implementation of an Educational Protocol for Covert Channel
 *        Analysis Purposes
 *
 * The goal of this tool is to provide a simple, accessible implementation
 * for students. The tool demonstrates several network covert channel
 * vulnerabilities in a single communication protocol.
 *
 * Copyright (C) 2016-2019 Steffen Wendzel, steffen (at) wendzel (dot) de
 *                    https://www.wendzel.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Cf. `LICENSE' file.
 *
 */

#include "main.h"

#define OUTPUT_PARAMETER_Q "-q     Suppress welcome message; useful if CCEAP runs in a loop\n"

void
	*
	calloc_exit(int size, int elem)
{
	void *q;

	if (!(q = calloc(size, elem)))
	{
		perror("calloc");
		fprintf(stderr, "calloc_exit was called, parameters: (%i, %i)\n", size, elem);
		exit(ERR_EXIT);
	}
	return q;
}

void print_gpl(void)
{
	printf("CCEAP protocol implementation. Copyright (C) 2016-2019 Steffen Wendzel.\n"
		   "This program comes with ABSOLUTELY NO WARRANTY; for details see "
		   "LICENSE file.\n"
		   "This is free software, and you are welcome to redistribute it "
		   "under certain conditions;\nfor details see LICENSE file.\n\n"
		   "Hint: cite this tool/paper as:\n"
		   "S. Wendzel, W. Mazurczyk: An Educational Network Protocol for\n"
		   "Covert Channel Analysis Using Patterns, in Proc. ACM Conference on\n"
		   "Computer and Communications Security (CCS), pp. 1739-1741, 2016.\n\n"
		   "Hint: use ./server -q to prevent CCEAP from showing this message.\n\n");
}

void usage(int type)
{
	switch (type)
	{
	case USAGE_CLIENT:
		printf("The following parameters are supported by CCEAP client v." CCEAP_VER ":\n"
			   "\n"
			   "--------------------------------------------------------------\n"
			   "*** General parameters:\n"
			   "-D x   Destination IP x to connect to\n"
			   "-P x   TCP port x to connect to\n"
			   "-h     Provide an overview of supported parameters (this output)\n" OUTPUT_PARAMETER_Q
			   "-v     Activate verbose mode\n"
			   "-t x   Use the inter-arrival times in 'x' between packets (x should\n"
			   "       be given in the format 'Time_1,Time_2,...' (in ms)\n"
			   "-T x   Use inter-arrival time x[ms] for *all* packets (default is\n"
			   "       1000ms, i.e. 1sec)\n"
			   "\n"
			   "*** Parameters specific for the CCEAP protocol:\n"
			   "-c x   Number of packets to send (default: 10)\n"
			   "-i x   Initial sequence number x to use for CCEAP\n"
			   "-p x   Duplicate the packet with the sequence number x\n"
			   "-x x   Exclude the packet with the sequence number x\n"
			   "-s x   Use a pre defined sequence numbers given in x in the form:\n"
			   "       'Seq_1,Seq_2,Seq_3', e.g. '1,2,3', where 1 is the ISN\n"
			   "-d x   Use CCEAP destination address x\n"
			   "-o x   Optional header elements specified via x.\n"
			   "       Formatting: 'Option_1/Option_2/Option_3/...' where each\n"
			   "       option is formatted as 'Identifier,Type,Value'.\n"
			   "       Maximum of allowed options: %d\n"
			   "       Example for a string with 3 options: '1,2,3/4,5,6/7,8,9'\n"
			   "-u x   Use digit x instead of 0 as `dummy' value in the main header\n"
			   "--------------------------------------------------------------\n",
			   MAX_NUM_OPTIONS);
		break;
	case USAGE_SERVER:
		printf("The following parameters are supported by the CCEAP server v." CCEAP_VER ":\n"
			   "\n"
			   "--------------------------------------------------------------\n"
			   "*** General parameters:\n"
			   "-h     Provide an overview of supported parameters (this output)\n"
			   "-v     Activate verbose mode\n"
			   "-P x   TCP port x to listen to\n" OUTPUT_PARAMETER_Q
			   "--------------------------------------------------------------\n");
		break;
	default:
		fprintf(stderr, "Unsupported usage() parameter!\n");
		break;
	}
	exit(ERR_EXIT);
}

char *GetSubString(char *str, int index, int count)
{
	int strLen = strlen(str);
	int lastIndex = index + count;

	if (index >= 0 && lastIndex > strLen)
		return "";

	char *subStr = malloc(count + 1);

	for (int i = 0; i < count; i++)
	{
		subStr[i] = str[index + i];
	}

	subStr[count] = '\0';

	return subStr;
}

char *AppendString(const char *str1, const char *str2)
{
	int str1Len = strlen(str1);
	int str2Len = strlen(str2);
	int strLen = str1Len + str2Len + 1;
	char *str = malloc(strLen);

	for (int i = 0; i < str1Len; i++)
		str[i] = str1[i];

	for (int i = 0; i < str2Len; i++)
		str[(str1Len + i)] = str2[i];

	str[strLen - 1] = '\0';

	return str;
}

char *_itoa(int num, char *buffer, int base)
{
	int current = 0;
	if (num == 0)
	{
		buffer[current++] = '0';
		buffer[current] = '\0';
		return buffer;
	}
	int num_digits = 0;
	if (num < 0)
	{
		if (base == 10)
		{
			num_digits++;
			buffer[current] = '-';
			current++;
			num *= -1;
		}
		else
			return NULL;
	}
	num_digits += (int)floor(log(num) / log(base)) + 1;
	while (current < num_digits)
	{
		int base_val = (int)pow(base, num_digits - 1 - current);
		int num_val = num / base_val;
		char value = num_val + '0';
		buffer[current] = value;
		current++;
		num -= base_val * num_val;
	}
	buffer[current] = '\0';
	return buffer;
}

char *InsertString(char *str, int index, char *subStr)
{
	char *s = GetSubString(str, 0, index);
	s = AppendString(s, subStr);
	s = AppendString(s, GetSubString(str, index, strlen(str) - index));

	return s;
}

char *CharToString(char c)
{
	char *str = malloc(2);
	str[0] = c;
	str[1] = '\0';

	return str;
}

char *DecimalToASCII(char *dec)
{
	char *ascii = "";
	int decLen = strlen(dec);

	for (int i = 0; i < decLen; i += 3)
	{
		ascii = AppendString(ascii, CharToString((char)atoi(GetSubString(dec, i, 3))));
	}

	return ascii;
}

char *ASCIIToDecimal(char *str)
{
	char *dec = "";
	int strLen = strlen(str);

	for (int i = 0; i < strLen; ++i)
	{
		char *cDec = malloc(11);
		_itoa(str[i], cDec, 10);
		int cDecLen = strlen(cDec);

		if (cDecLen < 3)
			for (size_t j = 0; j < (3 - cDecLen); j++)
				cDec = InsertString(cDec, 0, "0");

		dec = AppendString(dec, cDec);
	}

	return dec;
}

