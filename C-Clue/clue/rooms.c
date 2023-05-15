#include "rooms.h"

struct Room *makeRoom(char* n, int i) /*A function that creates a struct given a name and id*/
{
    struct Room* curRoom = (struct Room*) malloc(sizeof(struct Room)); 
    curRoom->name = n;
    curRoom->id = i;
    curRoom->North = NULL;
    curRoom->East = NULL;
    curRoom->West = NULL;
    curRoom->South = NULL;
    
    return curRoom;
}

void setNeighbor(struct Room *curRoom, struct Room *n, struct Room *e, struct Room *s, struct Room *w)  /*Function to set neighbors of a room, taking in the current room, the room to the north, east, south, and west respectively*/
{
    curRoom->North = n;
    curRoom->East = e;
    curRoom->West = w;
    curRoom->South =s;
}


