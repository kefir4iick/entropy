#include "entropy.h"
#include <math.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

const wchar_t ALPHABET[] = L" абвгдежзийклмнопрстуфхцчшщьыэюя";

int index_of(wchar_t c) {
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (ALPHABET[i] == c)
            return i;
    return -1;
}

double entropy_1(const int *freq, int total) {
    if (total == 0) return 0.0;
    double H = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (freq[i] > 0) {
            double p = (double)freq[i] / total;
            H -= p * log2(p);
        }
    }
    return H;
}

double entropy_2(const int freq[ALPHABET_SIZE][ALPHABET_SIZE], int total_bigrams) {
    if (total_bigrams == 0) return 0.0;
    double H = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        for (int j = 0; j < ALPHABET_SIZE; j++)
            if (freq[i][j] > 0) {
                double p = (double)freq[i][j] / total_bigrams;
                H -= p * log2(p);
            }
    return H / 2.0;
}

struct FreqPair {
    wchar_t sym;
    int count;
    double prob;
};

int cmp_freq(const void *a, const void *b) {
    return ((struct FreqPair *)b)->count - ((struct FreqPair *)a)->count;
}

static void save_monogram_section(FILE *f, const int freq[ALPHABET_SIZE], int total) {
    fwprintf(f, L"=======================================\n");
    fwprintf(f, L"            monogram\n");
    fwprintf(f, L"=======================================\n");
    fwprintf(f, L"symbol |  count  | prob\n");
    fwprintf(f, L"-------|---------|------------\n");
    struct FreqPair pairs[ALPHABET_SIZE];
    int non_zero = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (freq[i] > 0) {
            pairs[non_zero].sym = ALPHABET[i];
            pairs[non_zero].count = freq[i];
            pairs[non_zero].prob = (double)freq[i] / total;
            non_zero++;
        }
    }
    qsort(pairs, non_zero, sizeof(struct FreqPair), cmp_freq);
    for (int i = 0; i < non_zero; i++) {
        if (pairs[i].sym == L' ') {
            fwprintf(f, L" space | %7d | %.4f\n", pairs[i].count, pairs[i].prob);
        } else {
            fwprintf(f, L"   %lc   | %7d | %.4f\n", pairs[i].sym, pairs[i].count, pairs[i].prob);
        }
    }
    fwprintf(f, L"\n\n");
}

static void save_bigram_section(FILE *f, const int freq[ALPHABET_SIZE][ALPHABET_SIZE], const wchar_t *section_title) {
    fwprintf(f, L"=======================================\n");
    fwprintf(f, L"%ls\n", section_title);
    fwprintf(f, L"=======================================\n");
    
    fwprintf(f, L"     ");  
    for (int j = 0; j < ALPHABET_SIZE; j++) {
        if (ALPHABET[j] == L' ') {
            fwprintf(f, L"  s  ");  
        } else {
            fwprintf(f, L"  %lc  ", ALPHABET[j]);  
        }
    }
    fwprintf(f, L"\n");
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (ALPHABET[i] == L' ') {
            fwprintf(f, L"  s  ");  
        } else {
            fwprintf(f, L"  %lc  ", ALPHABET[i]);  
        }
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            fwprintf(f, L"%5d", freq[i][j]);  
        }
        fwprintf(f, L"\n");
    }
    fwprintf(f, L"\n\n");
}

static void save_bigram_prob_section(FILE *f, const int freq[ALPHABET_SIZE][ALPHABET_SIZE], int total_bigrams, const wchar_t *section_title) {
    fwprintf(f, L"=======================================\n");
    fwprintf(f, L"%ls\n", section_title);
    fwprintf(f, L"=======================================\n");
    
    fwprintf(f, L"        ");  
    for (int j = 0; j < ALPHABET_SIZE; j++) {
        if (ALPHABET[j] == L' ') {
            fwprintf(f, L"   s    ");  
        } else {
            fwprintf(f, L"   %lc    ", ALPHABET[j]);  
        }
    }
    fwprintf(f, L"\n");
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (ALPHABET[i] == L' ') {
            fwprintf(f, L"   s    ");  
        } else {
            fwprintf(f, L"   %lc    ", ALPHABET[i]); 
        }
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            double p = (total_bigrams > 0) ? (double)freq[i][j] / total_bigrams : 0.0;
            fwprintf(f, L"%.4f ", p);  
        }
        fwprintf(f, L"\n");
    }
    fwprintf(f, L"\n\n");
}

void save_all_tables(const char *filename, const int freq1[ALPHABET_SIZE], int total_monograms, const int freq2_over[ALPHABET_SIZE][ALPHABET_SIZE], int total2_over, const int freq2_nonover[ALPHABET_SIZE][ALPHABET_SIZE], int total2_nonover) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("error");
        return;
    }
    save_monogram_section(f, freq1, total_monograms);
    save_bigram_section(f, freq2_over, L"overlap bigrams (count)");
    save_bigram_prob_section(f, freq2_over, total2_over, L"overlap bigrams (prob)");
    save_bigram_section(f, freq2_nonover, L"disjoint bigrams (count)");
    save_bigram_prob_section(f, freq2_nonover, total2_nonover, L"disjoint bigrams (prob)");
    fclose(f);
}
