My project is very simple to compile and run. There is a .out file already present for playing by typing ./game, but if you would
like to compile your own, simply type gcc adventure.c -o (insert name here)

The gameboard structure is a 3 by 3 board of rooms linked by pointers like a multidimensional linked list. The game board is created
by a seperate gameboard function that calls upon a makeRoom function in my rooms.c file which creates and returns a valid room with
dynamically allocated memory. The gameboard function then randomizes the location of names and sets those randomized names to
the rooms. The gameboard function then goes and connects all the rooms using a setNeighbor function that sets the pointers
of the directions of the room to the other rooms. Finally, the gameboard function returns the room the main player starts in.
From there on out, that is used to access the rest of the rooms.

The game runs on one big while loop in the main function that constantly scans strings with scanf. Typing one of the required strings
would then run that command's function, for example inputting "help" would then print out all the different commands you can input.
As per rubric specification, typing any string that requires further input (Like 'go' and 'clue' ect) would then put you in another
while loop that waits another valid string. It is very intuitive and everything is explained in the game itself.

This project was created in linux, I am unsure of its ability to run on other operating systems without some editting of the code.