// functions for common tasks or debugging

#include "utility.h"

// count the number of digits in a number
int count_digits(int n) {
	int digits = 0;
	while (n != 0) {
		n /= 10;
		++digits;
	}
	return digits;
}

// prints out the specified number of characters of an array regardless of \0
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
