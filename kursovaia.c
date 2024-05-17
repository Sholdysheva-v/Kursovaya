#define _CRT_SECURE_NO_WAENINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDES 100

int main () {
	char line [MAX_LINE_LENGTH];
	char input_file [MAX_LINE_LENGTH];
	char output_file[MAX_LINE_LENGTH];

	printf ("Введите имя входного CSV-файла: ");
	fgets (input_file, sizeof(input_file), stdin);
	input_file[strcspn(input_file, "\n")] = '\0';

	FILE* file = fopen(input_file, "r");
	if (file == NULL) {
		perror ("ошибка открытия файла");
		return 1;
	}

	printf ("Введиет имя выходного JSON-файда");
	fgets (output_file, sizeof(output_file), stdin);
	output_file[strcspn(output_file, "\n")] = '\0';
	strcat (output_file, ".json");

	FILE* json_file = fopen (output_file, "w");
	if (json_file == NULL){
		perror ("Ошибка открытия файла для записи JSON");
		fclose(file);
		return 1;
	}

	char* keys [MAX_FIELDES];
	int field_count = 0;

	if(fgets(line, sizeof(line), file)!=NULL){
		char* token = strtok(line, ";");
		while (token!=NULL){
			char* newline_ptr = strchr(token, '\n');
			if(newline_ptr != NULL){
				*newline_ptr = '\0';
			}
			keys[field_count] = strdup(token);
			field_count++;
			token = strtok(NULL, ";");
		}
	}
	
	fprintf(json_file, "[\n");
	while (fgets(line, sizeof(line), file) != NULL){
		char* token = strtok (line, ";");
		fprintf (json_file, " {\n");
		for (int i = 0; i < field_count; i++){
			char* newline_ptr = strchr(token, '\n');
			if (newline_ptr!= NULL){
				*newline_ptr = '\0';
			}
			fprintf (json_file, "  \"%s\": \"%s\"", keys[i], token);
			token = strtok(NULL, ";");
			if (token != NULL) {
				fprintf (json_file, ",\n");
			}
			else {
				fprintf(json_file, "\n");
			}
		}
		fprintf (json_file, " }");

		if(!feof(file)) {
			fprintf(json_file, ",");
		}
		fprintf(json_file, "\n");
	}

	fprintf(json_file, "]\n");

	for (int i = 0; i < field_count; i++){
		free (keys[i]);
	}

	fclose(file);
	fclose(json_file);
	printf("Преобразование завершено. JSON сохранен в файле '%s'.\n", output_file);
	return 0;

}














