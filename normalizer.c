#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

#define MAX_TEXT 2000000

int read_text(const char *filename, wchar_t *text, int remove_spaces) {
    FILE *f = fopen(filename, "r");
    if (!f) return -1;
    wint_t ch;
    int n = 0;
    int prev_space = 0;
    while ((ch = fgetwc(f)) != WEOF && n < MAX_TEXT) {
        ch = towlower(ch);  
        if (ch == L'ё') ch = L'е';
        if (ch == L'ъ') ch = L'ь';

        if (!iswalpha(ch) && ch != L' ') {
            ch = L' ';
        }
        if (ch == L' ' && remove_spaces)
            continue;
        if ((ch >= L'а' && ch <= L'я') || ch == L' ') {
            if (ch == L' ') {
                if (!prev_space) {
                    text[n++] = L' ';
                    prev_space = 1;
                }
            } else {
                text[n++] = ch;
                prev_space = 0;
            }
        }
    }
    fclose(f);
    return n;
}
