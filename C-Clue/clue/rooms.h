#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

struct Room /*Room structure implementation*/
{
    char* name;
    int id;
    struct Room *North;
    struct Room *South;
    struct Room *East;
    struct Room *West;
    struct Item *inventory;
};

struct Room *makeRoom(char* n, int i); /*A function that creates a struct given a name and id*/

void setNeighbor(struct Room *curRoom, struct Room *n, struct Room *e, struct Room *s, struct Room *w); /*Function to set neighbors of a room, taking in the current room, the room to the north, east, south, and west respectively*/
