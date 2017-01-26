#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "record.h"

void csv_to_record(char* filename, char* block_size){
	FILE *fp;
	char current_line[MAX_CHARS_PER_LINE];
	char *line;
	char *buffer = malloc(atoi(block_size));
	int buffer_pointer = 0;
	printf("%s\n", filename);
	fp = fopen(filename, "r");
	if(fp == NULL){
		perror("Error opening file");
		return;
	}
	int total_record = atoi(block_size) / sizeof(Record);
	int file = 0;
	int i = 0;
	//Parse the lines in csv file to an array of array chars
	while((line = fgets(current_line, MAX_CHARS_PER_LINE, fp)) != NULL){
		line [strcspn (line, "\r\n")] = '\0';
		printf("%s\n", line);
		char *attr = strtok(line, ",");
		Record *record = malloc(sizeof(struct record));
		int j = 0;
		while(attr){

			printf("current attr %s\n", attr);
			if(j == 0){
				record->uid1 = atoi(attr);
			}else{
				record->uid2 = atoi(attr);
			}		
			attr = strtok(NULL, ",");
			j++;
		}
		if(i == total_record - 1){
			write_buffer_to_disk(buffer, atoi(block_size), file, filename);
			file++;
			buffer_pointer = 0;

		}else{
			write_record_to_buffer(buffer, record, buffer_pointer);
			buffer_pointer++;
		}
		i++;
		

	}

	if(file == 0){
		write_buffer_to_disk(buffer, atoi(block_size), file, filename);
	}
	free(buffer);
	fclose (fp);

}

void write_record_to_buffer(char* buffer, Record* record, int buffer_pointer){
	memcpy(buffer + buffer_pointer*sizeof(Record), &record, sizeof(record));
	free(record);

}

void write_buffer_to_disk(char* buffer, int buffer_size, int filenum, char* orginal_filename){
	FILE *fp;
	char fileName[100];
	char fileNumber[sizeof(int) + 1];
	fileName[0] = '\0'; 
    fileNumber[0] = '\0';
  
    snprintf(fileNumber, sizeof(int), "%d", filenum);
  	strncpy(fileName, orginal_filename, strlen(orginal_filename) + 1);
    strncat(fileName, fileNumber, strlen(fileNumber));

	fp = fopen(fileName, "wb");
	if(fp){
		fwrite(buffer, buffer_size, 1, fp);
	}else{
		return;
	}
	fclose(fp);

}

int main(int argc, char *atgv[]){
	csv_to_record(atgv[1], atgv[2]);
}
