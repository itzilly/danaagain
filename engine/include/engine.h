#pragma once

#include <string>
#include <vector>

/// @brief Represent the state of the server.
typedef enum {
    STARTING = 0,   // The server is starting up. No client connections are allowed.
    OPEN = 1,       // The server is open and accepting client connections.
    CLOSING = 2     // The server is closing and therefor disconnecting all clients.
} ServerState;

/// @brief Represents the state of the game.
typedef enum {
    MENU = 0,        // The game is in the menu state.
    CONNECTING = 1,  // The game is trying to connect to a server.
    PLAYING = 2,     // The game is currently being played.
} GameState;

/// @brief Represents the state of a player.
typedef enum {
    SYNCING = 0,         // The player is syncing data with the server.
    LOADING = 1,         // The player is loading into the world.
    NORMAL = 2,          // The player is actively engaged in gameplay, without any menus or overlays open.
    CHAT = 3,            // The player has the chat opened.
    INVENTORY = 4,       // The player has their inventory open.
    CRAFTING = 5,        // The player is in a crafting gui.
    OPEN_CONTAINER = 6,  // The player is viewing the contents of a container.
} PlayerState;

/// @brief Represents a player's color.
/// @note This is designed to be able to be switched
/// out for raylib's color type in the future.
typedef struct {
    unsigned char r;  // Red component (0-255)
    unsigned char g;  // Green component (0-255)
    unsigned char b;  // Blue component (0-255)
    unsigned char a;  // Alpha component (0-255, transparency)
} PlayerColor;

/// @brief Represents color
/// @note This is designed to be able to be switched
/// out for raylib's color type in the future.
typedef struct {
    unsigned char r;  // Red component (0-255)
    unsigned char g;  // Green component (0-255)
    unsigned char b;  // Blue component (0-255)
    unsigned char a;  // Alpha component (0-255, transparency)
} ColorR;

/// @brief Generates a random color.
/// @note The a value will always be 0 since player's can't be transparent.
/// @return A PlayerColor with random r, g, and b values.
PlayerColor generateRandomPlayerColor();

/// @brief Represents a 2D vector.
/// This is designed to be a 'wrapper' around 
/// raylib's vector type since including raylib
/// and networking together in the same file doesn't work.
typedef struct {
    float x;  // x coordinate of the vector
    float y;  // y coordinate of the vector
} EVec;

/// @brief Represents an inventory item.
/// @warning This will be deprecated in future
/// versions in favor of a class/object instead
/// of a struct.
typedef struct {
    std::string name;      // The name of the item.
    int maxStackSize;      // The maximum number of items that can be stacked.
    int amount;            // The current amount of this item in the stack.
} InventoryItem_s;

/// @brief Represents the inventory of a player or entity or container.
class Inventory {
public:
    /// @brief A vector containing all items in the inventory.
    std::vector<InventoryItem_s> items;

    /// @brief The number of rows in the inventory grid.
    int rows;

    /// @brief The number of columns in the inventory grid.
    int cols;

    /// @brief Constructs an Inventory with a specified number of rows and columns.
    /// @param r The number of rows in the inventory.
    /// @param c The number of columns in the inventory.
    Inventory(int r, int c) : rows(r), cols(c), items() {}
};

/// @brief Represents a player entity.
class PlayerEntity {
private:
    /// @brief The name of the player.
    std::string name;

    /// @brief The color representing the player.
    PlayerColor color;

    /// @brief The current position of the player in the game world.
    EVec pos;

    /// @brief The current health of the player.
    float health;

    /// @brief The player's inventory.
    Inventory inventory;

public:
    /// @brief Gets the player's name.
    /// @return The name of the player as a std::string.
    std::string getName() const;

    /// @brief Gets the player's name as a C-style string.
    /// @return The name of the player as a const char*.
    const char* getNameCStr() const;

    /// @brief Sets the player's color.
    /// @param color The color to set for the player.
    void setColor(const PlayerColor& color);

    /// @brief Gets the player's color.
    /// @return The player color as a PlayerColor (not Raylib Color)
    PlayerColor getColor() const;

    /// @brief Gets the world position of the player.
    /// @return The player's position as an EVec.
    EVec getPos() const;

    /// @brief Sets the player's position.
    /// @param vec The new position for the player.
    void setPos(const EVec& vec);

    /// @brief Gets the player's current health.
    /// @return The current health of the player as a float.
    float getHealth() const;

    /// @brief Sets the player's health.
    /// @param health The new health value for the player.
    void setHealth(float health);

    /// @brief Gets the player's inventory.
    /// @return The current inventory of the player as Inventory.
    Inventory getInventory();

    /// @brief Moves the player by a given vector.
    /// @param vec The vector by which to move the player.
    void move(const EVec& vec);
    
    /// @brief Damages the player by a given amount.
    /// @param amount The amount to reduce the player's health by.
    /// @return Returns true if the player dies after taking the damage, false otherwise.
    bool takeDamage(float amount);
    
    /// @brief Constructs a PlayerEntity.
    /// @param name The name of the player.
    /// @param color The color of the player.
    /// @param pos The world position of the player.
    /// @param health The health of the player.
    PlayerEntity(std::string name, PlayerColor color, EVec pos, float health, Inventory inventory) : name(name), color(color), pos(pos), health(health), inventory(std::move(inventory)) {};
};

EVec Lerp(const EVec& start, const EVec& end, float t);

//class Client {
//private:
//    Client() {}
//
//    PlayerEntity playerEntity;
//
//public:
//    PlayerEntity getPlayerEntity();
//};


