#pragma once

#include <raylib.h>
#include <engine.h>
#include <picojson.h>

#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080
#define WINDOW_TITLE "Multiplayer Networking"


class SceneEntity {
public:
    virtual void Update() = 0;
    virtual void Render() = 0;
    
    virtual ~SceneEntity() = default;
};

class PlayerEntityObject : public SceneEntity {
private:
    EVec pos;
    PlayerColor color;
    float health;

public:
    PlayerEntityObject(EVec position, PlayerColor playerColor, float playerHealth);

    void Update() override;
    void Render() override;
};

class Scene {
private:
    std::vector<std::unique_ptr<SceneEntity>> entities;

public:
    Scene();
    void addEntity(std::unique_ptr<SceneEntity> entity);
    void removeEntity(const SceneEntity& entity);
    const std::vector<std::unique_ptr<SceneEntity>>& getAllEntities() const;
};

class SceneManager {
private:
    Scene currentScene;

public:
    SceneManager();
    ~SceneManager();
    Scene& getScene();
};

class SceneLoader {
public:
    static Scene loadScene(const std::string& filePath);
    static void saveScene(const Scene& scene, const std::string& filePath);
};

/// @brief Represents the game window's properties.
class GameWindow {
private:
    int frameRate;      // The frame rate of the game window.
    int width;          // The width of the game window.
    int height;         // The height of the game window.
    const char* title;  // The title of the game window.

public:
    int getFrameRate() const;

    void setFrameRate(int frameRate);

    int getWidth() const;

    void setWidth(int width);

    int getHeight() const;

    void setHeight(int height);

    const char* getTitle() const;

    void setTitle(const char* title);

    GameWindow() noexcept;
    ~GameWindow() noexcept;
};

/// @brief Defines various log levels for game logging.
typedef enum {
    DEBUG = 0,       // Debugging information.
    WARNING = 10,    // Warnings about potential issues.
    INFO = 20,       // Informational messages.
    ERROR = 30,      // Error messages.
    CRITICAL = 40,   // Critical issues that may halt the game.
} LogLevel;

/// @brief Provides logging functionalities for the game.
class GameLogger {
private:
    LogLevel logLevel; ///< The current logging level.

public:
    /// @brief Gets the current logging level.
    /// @return The current LogLevel.
    LogLevel getLogLevel();

    /// @brief Sets the logging level.
    /// @param level The LogLevel to set.
    void setLogLevel(LogLevel level);

    /// @brief Logs a message with the current logging level.
    /// @param msg The message to log.
    void Log(std::string msg);

    /// @brief Logs a message with a specific logging level.
    /// @param level The LogLevel for the message.
    /// @param msg The message to log.
    void Log(LogLevel level, std::string msg);
};

/// @brief Represents the main game class that manages game state, player, and other components.
class Game {
private:
    GameWindow gameWindow;      // The game window properties.
    PlayerEntity playerEntity;  // The player entity.
    GameState gameState;        // The current state of the game.
    SceneManager sceneManager;  // Manages scenes in the game.
    PlayerState playerState;    // The current state of the player.
    GameLogger gameLogger;      // Handles game logging.

public:
    /// @brief Starts the game loop.
    /// @return An integer representing the exit status of the game.
    int Start();

    /// @brief Gets the player entity.
    /// @return A reference to the PlayerEntity.
    PlayerEntity& getPlayerEntity();

    /// @brief Gets the current game state.
    /// @return A reference to the GameState.
    GameState& getGameState();

    /// @brief Gets the current player state.
    /// @return A reference to the PlayerState.
    PlayerState& getPlayerState();

    /// @brief Gets the singleton instance of the Game.
    /// @return A reference to the singleton Game instance.
    inline static Game& getInstance() {
        static Game instance;
        return instance;
    }

    /// @brief Deleted copy constructor to prevent copying.
    Game(Game const&) = delete;

    /// @brief Deleted assignment operator to prevent assignment.
    void operator=(Game const&) = delete;

private:
    /// @brief Private constructor for singleton pattern.
    Game();
};


class TextEntity : public SceneEntity {
private:
    std::string text;
    EVec position;
    Color color;

public:
    TextEntity(const std::string& text, const EVec& position, const Color& color)
        : text(text), position(position), color(color){}

    void Update() override;

    void Render() override;

    EVec getPosition();
    std::string getText();
    Color getColor();

    static std::unique_ptr<TextEntity> fromJson(const picojson::object& obj);
};