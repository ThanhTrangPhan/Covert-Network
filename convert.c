// C program for writing to file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* readFile() {
	FILE *inFile;
	char *message;
	inFile = fopen("./sender.txt", "r");
	if(inFile == NULL){
		fprintf(stderr,"Error opening");
		exit(1);
	}

	while (fread(&message, sizeof(message), 1, inFile)){
		// printf("%c\n", message);
	}
	fclose(inFile);
	return message;
}

void writeFile(char* message) {
	FILE *outfile;
	outfile = fopen("./receiver.txt", "w");
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

void ASCIIToDecimal(char *txt){
	int *dec;
	memset(&dec, 0, 0*sizeof(int));
	printf("Size of txt %i\n", sizeof(txt));
	for(int i=0; i<sizeof(txt);++i){

	}
	
}
int main(){
	ASCIIToDecimal(readFile());

	
}