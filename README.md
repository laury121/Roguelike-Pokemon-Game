Author: Lauren Palan
Title: User Interface

Assignment Description:
Adding user interface using the ncurses library. The user  directly controls the player character through keyboard input of 123456789 or hjklyubn. The interface should update in real time without requiring the enter key.

My Project:
I used ncurses to handle input and rendering. The PC is controlled with a switch statement that maps the key presses to movement and checks for bounds and collisions to ensure valid movement. The game loop continues until the user presses shift+q, and then the process gets terminated.

Sources:
1) I used the following website for the ncurses 
https://dev.to/tbhaxor/introduction-to-ncurses-part-1-1bk5
