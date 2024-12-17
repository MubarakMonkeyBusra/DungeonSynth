#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

int main() {
    char command[100];  // buffer for storing commands
    Player player = {"Player", 500, 500, 0};  // Initialize the player
    Inventory inventory = {{NULL}, 0};     // Initialize the inventory
    int game = 1, playerX = 2, playerY = 2;  // locating the player


    printf("Welcome to the Dungeon Adventure Game! Get ready for a breath taking adventure! \n");
    loadItems();
    loadCreatures();
    loadRooms();

    printMap(playerX, playerY);

    while (game) {
        printf("\nEnter command (WASD for movement, look, inventory, pickup, attack, exit, save, load): ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;


        int goes = 1; // Check if the command is w, a, s or d
        for (int i = 0; i < strlen(command); i++) {
            if (command[i] != 'w' && command[i] != 'a' && command[i] != 's' && command[i] != 'd') {
                goes = 0;  // If typed character is not w, a, s or d
                break;
            }
        }
        if (goes) {
            movePlayer(command, &playerX, &playerY);  // Moves player
            printMap(playerX, playerY);  // Print player's new position
        } else if (strcmp(command, "look") == 0) {
            look(playerCurrentRoomID);  // Look at the room
        } else if (strcmp(command, "inventory") == 0) {
            viewInventory(&inventory);  // Show inventory
        } else if (strncmp(command, "pickup", 6) == 0) {
            char itemName[50];  // create buffer to stor item names
            if (sscanf(command, "pickup %49[^\n]", itemName) == 1) {
                pickup(&inventory, playerCurrentRoomID, itemName); // pickup X
            } else {
                printf("Choose what item to pick up. For example: pickup gun\n");
            }
        } else if (strncmp(command, "attack", 6) == 0) {
            char creatureName[50];  // create buffer to store creature name to attack
            if (sscanf(command, "attack %49[^\n]", creatureName) == 1) {

                attack(&player, &inventory, rooms[playerCurrentRoomID].creature, creatureName); // attack that creature
            } else {
                printf("Choose what creature to attack. For example: attack zombie \n");
            }
        } else if (strcmp(command, "drop") == 0) {
            drop(&inventory, playerCurrentRoomID);  // move out an item from the inventory (to current room)
        } else if (strncmp(command, "save", 4) == 0) {
            char filepath[100];
            if (sscanf(command, "save %99[^\n]", filepath) == 1) {
                save(filepath, &player, &inventory);  // Save the game to that file
            } else {
                printf("Chose a file path to save game.\n");
            }
        } else if (strncmp(command, "load", 4) == 0) {
            char filepath[100];
            if (sscanf(command, "load %99[^\n]", filepath) == 1) {
                load(filepath, &player, &inventory);  // Load the game from that file
            } else {
                printf("Chose a file path to load game.\n");
            }
        } else if (strcmp(command, "exit") == 0) {
            game = 0; // End  game loop
        } else {
            printf("Invalid command! Please try again!\n");
        }
    }

    printf("Thanks for playing! It was a pleasure! \n");
    return 0;  // Exit  program
}



