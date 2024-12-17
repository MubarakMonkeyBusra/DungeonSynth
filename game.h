#ifndef HEADER_H
#define HEADER_H

#define WIDTH 8      // width of the map
#define HEIGHT 4     // height of the map
#define MAX_INVENTORY 2 // Maximum items a player can carry

typedef struct {
    char *name;      // The name of the item
    int damage;      // The damage value
    int currentRoomID; // room in which item is located
} Item;

typedef struct {
    char *name;
    int maxHealth;
    int health;
    int currentRoomID;
} Player;

typedef struct {
    char *name;
    int maxHealth;
    int health;
    int damage;
    int currentRoomID;
} Creature;

typedef struct Room {
    char *name;
    int id;
    char *description;
    Item *item;
    struct Room *up, *down, *left, *right;
    Creature *creature;   /
} Room;


typedef struct {
    Item *itemsInInventory[MAX_INVENTORY];
    int itemCount;
} Inventory;

extern int playerCurrentRoomID;
extern Room rooms[4];
extern Creature creatures[4];
extern Item items[4];

void printMap(int playerX, int playerY);
void movePlayer(char *directions, int *playerX, int *playerY); /
void loadItems();
void loadCreatures();
void loadRooms();
void look(int roomID);
void viewInventory(const Inventory *inventory);
void pickup(Inventory *inventory, int playerCurrentRoomID, const char *itemName);
void attack(Player *player, const Inventory *inventory, Creature *creature, const char *creatureName);
void drop(Inventory *inventory, int roomID);

void save(const char *filepath, const Player *player, const Inventory *inventory);
void load(const char *filepath, Player *player, Inventory *inventory);


#endif

