#include "utility.h"

// clean out a buffer with end of string chars
void clear_buf(uint8_t* buf, int buf_size) {
	int i;
	for (i = 0; i < buf_size; ++i) {

		buf[i] = '\0';
	}
}

// count the number of digits in a number
int count_digits(int n) {
	int digits = 0;
	while (n != 0) {
		n /= 10;
		++digits;
	}
	return digits;
}

// prints out the specified length of an array
void print_length(uint8_t* array, int length) {
	int i;
	for (i=0; i < length; ++i) {
		printf("%c", array[i]);
	}
	printf("\r\n");
}

// converts a string into a uint8_t array
void str_to_uint(char* str, uint8_t* arr, int length) {
	int i;
	for (i = 0; i < length; ++i) {
		arr[i] = (uint8_t)str[i];
	}
}
