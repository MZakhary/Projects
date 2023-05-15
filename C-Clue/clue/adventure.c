#include "rooms.c"
#include "characters.c"
#include "items.c"
#include <time.h>

/*Note: The location number is set up like a numpad going from 1 to 9, with 1 being the bottom left corner and 9 being the top right corner.
This means that 1 has null to the south and west and has 2 to the east and 4 to the north, so on and so forth.
This allows for an easy algorithm where north is location+3, south is location -3, east is location +1, and west is location -1*/

/*This function sets up the actual gameboard, creating and assigning memory to all 9 rooms and randomizing the names, it then returns a pointer to the first room (the bottom left room)*/
struct Room *gameboard()
{
    char *name[9] = {"Cafeteria", "Admin", "Reactor", "Lower Engine", "Upper Engine", "Security", "Storage", "Shields", "Navigation"};
    struct Room *Rooms[9];
    int i;

    for (i = 0; i < 9; ++i)
    {
        int randomNumber = rand() % 9;
        char *temp = name[randomNumber];
        name[randomNumber] = name[i];
        name[i] = temp;
    }

    i = 0;

    for (i = 0; i < 9; ++i)
    {
        Rooms[i] = makeRoom(name[i], i);
    }

    /*Below this is the process that links up all the rooms by pointers manually, basically returning a linked list of rooms*/
    setNeighbor(Rooms[0], Rooms[3], Rooms[1], NULL, NULL);
    setNeighbor(Rooms[1], Rooms[4], Rooms[2], NULL, Rooms[0]);
    setNeighbor(Rooms[2], Rooms[5], NULL, NULL, Rooms[1]);
    setNeighbor(Rooms[3], Rooms[6], Rooms[4], Rooms[0], NULL);
    setNeighbor(Rooms[4], Rooms[7], Rooms[5], Rooms[1], Rooms[3]);
    setNeighbor(Rooms[5], Rooms[8], NULL, Rooms[2], Rooms[4]);
    setNeighbor(Rooms[6], NULL, Rooms[7], Rooms[3], NULL);
    setNeighbor(Rooms[7], NULL, Rooms[8], Rooms[4], Rooms[6]);
    setNeighbor(Rooms[8], NULL, NULL, Rooms[5], Rooms[7]);
    return Rooms[0];
}

