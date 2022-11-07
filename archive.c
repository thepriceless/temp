#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct archive {
	FILE* data;
	char** names;
	int* sizes;
	int amount;
};

void dec_to_separated_dec(int number, unsigned char* space, int starting_byte) {
	space[starting_byte] = number % 256;
	number /= 256;
	space[starting_byte + 1] = number % 256;
	number /= 256;
	space[starting_byte + 2] = number % 256;
	number /= 256;
	space[starting_byte + 3] = number;
}

int separated_dec_to_dec(unsigned char* four_bytes) {
	int size = four_bytes[0] + four_bytes[1] * 256 + four_bytes[2] * 256 * 256 + four_bytes[3] * 256 * 256 * 256;
	return size;
}

void reader(FILE* input, struct archive* result, int extract) {
	int maximum_amount_of_files = 10;
	result->amount = 0;
	result->data = input;
	result->names = malloc(maximum_amount_of_files * sizeof(char*));
	result->sizes = malloc(maximum_amount_of_files * sizeof(int));
	fseek(input, 0L, SEEK_END);
	int full_size = ftell(input);
	fseek(input, 0, SEEK_SET);
	int temp, bytes_read = 0, cycle_num = 0;
	FILE* temp_file;
	unsigned char* memory;
	unsigned char* file_name = malloc(50 * sizeof(unsigned char));
	while (bytes_read < full_size) {
		if (result->amount == maximum_amount_of_files) {
			result->names = realloc(result->names, 2 * maximum_amount_of_files * sizeof(char*));
			result->sizes = realloc(result->sizes, 2 * maximum_amount_of_files * sizeof(int));
			maximum_amount_of_files *= 2;
		}
		memory = malloc(full_size * sizeof(unsigned char));
		file_name = malloc(50 * sizeof(unsigned char));
		result->amount++;
		fread(memory, 1, 1, input);
		temp = (int)memory[0];
		free(memory);
		fread(file_name, 1, temp, input);
		file_name[temp] = '\0';
		result->names[cycle_num] = file_name;
		memory = malloc(full_size * sizeof(unsigned char));
		fread(memory, 1, 4, input);
		result->sizes[cycle_num] = separated_dec_to_dec(memory);
		free(memory);
		if (extract) {
			memory = malloc(full_size * sizeof(unsigned char));
			fread(memory, 1, result->sizes[cycle_num], input);
			temp_file = fopen(result->names[cycle_num], "wb+");
			fwrite(memory, 1, result->sizes[cycle_num], temp_file);
			free(memory);
			fclose(temp_file);
		}
		else { fseek(input, result->sizes[cycle_num], SEEK_CUR); }
		bytes_read += 5 + temp + result->sizes[cycle_num];
		cycle_num++;
	}
}

FILE* create(char* name, struct archive* files) {
	FILE* final = fopen(name, "wb+");
	FILE* temp;
	int file_size;
	char* memory;
	unsigned char* size = malloc(4 * sizeof(unsigned char));
	for (int i = 0; i < files->amount; i++) {
		temp = fopen(files->names[i], "rb+");
		fseek(temp, 0L, SEEK_END);
		file_size = ftell(temp);
		files->sizes[i] = file_size;
		unsigned char* name_size = malloc(1 * sizeof(unsigned char));
		name_size[0] = strlen(files->names[i]);
		fwrite(name_size, 1, 1, final);
		fwrite(files->names[i], 1, strlen(files->names[i]), final);
		dec_to_separated_dec(file_size, size, 0);
		fwrite(size, 1, 4, final);
		memory = malloc(file_size * sizeof(char));
		fseek(temp, 0, SEEK_SET);
		fread(memory, 1, file_size, temp);
		fwrite(memory, 1, file_size, final);
		free(memory);
	}
	fclose(final);
}

int main(int argc, char* argv[]) {
	struct archive file;
	file.names = malloc((argc - 2) * sizeof(char*));
	file.sizes = malloc((argc - 2) * sizeof(int));
	file.data = malloc((argc - 2) * sizeof(FILE*));
	file.amount = 0;
	char* archive_name = malloc(40 * sizeof(char));
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--file") == 0) {
			archive_name = argv[i + 1];
		}
		else if (strcmp(argv[i], "--create") == 0) {
			for (int j = i + 1; j < argc; j++) {
				file.names[j - i - 1] = argv[j];
				file.amount++;
			}
			FILE* arc = create(archive_name, &file);
		}
		if (strcmp(argv[i], "--list") == 0) {
			FILE* input = fopen(archive_name, "rb+");
			struct archive info;
			reader(input, &info, 0);
			printf("List of files in the archive:\n");
			for (int j = 0; j < info.amount; j++) {
				printf("%d. %s\n", j + 1, info.names[j]);
			}
		}
		if (strcmp(argv[i], "--extract") == 0) {
			FILE* input = fopen(archive_name, "rb+");
			struct archive info;
			reader(input, &info, 1);
			printf("Files extracted successfully\n");
		}
	}
}
