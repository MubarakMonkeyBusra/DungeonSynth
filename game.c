#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int playerCurrentRoomID = 0;  // player starts in room 0
Item items[4];                // store items
Creature creatures[4];        // store creatures
Room rooms[4];                // store rooms

int playerX = 2, playerY = 2; // player coordinates at start

void printMap(int playerX, int playerY) {
    printf("\n---------------------------------------------------\n");
    for (int i = 0; i < HEIGHT * 2; i++) {
        printf("|");
        for (int j = 0; j < WIDTH * 2; j++) {
            if (i == playerY && j == playerX) {
                printf(" P ");
            } else if (i == 1 && j == WIDTH + 4) {
                printf("Zom");
            } else if (i == HEIGHT + 1 && j == 2) {
                printf("Orc");
            } else if (i == 1 && j == 4) {
                printf("Srp");
            } else if (i == HEIGHT + 1 && j == WIDTH + 4) {
                printf("Vam");
            } else {
                printf(" . ");
            }
            if (j == WIDTH - 1 || j == WIDTH * 2 - 1) printf("|");
        }
        printf("\n");
        if (i == HEIGHT - 1) printf("---------------------------------------------------\n");
    }
    printf("---------------------------------------------------\n");
}

void movePlayer(char *directions, int *playerX, int *playerY) {
    for (int i = 0; directions[i] != '\0'; i++) {
        switch (directions[i]) {
        case 'w': if (*playerY > 0) (*playerY)--; break; // Move up
        case 's': if (*playerY < HEIGHT * 2 - 1) (*playerY)++; break; // Move down
        case 'a': if (*playerX > 0) (*playerX)--; break; // Move left
        case 'd': if (*playerX < WIDTH * 2 - 1) (*playerX)++; break; // Move right
        default: printf("Invalid key: %c\n", directions[i]);
        }
    }
    if (*playerY < HEIGHT && *playerX < WIDTH) playerCurrentRoomID = 0;
    else if (*playerY < HEIGHT && *playerX >= WIDTH) playerCurrentRoomID = 1;
    else if (*playerY >= HEIGHT && *playerX < WIDTH) playerCurrentRoomID = 2;
    else playerCurrentRoomID = 3; // Room 3

    printf("You moved to position (%d, %d).\n", *playerX, *playerY);
}

void look(int roomID) {
    Room *currentRoom = &rooms[roomID];
    printf("\nYou are in: %s\n", currentRoom->name);
    printf("Description: %s\n", currentRoom->description);
    if (currentRoom->item) printf("You see a %s here.\n", currentRoom->item->name);
    if (currentRoom->creature) printf("There is a %s here!\n", currentRoom->creature->name);
}

void loadItems() {
    items[0] = (Item){"knife", 10,   2};
    items[1] = (Item){"gun", 30,  1};
    items[2] = (Item){"kalashnikov", 40, 3};
    items[3] = (Item){"bomb", 60, 3};
    printf("Items loaded into the game world.\n");
}

void loadCreatures() {
    creatures[0] = (Creature){"zombie", 30, 50, 5, 1};
    creatures[1] = (Creature){"orc", 50, 60, 10, 2};
    creatures[2] = (Creature){"vampire", 60, 80, 15, 3};
    creatures[3] = (Creature){"serpent", 80, 100, 20, 0};
    printf("Creatures loaded.\n");
}

void loadRooms() {
    rooms[0] = (Room){"Entrance", 0, "A dark and eerie entrance.", NULL, &rooms[1], NULL, NULL, NULL, &creatures[3]};
    rooms[1] = (Room){"Ancient Library", 1, "A vast, forgotten library filled with dusty bookshelves and the faint smell of. The walls are made of parchment,"
                " adorned with mysterious symbols that glow faintly.", &items[1], &rooms[2], &rooms[0], NULL, NULL, &creatures[0]};
    rooms[2] = (Room){"Hall of Echoes", 2,         "A grand hall with towering pillars and shattered stained glass windows. "
            "Every step you take echoes endlessly, as if something is listening.", &items[0], NULL, &rooms[1], NULL, NULL, &creatures[1]};
    rooms[3] = (Room){"Cavern of Whispers", 3, "A damp and echoing cavern. The sound of distant whispers bounces off the stone walls, "
                                               "as if the very rocks are alive with secrets.", &items[2], NULL, NULL, &rooms[1], NULL, &creatures[2]};
    printf("Rooms loaded into the game.\n");
}


void pickup(Inventory *inventory, int playerCurrentRoomID, const char *itemName) {
    Room *currentRoom = &rooms[playerCurrentRoomID];

    if (currentRoom->item == NULL) {
        printf("No items to pick up in this room.\n");  // No item in the room
        return;
    }

    if (strcmp(currentRoom->item->name, itemName) == 0) {
        if (inventory->itemCount >= MAX_INVENTORY) {
            printf("Inventory is full! You need to drop some items first.\n");
        } else {
            inventory->itemsInInventory[inventory->itemCount++] = currentRoom->item;
            printf("You picked up the %s.\n", currentRoom->item->name);

            currentRoom->item = NULL;
        }
    } else {
        printf("There is no %s here.\n", itemName);
    }
}

void viewInventory(const Inventory *inventory) {
    if (inventory->itemCount == 0) printf("Inventory is empty. Pick up some items.\n");
    else {
        printf("Inventory:\n");
        for (int i = 0; i < inventory->itemCount; i++) {
            printf("- %s (Damage: %d)\n",
                  inventory->itemsInInventory[i]->name,
                  inventory->itemsInInventory[i]->damage
);
        }
    }
}

