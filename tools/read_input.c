
#include "read_input.h"

char* readline() {
	size_t alloc_length = 1024;
	size_t data_length = 0;
	char* data = (char*)malloc(alloc_length);

	while (1) {
		char* cursor = data + data_length;
		char* line = fgets(cursor, (int)(alloc_length - data_length), stdin);

		if (!line) { break; }

		data_length += strlen(cursor);

		if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') { break; }

		size_t new_length = alloc_length << 1;
		data = (char*)realloc(data, new_length);

		if (!data) { break; }

		alloc_length = new_length;
	}

	if (data[data_length - 1] == '\n') {
		data[data_length - 1] = '\0';
	}

	data = (char*)realloc(data, data_length);

	return data;
}

char** split_string(char* str) {
	char** splits = NULL;
	char* token = strtok(str, " ");

	int spaces = 0;

	while (token) {
		splits = (char**)realloc(splits, sizeof(char*) * ++spaces);
		if (!splits) {
			return splits;
		}

		splits[spaces - 1] = token;

		token = strtok(NULL, " ");
	}

	return splits;
}

int read_digit() {
	int err, digit;
	err = scanf("%d", &digit);
	if (err == 0) {
		digit = INT_MIN;
	}
	while (getchar() != '\n');
	return digit;
}

unsigned long long read_digit_ull() {
	int err;
	unsigned long long digit;
	err = scanf("%llu", &digit);
	if (err == 0) {
		digit = 0;
	}
	while (getchar() != '\n');
	return digit;
}

int count_split(char* str, int count) {
	for (int i = 0; i < strlen(str) + 1; ++i) {
		if (str[i] == ' ') { --count; }
	}
	return count == 0;
}
