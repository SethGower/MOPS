///
/// Description:  test driver for the MapADT module
///
/// This test program creates a map having structs as both
/// its keys and values, and performs a number of tests
/// on the map.
///
/// @author wrc
///
/// Based on the mapTest2.c test file.
///

// need setvbuf() declaration
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include "mapADT.h"

//
// Testing structures
//

#define NOTE_LENGTH 30

typedef struct
{
    unsigned int year;
    char         note[ NOTE_LENGTH ];
    unsigned int idNumber;
} TestStruct;

#define NAME_LEN 20

typedef struct {
    int  idNumber;
    char lname[ NAME_LEN ];
    char fname[ NAME_LEN ];
} UserStruct;

///
/// Sequence of test values to be inserted and removed
///
char *StrTestData[] = { "car", "Ford", "Chevy", "Toyota", "Jeep",
    "Samsung", "Apple", "HTC", "abcdefghijklmnopqrstuvwxyz", "12345" };
int NumStrData = 10;

unsigned int UintTestData2[] = { 2008, 1993, 1989, 1985, 2016, 1991,
    2005, 2002, 1987, 1990 };
int NumUintData2 = sizeof(UintTestData2) / sizeof(unsigned int);

UserStruct UserData[] = {
   { 297066, "Elliott", "Jeannie" },   { 498400, "Sparks", "Leigh" },
   { 321438, "Mendoza", "Glen" },      { 305834, "Adams", "Luke" },
   { 356597, "Brewer", "Caledonia" },  { 383011, "Clark", "Gregory" },
   { 128604, "Townsend", "Sadie" },    { 153315, "Powell", "Colleen" },
   { 242118, "Carlson", "Russell" },   { 176831, "Park", "Misty" },
   { 157253, "Figueroa", "Edna" },     { 188625, "Fenstermacher", "Timothy" },
   { 215653, "Burgess", "Rolando" },   { 305644, "Baskerville", "Montgomery" },
   { 455921, "Tate", "Maria" },        { 356639, "Castro-Herrera", "Ella" },
   { 419381, "Jimenez", "Christian" }, { 421006, "Stevenson", "Allan" },
   { 240662, "Williams", "Bethany" },  { 422251, "D'Ippolito", "Mona" },
   { 291516, "Mckenzie", "Mable" },    { 319642, "Cannon", "Rogelio" },
   { 342336, "Maxwell", "Dewey" },     { 198308, "Champion-Demers", "Tara" },
   { 254511, "West", "Christy" },      { 244956, "Barnes", "Claudia" },
   { 302203, "Yates", "Angelica" },    { 387826, "Farmer", "Fred" },
   { 152411, "Arnold", "Josephine" },  { 385473, "Kelly", "Karen" },
   { 210572, "DeFrancesco", "Kerry" }, { 301833, "Estrada", "Omar" },
   { 138056, "Wright", "Brad" },       { 121926, "Owen", "Jon" },
   { 116518, "Padilla", "Kevin" },     { 293528, "Shelton", "Evangeline" },
   { 231951, "Castro", "Louis" },      { 208811, "Perez", "Leon" },
   { 489665, "Myers", "Manuel" },      { 345588, "Pope", "Pierre-Andre" },
   { 299032, "Hale", "Craig" },        { 184563, "Chesterfield", "Clayton" },
   { 143638, "Simpson", "Linda" },     { 107591, "Vanderheyden", "Julius" },
   { 474864, "Shellenberger", "Kristin" },
   { 408346, "Fowler-Hornbuckle", "Glenda" },
   { 353816, "Chaisupakosol", "Alessandra" },
   { 487885, "Constantinides", "Bartholomew" },
   { 318398, "Baggerman-Webster", "Henrietta" },
   { 313363, "Ravindranath", "Venkatakrishna" },
};
int NumUserData = sizeof(UserData) / sizeof(UserStruct);

///
/// Comparison function for UserStructs
///
/// @param a first value to compare
/// @param b second value to compare
/// @return integer indicating the relationship between a and b
///
int compare_user( const void *a, const void *b ) {
    UserStruct *key1, *key2;

    key1 = (UserStruct*)a;
    key2 = (UserStruct*)b;

    // basically, just do equality testing for these

    int f1 = key1->idNumber != key2->idNumber;
    int f2 = strcmp( key1->lname, key2->lname );
    int f3 = strcmp( key1->fname, key2->fname );

    // 0 --> identical, else some difference
    return( f1 || f2 || f3 );
}

///
/// Process insertions/removals using a specific map
///
/// @param map the MapADT to use for the tests
///
void process( MapADT map) {
    UserStruct *key;
    TestStruct *value;
    const TestStruct *retVal;
    unsigned int countReplaced = 0;
    unsigned int tempKey;

    srand( 23 );

    printf( "Calling map_create\n" );
    map = map_create( compare_user );

    printf( "Putting entries in map\n" );
    for( int i=0; i<NumUserData; ++i ) {
        key = (UserStruct *) malloc( sizeof(UserStruct) );
        assert( key != NULL );
        strncpy( key->lname, UserData[i].lname, NAME_LEN );
        strncpy( key->fname, UserData[i].fname, NAME_LEN );
        key->idNumber = UserData[i].idNumber;

        value = (TestStruct*) malloc( sizeof(TestStruct) );
        assert( value != NULL );
        value->year = UintTestData2[rand() % NumUintData2];
        strncpy( value->note, StrTestData[rand() % NumStrData], NOTE_LENGTH );
        value->idNumber = rand();

        value = map_put( map, key, value );

        if( NULL != value ) {
            free( value );
            free( key );
            countReplaced++;
        }
    }

    printf( "Called map_put with a key already in the map: %d times\n",
            countReplaced );

    printf( "Calling map_get 10 times\n" );
    for( int i=0; i<10; ++i ) {
        tempKey = rand() % NumUserData;
	UserStruct *tmp = &UserData[tempKey];
        retVal = map_get( map, tmp );
        if( NULL == retVal ) {
            printf( "   key (%u/%s/%s) not found in map\n",
                    tmp->idNumber, tmp->lname, tmp->fname );
        } else {
            printf( "   key (%u/%s/%s) -- value:%u %s %u\n",
                    tmp->idNumber, tmp->lname, tmp->fname,
		    retVal->year, retVal->note, retVal->idNumber );
        }
    }
    printf( "Calling map_destroy\n" );
    map_destroy( map );
}

///
/// main routine - initiate all the tests
///
int main( void ) {
    MapADT myMap;

    //
    // to aid in debugging, we reset stdout to be line buffered
    // so that we don't lose any output if an assertion fails
    // even if stdout is redirected into a file
    //
    setvbuf( stdout, NULL, _IOLBF, BUFSIZ );

    myMap = map_create( compare_user );
    if( myMap == NULL ) {
        fputs( "Cannot create map with struct keys\n", stderr );
        exit( EXIT_FAILURE );
    }
    puts( "\nTesting the [struct, struct] map" );
    process( myMap );
    map_destroy( myMap );

    return 0;
}
