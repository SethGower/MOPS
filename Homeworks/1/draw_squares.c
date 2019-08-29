#include <stdio.h>

void draw_square(int side, int indent, char ch) {
    int i = 0;
    int j = 0;
    for (i = 0; i < side; i++) {
        for (j = 0; j < indent; j++) {
            putchar(' ');
        }
        for (j = 0; j < side; j++) {
            putchar(ch);
        }
        putchar('\n');
    }
}

int main() {
    draw_square(3, 2, '#');
    draw_square(5, 4, '*');
    draw_square(8, 3, '+');
    return 0;
}
