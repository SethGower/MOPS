// racer.h - thread controlling a figure that races across a character window
// @author CS@RIT.EDU
// @contributor tjb


#ifndef _RACER_H
#define _RACER_H

#include <stdlib.h>
#include <stdio.h>

#define FINISH_LINE 70    ///< terminal column of victory
#define MAX_NAME_LEN 9    ///< not including the trailing NUL byte
#define DEFAULT_WAIT 200  ///< default wait time milliseconds

/// Rcr struct 
typedef struct Rcr {

    int dist;             ///< current distance from starting line (column)

    int row;              ///< vertical position of racer, i.e. "racing lane"

    char *graphic;        ///< graphic: the drawable text
} racer_t;


/// init_racers - Do setup work for all racers at the start of the program.
/// @param milliseconds length of pause between steps in animation 

void init_racers( long milliseconds );

/// make_racer - Create a new racer.
///
/// @param name the string name to show on the display for this racer
/// @param position the row in which to race
/// @return racer_t pointer a dynamically allocated racer_t object
/// @pre strlen( name ) < MAX_NAME_LEN, for display reasons.

racer_t *make_racer( char *name, int position );

/// destroy_racer - Destroy all dynamically allocated storage for a racer.
///
/// @param racer the object to be de-allocated

void destroy_racer( racer_t *racer );

/// run Run one racer in the race.
///
/// Initialize the display of the racer*:
///   The racer starts at the start position, column 1.
///   The racer's graphic (text name) is displayed.
/// The actions below happen repetitively, until its position is at FINISH_LINE:
///   Randomly calculate a waiting period, between 0 and 
///     the value given to init_racers
///   Sleep for that length of time.
///   Change the display position of this racer by +1 column*:
///     Erase the racer's name from the display.
///     Update the racer's dist field by +1.
///     Display the racer's name at the new position.
///
/// This function executes as a 'thread main' for each racer_t instance.
///
/// *note: code must ensure that each 1 column move of one racer is "atomic".
///
/// @param racer a racer_t pointer, declared as void* for pthread compatibility 
/// @return void pointer to result  
/// @pre racer cannot be NULL.

void *run( void *racer );

#endif
