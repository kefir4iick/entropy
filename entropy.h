#ifndef ENTROPY_H
#define ENTROPY_H

#include <wchar.h>

#define ALPHABET_SIZE 32
extern const wchar_t ALPHABET[];

int index_of(wchar_t c);
double entropy_1(const int *freq, int total);
double entropy_2(const int freq[ALPHABET_SIZE][ALPHABET_SIZE], int total_bigrams);

#endif
