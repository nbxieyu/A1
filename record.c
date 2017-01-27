#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "record.h"

void csv_to_record(char* filename, char* block_size){
	FILE *fp;
	char current_line[MAX_CHARS_PER_LINE];
	char *line;
	int records_per_block = atoi(block_size) / sizeof(Record);
	Record *buffer = (Record *) calloc (records_per_block, sizeof (Record));
	int buffer_pointer = 0;
	printf("%s\n", filename);
	fp = fopen(filename, "r");
	if(fp == NULL){
		perror("Error opening file");
		return;
	}
	
	int file = 0;
	int i = 0;
	//Parse the lines in csv file to an array of array chars
	while((line = fgets(current_line, MAX_CHARS_PER_LINE, fp)) != NULL){
		line [strcspn (line, "\r\n")] = '\0';
		printf("%s\n", line);
		char *attr = strtok(line, ",");
		//Record *record = malloc(sizeof(struct record));
		int j = 0;
		while(attr){

			printf("current attr %s\n", attr);
			if(j == 0){
				buffer[i].uid1 = atoi(attr);
			}else{
				buffer[i].uid2 = atoi(attr);
			}		
			attr = strtok(NULL, ",");
			j++;
		}
		i++;
		if(i == records_per_block - 1){
			write_buffer_to_disk(buffer, records_per_block, file, filename);
			file++;
			i = 0;

		}
		
		

	}

	if(file == 0){
		write_buffer_to_disk(buffer, i*sizeof(Record), file, filename);
	}
	free(buffer);
	fclose (fp);

}

// void write_record_to_buffer(Record* buffer, Record* record, int buffer_pointer){
// 	memcpy(buffer[buffer_pointer], &record, sizeof(record));
// 	free(record);

// }

void write_buffer_to_disk(Record* buffer, int total_records, int filenum, char* orginal_filename){
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
		fwrite(buffer, sizeof(Record), total_records, fp);
		fflush (fp);
	}else{
		return;
	}
	fclose(fp);

}

