/// file: warning.c
/// description: faulty code example that needs repair.
/// @author RIT CS Dept.

#include <stdio.h>

#define SIZE 1 ///< size of the array

/// compute and return the year representing the current millenium
/// using the century parameter to compute the result.
/// @param century integer number of the current century AD
/// @return year of the last presidential election

int compute(int century) {
    int b = (century - 1) * 100;
    return b + 16;
}

/// main computes and prints the year of the next presidential election.
/// The function should have no parameters and uses compute for its job.
/// @return 0 to tell the OS that the process ran to a success conclusion

int main() {
    int y = compute(21);
    printf("Last Presidental Election: %d\n", y);
    printf("Next Presidental Election: %d\n", y + 4);

    return 0;
}
