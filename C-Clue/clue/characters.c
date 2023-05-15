#include "characters.h"

struct Character *makeChar(char* n, int i, struct Room* curRoom)  /*A function used to create a character and dynamically allocate memory to them, setting a name id and location, and returning the character*/
{
    struct Character *chara = (struct Character*) malloc(sizeof(struct Character)); 
    chara->name = n;
    chara->id = i;
    chara->location = curRoom;
    return chara;
}