
// C program for writing to file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

char* readFile() {
	File *inFile;
	char *message;
	infile = fopen("sender.txt", "r");
	if(inFile == NULL){
		fprintf(stderr,"Error opening");
		exit(1);
	}

	while fread(&message, sizeof(message), 1, inFile){
		printf("%s\n", message);
	}
	fclose(inFile);
	return message;
}

void writeFile(char* message) {
	File *outfile;
	char *message;
	outfile = fopen("receiver.txt", "w");
	if(outfile == NULL){
		fprintf(stderr,"Error opening");
		exit(1);
	}
	
	fwrite(&message, sizeof(message), 1, outfile);
	if(fwrite != 0)
        printf("contents to file written successfully !\n");
    else
        printf("error writing file !\n");
 
    // close file
    fclose (outfile);
}

int main(){
	File *o
	outfile = new File
}