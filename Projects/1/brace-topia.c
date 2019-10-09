#include "DebugMacros.h"
#include "bracetopia.h"
#include "display.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#undef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
typedef enum { CURSOR, PRINT } mode;
int main(int argc, char *argv[]) {
    BracetopiaBoard board;
    int dim = 15;
    double percVac = 0.2;
    double percEnd = 0.6;
    double threshold = 0.5;
    int delay = 900000;
    int maxCycles = 0, currCycle = 0;
    int movesCycle = 0;
    int returnCode = 0;
    char *getoptOptions = "ht:c:d:s:v:e:";
    int opt;
    mode opMode = CURSOR;
    while ((opt = getopt(argc, argv, getoptOptions)) != -1) {
        switch (opt) {
        case 'h':
            fprintf(stderr, "usage:\n");
            fprintf(stderr,
                    "brace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v "
                    "%%vac] [-e %%end]\n");
            fprintf(stderr, "Option      Default   Example   Description\n");
            printf(
                "'-h'        NA        -h        print this usage message.\n");
            fprintf(stderr,
                    "'-t N'      900000    -t 5000   microseconds cycle "
                    "delay.                 \n");
            fprintf(stderr,
                    "'-c N'      NA        -c4       count cycle maximum "
                    "value.                \n");
            fprintf(stderr, "'-d dim'    15        -d 7      width and height "
                            "dimension.               \n");
            fprintf(stderr,
                    "'-s %%str'   50        -s 30     strength of preference.  "
                    "                 \n");
            fprintf(stderr,
                    "'-v %%vac'   20        -v30      percent vacancies.       "
                    "                 \n");
            fprintf(stderr,
                    "'-e %%endl'  60        -e75      percent Endline braces. "
                    "Others want Newline.\n");
            return 0;
        case 't':
            delay = (int)strtol(optarg, NULL, 10);
            break;
        case 'c':
            maxCycles = (int)strtol(optarg, NULL, 10);
            opMode = PRINT;
            break;
        case 'd':
            dim = (int)strtol(optarg, NULL, 10);
            break;
        case 's':
            threshold = strtod(optarg, NULL) / 100.0;
            break;
        case 'v':
            percVac = strtod(optarg, NULL) / 100.0;
            break;
        case 'e':
            percEnd = strtod(optarg, NULL) / 100.0;
            break;
        }
    }

    returnCode = createBracetopia(&board, dim, threshold, percVac, percEnd);
    populateBoard(&board, percVac, percEnd);

    if (opMode == PRINT) {
        for (currCycle = 0; currCycle <= maxCycles; currCycle++) {
            printGrid(&board, currCycle, movesCycle);
            movesCycle = move(&board);
        }
    } else {
        while (true) {
            clear();
            set_cur_pos(0, 0);
            printGrid(&board, currCycle++, movesCycle);
            movesCycle = move(&board);
            usleep(delay);
        }
    }

    destroyBracetopia(&board);
    return returnCode;
}
