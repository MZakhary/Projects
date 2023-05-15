#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"

// Creates an item given a name, id.
struct Item* makeItem (char* name, int id, struct Item *next)
{
    struct Item* curItem = (struct Item*) malloc(sizeof(struct Item)); 
    curItem->name = name;
    curItem->id = id;
    curItem->next = next;
    return curItem;
}

/*A function that takes in a pointer to a linked list and an id, and puts the item in the linked list. It returns the head */
struct Item* putItem(struct Item *head, struct Item *item)
{
    struct Item *last = head;
    while (last->next != NULL)
    last = last->next;
    last->next = item;
    return head;
}

/*Function that takes in a pointer to a linked list, and an item id, and removes that item from a linked list. Returns the item removed.*/
struct Item* removeItem(struct Item *head, int id)
{
    
    struct Item *temp = head;
    struct Item *tempItem = head->next;
    if(tempItem == NULL)
    return NULL;
    
    if (tempItem != NULL && tempItem->id == id) /*If first item in list*/
    {
        head->next = tempItem->next;
        tempItem->next = NULL;
        return tempItem;
    }
    while (tempItem!= NULL && tempItem->id != id)
    {
        temp = temp->next;
        tempItem = tempItem->next;
    }
    if(tempItem->id == id)
    {
        temp->next = tempItem->next;
        tempItem->next = NULL;
        return tempItem;
    } else 
    {
        printf("That item was not found!\n");
        return NULL;
    }
    
    
}