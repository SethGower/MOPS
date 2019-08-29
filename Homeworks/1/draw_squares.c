#include <stdio.h>

/* draws a square of a given size, indented by a certain amount and created
 * with a certain character */
void draw_square(int side, int indent, char ch) {
    int i = 0;
    int j = 0;
    /* outer loop to perform the rows */
    for (i = 0; i < side; i++) {
        /* handle the indents */
        for (j = 0; j < indent; j++) {
            putchar(' ');
        }
        /* handle the columns */
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
