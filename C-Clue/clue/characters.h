#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* Structure for Non playable characters, takes in a name, a current location, and an id. id is used for purposes of win condition and movement*/
struct Character
{
    char* name;
    int id;
    struct Room *location;
    struct Item *inventory;
};

// struct Character *makeChar(char* n, int i, struct Room* curRoom);