void main() /*This is my main function where the majority of the code is. It is how the game is played*/
{
    /*Here im creating the randomized win condition for later use in the clue command*/
    srand(time(0));
    int winroom = rand() % 9;
    int winchar = 2 + rand() % 5;
    int winitem = 1 + rand() % 6;

    /*Here i use the gameboard function to create the gameboard, and set the room it returns as the "Start"ing room*/
    /*I then create the characters and assign their location based on the Starting room, sending 1 character to a different room*/
    struct Room *Start = gameboard();

    /*Here I create all the characters including the player's avatar, as well as create all the inventories for the rooms.
    I use placeholder items to create the inventories, these placeholder items are hidden from the player and cannot be seen
    using any of the look/inventory commands. While you can technically try to remove them, it will most likely cause
    critical error and crash the game, so since you can see this as you (and not the average player) can read C code, please
    do not remove these items. All the items in the rooms are also created here.*/
    struct Character *Chara[7];
    Chara[1] = makeChar("Player", 1, Start);
    Chara[2] = makeChar("Red", 2, Start->North->North);
    Chara[3] = makeChar("Blue", 3, Start->North->East);
    Chara[4] = makeChar("Yellow", 4, Start->East);
    Chara[5] = makeChar("White", 5, Start->East->East);
    Chara[6] = makeChar("Black", 6, Start->North);
    Start->inventory = makeItem("placeholder", 0, NULL);
    Start->North->inventory = makeItem("placeholder", 0, makeItem("Knife", 1, NULL));
    Start->North->North->inventory = makeItem("placeholder", 0, makeItem("Gun", 2, NULL));
    Start->East->inventory = makeItem("placeholder", 0, makeItem("Shotgun", 3, NULL));
    Start->East->East->inventory = makeItem("placeholder", 0, makeItem("Cake", 4, NULL));
    Start->East->North->inventory = makeItem("placeholder", 0, makeItem("Sword", 5, NULL));
    Start->East->East->North->inventory = makeItem("placeholder", 0, makeItem("Chicken", 6, NULL));
    Start->North->North->East->inventory = makeItem("placeholder", 0, NULL);
    Start->North->North->East->East->inventory = makeItem("placeholder", 0, NULL);
    Chara[1]->inventory = makeItem("placeholder", 0, NULL);

    /*This is technically where my game starts, printing a starting statement and using a massive while loop to wrap around everything with a "win" variable.*/
    /*All command inmplementations are below*/
    printf("\nWelcome to my text adventure game rendition of the hit game amogus based to fit Clue. \nYou will be playing as the purple crewmate who is on their quest to locate the imposter! \nUse the 'help' command to see the other commands you have access to.\n\n");
    printf("You are given a secret hint: %d %d %d \n\n\n\n", winroom, winchar, winitem); /*This hint is actually the 3 winning ids in order of room character item. Use this so you know how to win easily after reading my code!*/
    int win = 0;                                                                         /*Variable that decides if you win or not.*/
    int clueAttempt = 0;
    while (win == 0)
    {
        char com[20]; /*The variable that takes in a player command and executes a different if statement depending on the command.*/
        scanf("%20s", com);

        if (0 == strcmp(com, "win")) /*This is a test function for testing winning, where if you type in the command "win" you win, just for ease of grading and creation.*/
        {
            win = 1;
            break;
        }
        if (0 == strcmp(com, "help")) /*The simple help function which tells you about all the commands you can use.*/
        {
            printf("The commands you may type are 'go' which require you to enter a direction, \n'look' which provides information about your whereabouts, \n You can use 'list' in order to list all characters rooms and items. \nand 'clue' in order to gain a clue or attempt to win. \nWhen using Clue, you have to input a character which will move to your room. \nYou only have 10 tries with the clue command. \nYou may also check your 'Inventory', 'Take' items from the room or 'Drop' items from your inventory. \n\n\n");
        }
        if (0 == strcmp(com, "list")) /*A simple list function that lists all characters, items, and rooms*/
        {
            printf("The rooms are: Cafeteria, Admin, Reactor, Lower Engine, Upper Engine, Security, Storage, Shields, and Navigation.\n The characters in question are: Red, Blue, Yellow, White, and Black.\n The items in question are: Knife, Gun, Shotgun, Cake, Sword, and Chicken.\n");
        }
        if (0 == strcmp(com, "go")) /*The simple Go function which prompts you to enter a direction then moves the player to that direction if valid, prompting another direction if invalid*/
        {
            printf("Where would you like to go? Enter: 'north', 'south', 'east', or 'west' \n");
            char dir[20];
            int x = 0;
            while (x == 0)
            {
                scanf("%20s", dir);
                if (0 == strcmp(dir, "cancel"))
                    break;
                if (strcmp(dir, "north") != 0 && strcmp(dir, "south") != 0 && strcmp(dir, "east") != 0 && strcmp(dir, "west") != 0)
                {
                    printf("That is an invalid input, Please type 'north', 'south', 'east', or 'west' Alternatively, you may type 'cancel' to cancel movement.");
                }
                if (0 == strcmp(dir, "north"))
                {
                    if (Chara[1]->location->North != NULL)
                    {
                        Chara[1]->location = Chara[1]->location->North;
                        x = 1;
                        printf("You move to %s\n\n", Chara[1]->location->name);
                    }
                    else
                    {
                        printf("There is no door that way!\n");
                    }
                }
                if (0 == strcmp(dir, "south"))
                {
                    if (Chara[1]->location->South != NULL)
                    {
                        Chara[1]->location = Chara[1]->location->South;
                        x = 1;
                        printf("You move to %s\n\n", Chara[1]->location->name);
                    }
                    else
                    {
                        printf("There is no door that way!\n");
                    }
                }
                if (0 == strcmp(dir, "west"))
                {
                    if (Chara[1]->location->West != NULL)
                    {
                        Chara[1]->location = Chara[1]->location->West;
                        x = 1;
                        printf("You move to %s\n\n", Chara[1]->location->name);
                    }
                    else
                    {
                        printf("There is no door that way!\n");
                    }
                }
                if (0 == strcmp(dir, "east"))
                {
                    if (Chara[1]->location->East != NULL)
                    {
                        Chara[1]->location = Chara[1]->location->East;
                        x = 1;
                        printf("You move to %s\n\n", Chara[1]->location->name);
                    }
                    else
                    {
                        printf("There is no door that way!\n");
                    }
                }
            }
        }

        /*The look function, returning vital information about the player's whereabouts*/
        if (0 == strcmp(com, "look"))
        {
            printf("You are currently in %s\n", Chara[1]->location->name);

            if (Chara[1]->location->North != NULL)
                printf("To the north is a door to %s\n", Chara[1]->location->North->name);
            if (Chara[1]->location->East != NULL)
                printf("To the east is a door to %s\n", Chara[1]->location->East->name);
            if (Chara[1]->location->South != NULL)
                printf("To the south is a door to %s\n", Chara[1]->location->South->name);
            if (Chara[1]->location->West != NULL)
                printf("To the west is a door to %s\n", Chara[1]->location->West->name);
            int characterCheck;
            for (characterCheck = 2; characterCheck < 7; ++characterCheck)
            {
                if (Chara[characterCheck]->location == Chara[1]->location)
                    printf("The %s crewmate is here\n", Chara[characterCheck]->name);
            }
            struct Item *temp = Chara[1]->location->inventory->next;
            while (temp != NULL)
            {
                if (temp == NULL)
                    break;
                else
                {
                    printf("There is a '%s' on the ground.\n", temp->name);
                    temp = temp->next;
                }
            }
        }

        /*The inventory function, returning all the items in the player's inventory.*/
        if (0 == strcmp(com, "inventory"))
        {
            struct Item *temp = Chara[1]->inventory;
            if (temp->next == NULL)
                printf("You have nothing in your inventory. \n");
            else
            {
                temp = temp->next;
                while (temp != NULL)
                {
                    if (temp == NULL)
                        break;
                    else
                    {
                        printf("You have a '%s'\n", temp->name);
                        temp = temp->next;
                    }
                }
            }
        }

        if (0 == strcmp(com, "take")) /*This function asks the player for a string and checks its name, then uses the name to link it to an ID and finally uses the ID to remove the item from the the room and add it to the player's linked list*/
        {
            printf("Which item would you like to take? \n");
            char itemName[20];
            int h = 0;
            while (h == 0)
            {
                scanf("%20s", itemName);
                if (0 == strcmp(itemName, "cancel"))
                {
                    break;
                }
                if (strcmp(itemName, "knife") != 0 && strcmp(itemName, "gun") != 0 && strcmp(itemName, "chicken") != 0 && strcmp(itemName, "shotgun") != 0 && strcmp(itemName, "sword") != 0 && strcmp(itemName, "cake") != 0 && strcmp(itemName, "cancel") != 0)
                {
                    printf("That is not an actual item, please type an item name.\n");
                }
                int itemid;
                if (strcmp(itemName, "knife") == 0)
                    itemid = 1;
                if (strcmp(itemName, "gun") == 0)
                    itemid = 2;
                if (strcmp(itemName, "shotgun") == 0)
                    itemid = 3;
                if (strcmp(itemName, "cake") == 0)
                    itemid = 4;
                if (strcmp(itemName, "sword") == 0)
                    itemid = 5;
                if (strcmp(itemName, "chicken") == 0)
                    itemid = 6;
                struct Item *temp = Chara[1]->location->inventory;
                int pickedup = 0;
                while (temp != NULL)
                {
                    if (temp->id == itemid)
                    {
                        h = 1;
                        putItem(Chara[1]->inventory, removeItem(Chara[1]->location->inventory, itemid));
                        printf("You picked up a %s\n", temp->name);
                        pickedup = 1;
                    }
                    temp = temp->next;
                }
                if (pickedup == 0)
                {
                    printf("That item is not in the room!\n");
                }
            }
        }

        if (0 == strcmp(com, "drop")) /*This function asks the player for a string and checks its name, then uses the name to link it to an ID and finally uses the ID to remove the item from the player's inventory and add it to the room's linked list*/
        {
            printf("Which item would you like to drop \n");
            char itemName[20];
            int h = 0;
            while (h == 0)
            {
                scanf("%20s", itemName);
                if (0 == strcmp(itemName, "cancel"))
                {
                    break;
                }
                if (strcmp(itemName, "knife") != 0 && strcmp(itemName, "gun") != 0 && strcmp(itemName, "chicken") != 0 && strcmp(itemName, "shotgun") != 0 && strcmp(itemName, "sword") != 0 && strcmp(itemName, "cake") != 0 && strcmp(itemName, "cancel") != 0)
                {
                    printf("That is not an actual item, please type an item name.\n");
                }
                int itemid;
                if (strcmp(itemName, "knife") == 0)
                    itemid = 1;
                if (strcmp(itemName, "gun") == 0)
                    itemid = 2;
                if (strcmp(itemName, "shotgun") == 0)
                    itemid = 3;
                if (strcmp(itemName, "cake") == 0)
                    itemid = 4;
                if (strcmp(itemName, "sword") == 0)
                    itemid = 5;
                if (strcmp(itemName, "chicken") == 0)
                    itemid = 6;

                struct Item *temp = Chara[1]->inventory;
                while (temp != NULL && temp->id != itemid)
                {
                    temp = temp->next;
                }
                if (temp == NULL)
                {
                    printf("You do not have that item!\n");
                    h = 1;
                }
                else
                {
                    putItem(Chara[1]->location->inventory, removeItem(Chara[1]->inventory, temp->id));
                    printf("You dropped a %s\n", temp->name);
                    h = 1;
                }
            }
        }

        /*The Clue function, which is what allows you to win the game. Invoking it asks for a character that it then moves to a player room.*/
        /*It then proceeds to check the player's location, the character in the room, and the winning item's location, incrementing a win counter for each one*/
        /*When the wincounter is 3, aka when all 3 are correct, it ends the main function while loop and goes to the end game screen.*/
        if (0 == strcmp(com, "clue"))
        {
            int wincounter = 0;
            int identification = 0;
            int p = 0;
            while (p == 0)
            {
                char command[20];
                printf("Please enter a character name.\n");
                scanf("%20s", command);
                if (0 == strcmp(command, "cancel"))
                {
                    p = 1;
                    break;
                }
                else if (strcmp(command, "red") != 0 && strcmp(command, "blue") != 0 && strcmp(command, "yellow") != 0 && strcmp(command, "white") != 0 && strcmp(command, "black") != 0)
                {
                    printf("That is not a valid character. You may type cancel to cancel this command.\n\n");
                }
                else
                {
                    if (0 == strcmp(command, "red"))
                        identification = 2;
                    if (0 == strcmp(command, "blue"))
                        identification = 3;
                    if (0 == strcmp(command, "yellow"))
                        identification = 4;
                    if (0 == strcmp(command, "white"))
                        identification = 5;
                    if (0 == strcmp(command, "black"))
                        identification = 6;
                    Chara[identification]->location = Chara[1]->location;
                    p = 1;
                }
            }

            if (Chara[1]->location->id == winroom)
            {
                printf("Room Match\n");
                ++wincounter;
            }

            if (identification == winchar)
            {
                printf("Character Match\n");
                ++wincounter;
            }
            struct Item *temproominventory = Chara[1]->location->inventory;
            while (temproominventory != NULL && temproominventory->id != winitem)
            {
                temproominventory = temproominventory->next;
            }

            struct Item *tempcharinventory = Chara[1]->inventory;
            while (tempcharinventory != NULL && tempcharinventory->id != winitem)
            {
                tempcharinventory = tempcharinventory->next;
            }
            if (temproominventory != NULL || tempcharinventory != NULL)
            {
                printf("Item Match\n");
                ++wincounter;
            }
            if (wincounter == 0)
                printf("No Match\n\n");
            if (wincounter == 3)
            {
                win = 1;
                break;
            }
            else
                ++clueAttempt;
        }

        if (clueAttempt > 9)
            break;
    }

    if (win == 1)
    {
        printf("Congratulations, you won!\n");
    }
    else
        printf("You tried to guess too many times and didnt get it right. You lose.\n");

    /*Freeing all the allocated memories for all characters, items, rooms*/
    free(Start->inventory);
    free(Start->North->inventory);
    free(Start->North->North->inventory);
    free(Start->East->inventory);
    free(Start->East->East->inventory);
    free(Start->East->North->inventory);
    free(Start->East->East->North->inventory);
    free(Start->North->North->East->inventory);
    free(Start->North->North->East->East->inventory);
    free(Start->North->North);
    free(Start->North);
    free(Start->East->East);
    free(Start->East->North->North->East);
    free(Start->East->North->East);
    free(Start->East->North);
    free(Start->East);
    free(Start);
    int freeint;
    for (freeint = 1; freeint < 7; ++freeint)
    {
        free(Chara[freeint]);
    }

    return;
}