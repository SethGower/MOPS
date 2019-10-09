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
    int tempInt = 0; /* temp variable to convert to float */
    mode opMode = CURSOR;
    /* getopt stuff */
    while ((opt = getopt(argc, argv, getoptOptions)) != -1) {
        switch (opt) {
        case 'h':
            /* the ugly print out for help */
            /* Looks good when printed. Not so good in source code */
            fprintf(stderr, "usage:\n");
            fprintf(stderr,
                    "brace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v "
                    "%%vac] [-e %%end]\n");
            fprintf(stderr, "Option      Default   Example   Description\n");
            fprintf(
                stderr,
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
            /* changes the string of the argument to delay uS */
            delay = (int)strtol(optarg, NULL, 10);
            if (delay < 1) {
                delay = 900000;
            }
            break;
        case 'c':
            /* changes string argument to max num of cycles */
            maxCycles = (int)strtol(optarg, NULL, 10);
            opMode = PRINT;
            if (maxCycles < 0) {
                fprintf(stderr, "count (%d) must be an non-negative integer\n",
                        maxCycles);
                fprintf(stderr, "usage:\n");
                fprintf(stderr,
                        "%s [-h] [-t N] [-c N] [-d dim] [-s "
                        "%%str] [-v %%vac] [-e %%end]\n",
                        argv[0]);
                return 2;
            }
            break;
        case 'd':
            /* the dimension of the square grid */
            dim = (int)strtol(optarg, NULL, 10);
            if (dim < 5 || dim > 39) {
                fprintf(stderr, "dimension (%d) must be a value in [5...39]\n",
                        dim);
                fprintf(stderr, "usage:\n");
                fprintf(stderr,
                        "%s [-h] [-t N] [-c N] [-d dim] [-s "
                        "%%str] [-v %%vac] [-e %%end]\n",
                        argv[0]);
                return 2;
            }
            break;
        case 's':
            /* strength of preference, aka threshold */
            tempInt = strtol(optarg, NULL, 10);

            if (tempInt < 1 || tempInt > 99) {
                fprintf(
                    stderr,
                    "preference strength (%d) must be a value in [1...99]\n",
                    tempInt);
                fprintf(stderr,
                        "%s [-h] [-t N] [-c N] [-d dim] [-s "
                        "%%str] [-v %%vac] [-e %%end]\n",
                        argv[0]);
                return 2;
            }
            threshold = (double)tempInt / 100.0;
            break;
        case 'v':
            /* percent vacancy */
            tempInt = strtol(optarg, NULL, 10);
            if (tempInt < 1 || tempInt > 99) {
                fprintf(stderr, "vacancy (%d) must be a value in [1...99]\n",
                        tempInt);
                fprintf(stderr,
                        "%s [-h] [-t N] [-c N] [-d dim] [-s "
                        "%%str] [-v %%vac] [-e %%end]\n",
                        argv[0]);
                return 2;
            }
            percVac = (double)tempInt / 100.0;
            break;
        case 'e':
            /* percent of total agents that are end line characters */
            tempInt = strtol(optarg, NULL, 10);
            if (tempInt < 1 || tempInt > 99) {
                fprintf(stderr,
                        "endline proportion (%d) must be a value in [1...99]\n",
                        tempInt);
                fprintf(stderr,
                        "%s [-h] [-t N] [-c N] [-d dim] [-s "
                        "%%str] [-v %%vac] [-e %%end]\n",
                        argv[0]);
                return 2;
            }
            percEnd = (double)tempInt / 100.0;
            break;
        case '?':
            /* fprintf(stderr, "brace-topia: invalid option -- %c\n",
             * (char)opt); */
            fprintf(stderr, "usage:\n");
            fprintf(stderr,
                    "%s [-h] [-t N] [-c N] [-d dim] [-s "
                    "%%str] [-v %%vac] [-e %%end]\n",
                    argv[0]);
            return 1;
        default:
            fprintf(stderr, "usage:\n");
            fprintf(stderr,
                    "%s [-h] [-t N] [-c N] [-d dim] [-s "
                    "%%str] [-v %%vac] [-e %%end]\n",
                    argv[0]);
        }
    }

    returnCode = createBracetopia(&board, dim, threshold, percVac, percEnd);
    if (returnCode) {
        fprintf(stderr, "Game creation failed. Exiting\n");
    } else {
        populateBoard(&board, percVac, percEnd);

        /* check which mode to run in. This is decided if the user passed in a
         * cycle max. If so, then it defaults to print mode */
        if (opMode == PRINT) {
            for (currCycle = 0; currCycle <= maxCycles; currCycle++) {
                printGrid(&board, currCycle, movesCycle);
                movesCycle = move(&board);
            }
        } else {
            /* enable cursor control */
            while (true) {
                clear();
                set_cur_pos(0, 0);
                printGrid(&board, currCycle++, movesCycle);
                movesCycle = move(&board);
                usleep(delay);
            }
        }
    }
    destroyBracetopia(&board);

    return returnCode;
}
