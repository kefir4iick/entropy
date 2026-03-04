#include <stdio.h>
#include <locale.h>
#include "entropy.h"

#define MAX_TEXT 2000000

int read_text(const char *filename, wchar_t *text, int remove_spaces);
void save_all_tables(const char *filename, const int freq1[ALPHABET_SIZE], int total_monograms, const int freq2_over[ALPHABET_SIZE][ALPHABET_SIZE], int total2_over, const int freq2_nonover[ALPHABET_SIZE][ALPHABET_SIZE], int total2_nonover);

void process(const char *title, const wchar_t *text, int n, const char *output_file) {
    int freq1[ALPHABET_SIZE] = {0};
    int freq2_over[ALPHABET_SIZE][ALPHABET_SIZE] = {0};
    int freq2_nonover[ALPHABET_SIZE][ALPHABET_SIZE] = {0};
    int total2_over = 0;
    int total2_nonover = 0;

    for (int i = 0; i < n; i++) {
        int id = index_of(text[i]);
        if (id >= 0)
            freq1[id]++;
    }

    for (int i = 0; i < n - 1; i++) {
        int a = index_of(text[i]);
        int b = index_of(text[i + 1]);
        if (a >= 0 && b >= 0) {
            freq2_over[a][b]++;
            total2_over++;
        }
    }

    for (int i = 0; i < n - 1; i += 2) {
        int a = index_of(text[i]);
        int b = index_of(text[i + 1]);
        if (a >= 0 && b >= 0) {
            freq2_nonover[a][b]++;
            total2_nonover++;
        }
    }

    double H1 = entropy_1(freq1, n);
    double H2_over = entropy_2(freq2_over, total2_over);
    double H2_nonover = entropy_2(freq2_nonover, total2_nonover);

    printf("\n====================================\n");
    printf("%s\n", title);
    printf("text length: %d symbols\n", n);
    printf("H1 = %.4f bit/symbol\n", H1);
    printf("H2 (overlap) = %.4f bit/symbol\n", H2_over);
    printf("H2 (disjoint) = %.4f bit/symbol\n", H2_nonover);
    save_all_tables(output_file, freq1, n, freq2_over, total2_over, freq2_nonover, total2_nonover);
}

int main(void) {
    setlocale(LC_ALL, "");
    static wchar_t text[MAX_TEXT];
    int n1 = read_text("text.txt", text, 0);
    if (n1 < 0) {
        printf("error\n");
        return 1;
    }
    process("text with spaces", text, n1, "tables_with_spaces.txt");
    int n2 = read_text("text.txt", text, 1);
    process("text without spaces", text, n2, "tables_no_spaces.txt");
    return 0;
}
