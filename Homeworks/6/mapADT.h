///
/// Description:  Interface to the Map ADT module.
///
/// @author ckd
/// @author wrc
///

#ifndef _MAPADT_H_
#define _MAPADT_H_

#include <stdbool.h>

///
/// The MapADT type (opaque to the application)
///
typedef struct mapStruct *MapADT;

///
/// Create a MapADT which uses the supplied function as a comparison
/// routine.
///
/// The comparison function takes two void* parameters, and returns
/// an integer result which indicates if the two things are identical: 
///
///	Result	Relationship
///	======	===========
///	 < 0	a <  b
///	 = 0	a == b
///	 > 0	a >  b
///
/// @param compare the address of the comparison function to be used for
///    determining the relationship between two keys
/// @return the created MapADT, or NULL if the allocation fails
///
MapADT map_create( int (*compare)(const void *a, const void *b) );

///
/// Tear down and deallocate the supplied MapADT.
///
/// @param map - the MapADT to be manipulated
///
void map_destroy( MapADT map );

///
/// Remove all contents from the supplied MapADT, but do not
/// deallocate the MapADT itself.
///
/// @param map - the MapADT to be manipuated
///
void map_clear( MapADT map );

///
/// Check if the specified entry exists in the map
///
/// Uses the map's equals function to determine if an entry with
/// the same key already exists in the map. If so, then return
/// true.
///
/// @param key the key to lookup in the map
/// @return true if the entry exists, else false
///
bool map_contains( const MapADT map, const void *key );

///
/// Put the specified entry into the Map
///
/// Uses the map's equals function to determine if an entry with
/// the same key already exists in the map. If so, then replace it 
/// and return a pointer to the old value.
///
/// @param map the MapADT into which the value is to be inserted
/// @param key the key to be inserted
/// @param value the value to be inserted
/// @exception If the value cannot be inserted into the map for
///     whatever reason (usually implementation-dependent), the program
///     should terminate with an error message.  This can be done by
///     printing an appropriate message to the standard error output and
///     then exiting with EXIT_FAILURE, or by having an assert() fail.
///
void* map_put( MapADT map, void *key, void *value);

///
/// Find the entry specified by the key and return a pointer to the value
///
/// @param map the MapADT to be manipulated
/// @return the value that corresponds to the key
/// @exception If the map is empty, the program should terminate
///     with an error message.  This can be done by printing an
///     appropriate message to the standard error output and then
///     exiting with EXIT_FAILURE, or by having an assert() fail.
///
const void *map_get( const MapADT map, const void *key );

///
/// Find the entry specified by the key and remove it from the MapADT,
/// returning the value pointer. 
///
/// @param map the MapADT to be manipulated
/// @return the value that was removed from the map
/// @exception If the map is empty, the program should terminate
///     with an error message.  This can be done by printing an
///     appropriate message to the standard error output and then
///     exiting with EXIT_FAILURE, or by having an assert() fail.
///
void *map_delete( MapADT map, const void *key );

///
/// Indicate whether or not the supplied Map is empty
///
/// @param the MapADT to be tested
/// @return true if the map is empty, otherwise false
///
bool map_empty( const MapADT map );

#endif