void drop(Inventory *inventory, int roomID) {
    Room *currentRoom = &rooms[roomID];
    if (inventory->itemCount == 0) {
        printf("Your inventory is empty! There's nothing to drop.\n");
        return;
    }

    printf("Which item would you like to drop? Choose a number:\n");
    for (int i = 0; i < inventory->itemCount; i++) {
        printf("%d. %s (Damage: %d)\n", i + 1,
               inventory->itemsInInventory[i]->name,
               inventory->itemsInInventory[i]->damage
        );
    }

    int choice;
    printf("Enter the number of the item to drop: ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > inventory->itemCount) {
        printf("Invalid choice! Please choose a valid item number.\n");
        return;
    }

    choice--;
    Item *itemToDrop = inventory->itemsInInventory[choice];

    if (currentRoom->item != NULL) {
        printf("There's already an item here, and it has been replaced.\n");
    }

    currentRoom->item = itemToDrop;

    for (int i = choice; i < inventory->itemCount - 1; i++) {
        inventory->itemsInInventory[i] = inventory->itemsInInventory[i + 1];
    }
    inventory->itemsInInventory[inventory->itemCount - 1] = NULL;
    inventory->itemCount--;

    printf("You dropped the %s.\n", itemToDrop->name);
}

void attack(Player *player, const Inventory *inventory, Creature *creature, const char *creatureName) {
    if (creature == NULL || creature->health <= 0) {
        printf("%s doesn't exist or has already been defeated.\n", creatureName);
        return;
    }

    if (strcmp(creature->name, creatureName) != 0) {
        printf("There is no creature named '%s' in this room.\n", creatureName);
        return;
    }

    int hasWeapon = 0;
    int weaponDamage = 0;
    for (int i = 0; i < inventory->itemCount; i++) {
        if (inventory->itemsInInventory[i] != NULL && inventory->itemsInInventory[i]->damage > 0) {
            hasWeapon = 1;
            if (inventory->itemsInInventory[i]->damage > weaponDamage) {
                weaponDamage = inventory->itemsInInventory[i]->damage;
            }
        }
    }

    if (!hasWeapon) {
        printf("You don't have any weapons to attack with. You need to pick up a weapon first.\n");
        return;
    }

    printf("You attack the %s with your %s for %d damage!\n", creature->name, inventory->itemsInInventory[0]->name, weaponDamage);
    creature->health -= weaponDamage;

    if (creature->health <= 0) {
        printf("You defeated the %s!\n", creature->name);
        creature->health = 0;
        return;
    } else {
        printf("The %s has %d health left.\n", creature->name, creature->health);
    }

    printf("The %s attacks you for %d damage!\n", creature->name, creature->damage);
    player->health -= creature->damage;

    if (player->health <= 0) {
        printf("You were defeated by the %s! Game over.\n", creature->name);
        exit(0);
    } else {
        printf("You have %d health remaining.\n", player->health);
    }
}

void save(const char *filepath, const Player *player, const Inventory *inventory) {
    FILE *file = fopen(filepath, "wb");
    if (file == NULL) {
        printf("Failed to open file for saving.\n");
        return;
    }
    printf("Saving to file %s...\n", filepath);

    printf("Saving player data: health=%d, maxHealth=%d, roomID=%d\n", player->health, player->maxHealth, player->currentRoomID);
    fwrite(&player->health, sizeof(player->health), 1, file);
    fwrite(&player->maxHealth, sizeof(player->maxHealth), 1, file);
    fwrite(&player->currentRoomID, sizeof(player->currentRoomID), 1, file);

    printf("Saving inventory data: itemCount=%d\n", inventory->itemCount);
    fwrite(&inventory->itemCount, sizeof(inventory->itemCount), 1, file);
    for (int i = 0; i < inventory->itemCount; i++) {
        Item *item = inventory->itemsInInventory[i];
        if (item != NULL) {
            printf("Saving item: name=%s, damage=%d, roomID=%d\n", item->name, item->damage, item->currentRoomID);
            fwrite(item->name, sizeof(char), strlen(item->name) + 1, file);
            fwrite(&item->damage, sizeof(item->damage), 1, file);
            fwrite(&item->currentRoomID, sizeof(item->currentRoomID), 1, file);
        }
    }
    fclose(file);
    printf("Game saved to %s.\n", filepath);
}


void load(const char *filepath, Player *player, Inventory *inventory) {
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        printf("Failed to open file for loading.\n");
        return;
    }
    fread(&player->health, sizeof(player->health), 1, file);
    fread(&player->maxHealth, sizeof(player->maxHealth), 1, file);
    fread(&player->currentRoomID, sizeof(player->currentRoomID), 1, file);
    fread(&inventory->itemCount, sizeof(inventory->itemCount), 1, file);

    for (int i = 0; i < inventory->itemCount; i++) {
        char itemName[100];
        int damage, currentRoomID;

        fread(itemName, sizeof(char), sizeof(itemName), file);
        itemName[99] = '\0';
        fread(&damage, sizeof(damage), 1, file);
        fread(&currentRoomID, sizeof(currentRoomID), 1, file);

        Item *newItem = (Item *)malloc(sizeof(Item));
        if (newItem == NULL) {
            printf("Memory allocation for new item failed!\n");
            fclose(file);
            return;
        }

        newItem->name = strdup(itemName);
        newItem->damage = damage;
        newItem->currentRoomID = currentRoomID;

        inventory->itemsInInventory[i] = newItem;
    }
    fclose(file);
    printf("Game loaded from %s.\n", filepath);
}
