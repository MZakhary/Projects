#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/*Item struct, holding an item ID and a pointer to the next item*/
struct Item
{
    char* name;
    int id;
    struct Item *next;
};

/*A function that makes an item and returns it*/
struct Item* makeItem (char* name, int id, struct Item *next); 

/*A function that takes in a pointer to a linked list and an id, and puts the item in the linked list. It returns nothing*/
struct Item* putItem(struct Item *head, struct Item *item);

/*Function that takes in a pointer to a linked list, and an item id, and removes that item from a linked list. Does not return the item removed.*/
struct Item* removeItem(struct Item *head, int id);
