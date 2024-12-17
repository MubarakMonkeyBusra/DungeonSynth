# DungeonSynth
Dungeon Game in C 

## Overview 
This is a console-based dungeon exploration game written in C. The player begins their adventure in a dungeon room with at least one open exit. Moving through an exit does not close it, but revisiting a room will reset its contents due to memory management constraints.
Each room can contain a single item and up to four creatures lurking in the shadows. The player must inspect the room using the look command to uncover these hidden details.

##  Game Mechanics
	•	Monsters: Zombie, Vampire, Orc, Serpent
	•	Items: Knife, Gun, Kalashnikov, Bomb
	•	Rooms: Entrance, Ancient Library, Hall of Echoes, Cavern of Whispers

## Gameplay
w, a, s, d —> Move up, left, down, or right
look —> Look around in the current room.
inventory —> View items in your inventory.
pickup <item>—> Pick up an item in the room.
attack <creature> —>Attack a creature in the room.
drop <item> —> Drop an item from your inventory.
save <file> —> Save the game to a file.
load <file> —> Load a saved game from a file.
exit —> Quit the game.

##   Project Structure

The project follows a clean and modular structure:

DungeonSynth/
├── build/                    # Compiled output (created after build)
├── src/                       # Source files
│   ├── main.c            # Main game loop
│   ├── game.c           # Core game logic
│   └── game.h           # Header file for structures and prototypes
├── CMakeLists.txt      # CMake configuration file
├── Makefile                 # Makefile to build the project
└── README.md         # Project documentation (this file)
