#include <game.h>

#include <raylib.h>
#include <algorithm>

#include "picojson.h"
#include <fstream>
#include <sstream>
#include <iostream>


GameWindow::GameWindow() noexcept {
    frameRate = 60;
    width = DEFAULT_WINDOW_WIDTH;
    height = DEFAULT_WINDOW_HEIGHT;
    title = WINDOW_TITLE;
    
    InitWindow(width, height, title);
    SetTargetFPS(frameRate);
}

GameWindow::~GameWindow() noexcept {
    CloseWindow();
}


Scene SceneLoader::loadScene(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open scene file: " << filePath << std::endl;
        return Scene();  // Return an empty scene on failure
    }
    std::cerr << "Loading scene file... " << filePath << std::endl;

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    picojson::value v;
    std::string err = picojson::parse(v, buffer.str());
    if (!err.empty()) {
        std::cerr << "JSON parse error: " << err << std::endl;
        return Scene();  // Return an empty scene on parse error
    }

    Scene scene;

    // Parse the scene object
    picojson::object& sceneObj = v.get<picojson::object>();

    // Parse entities
    picojson::array& entities = sceneObj["entities"].get<picojson::array>();
    for (picojson::array::iterator it = entities.begin(); it != entities.end(); ++it) {
        picojson::object& entityObj = it->get<picojson::object>();
        std::string type = entityObj["type"].get<std::string>();

        if (type == "TextEntity") {
            std::cerr << "Loading TextEntity..." << std::endl;
            std::string text = entityObj["text"].get<std::string>();
            picojson::object& positionObj = entityObj["position"].get<picojson::object>();
            EVec position = { float(positionObj["x"].get<double>()), float(positionObj["y"].get<double>()) };

            picojson::object& colorObj = entityObj["color"].get<picojson::object>();
            Color color = { uint8_t(colorObj["r"].get<double>()), uint8_t(colorObj["g"].get<double>()),
                                  uint8_t(colorObj["b"].get<double>()), uint8_t(colorObj["a"].get<double>()) };

            auto entity = std::make_unique<TextEntity>(text, position, Color{color.r, color.g, color.b, color.a});
            
            std::cerr << "Added entity: " << text  << std::endl;
            scene.addEntity(std::move(entity));
        } else {
            std::cerr << "Could not locate type: '" << type << "'" << std::endl;
        }
        // Add more entity types as needed
    }

    return scene;
}

void SceneLoader::saveScene(const Scene& scene, const std::string& filePath) {
    picojson::object sceneObj;

    // Serialize entities
    picojson::array entitiesArray;
    for (const auto& entity : scene.getAllEntities()) {
        picojson::object entityObj;
        if (auto textEntity = dynamic_cast<TextEntity*>(entity.get())) {
            entityObj["type"] = picojson::value("TextEntity");
            entityObj["text"] = picojson::value(textEntity->getText());

            picojson::object positionObj;
            positionObj["x"] = picojson::value(static_cast<double>(textEntity->getPosition().x));
            positionObj["y"] = picojson::value(static_cast<double>(textEntity->getPosition().y));
            entityObj["position"] = picojson::value(positionObj);

            picojson::object colorObj;
            colorObj["r"] = picojson::value(static_cast<double>(textEntity->getColor().r));
            colorObj["g"] = picojson::value(static_cast<double>(textEntity->getColor().g));
            colorObj["b"] = picojson::value(static_cast<double>(textEntity->getColor().b));
            colorObj["a"] = picojson::value(static_cast<double>(textEntity->getColor().a));
            entityObj["color"] = picojson::value(colorObj);
        }
        // Add more entity types as needed

        entitiesArray.push_back(picojson::value(entityObj));
    }

    sceneObj["entities"] = picojson::value(entitiesArray);

    // Write JSON to file
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open scene file for writing: " << filePath << std::endl;
        return;
    }

    picojson::value v(sceneObj);
    file << v.serialize(true);  // Pretty-print with indent
    file.close();
}


PlayerEntityObject::PlayerEntityObject(EVec position, PlayerColor playerColor, float playerHealth)
    : pos(position), color(playerColor), health(playerHealth) {}

void PlayerEntityObject::Update() {
    // Implement update logic here
}

void PlayerEntityObject::Render() {
    // Implement render logic here
    DrawCircle(pos.x, pos.y, 20, { color.r, color.g, color.b, color.a });
}

Scene::Scene() {
    // Scene constructor implementation
}

void Scene::addEntity(std::unique_ptr<SceneEntity> entity) {
    entities.push_back(std::move(entity));
}

void Scene::removeEntity(const SceneEntity& entity) {
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [&entity](const std::unique_ptr<SceneEntity>& ptr) { return ptr.get() == &entity; }),
        entities.end());
}

const std::vector<std::unique_ptr<SceneEntity>>& Scene::getAllEntities() const {
    return entities;
}

SceneManager::SceneManager() : currentScene() {
    // Explicitly initialize currentScene using Scene's constructor
}

SceneManager::~SceneManager() {
    // Destructor (if needed for cleanup)
}

Scene& SceneManager::getScene() {
    return currentScene;
}


LogLevel GameLogger::getLogLevel() {
    return logLevel;
}

void GameLogger::setLogLevel(LogLevel level) {
    logLevel = level;
}

void GameLogger::Log(std::string msg) {
    Log(logLevel, msg);
}

void GameLogger::Log(LogLevel level, std::string msg) {
    switch (level) {
        case DEBUG: 
            TraceLog(LOG_DEBUG, msg.c_str());
            break;
        case WARNING:
            TraceLog(LOG_WARNING, msg.c_str());
            break;
        case INFO:
            TraceLog(LOG_INFO, msg.c_str());
            break;
        case ERROR:
            TraceLog(LOG_ERROR, msg.c_str());
            break;
        case CRITICAL:
            TraceLog(LOG_FATAL, msg.c_str());
            break;
    }
}


Game::Game() : gameWindow(), gameState(), sceneManager(), playerState(), gameLogger(),
      playerEntity("Player", generateRandomPlayerColor(), EVec{0, 0}, 1.0f, Inventory(9, 3)) {
}

int Game::Start() {
    // Load the scene from the JSON file
    Scene scene = SceneLoader::loadScene("assets/scene/title.json");

    std::cerr << "Scene entity count: " << scene.getAllEntities().size() << std::endl;

    sceneManager.getScene() = std::move(scene);  // Use move assignment here

    while (!WindowShouldClose()) {
        // Update game state
        for (const auto& entity : sceneManager.getScene().getAllEntities()) {
            entity->Update();
        }

        // Begin drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Render game scene
        for (const auto& entity : sceneManager.getScene().getAllEntities()) {
            entity->Render();
        }

        EndDrawing();
    }

    return 0;
}


PlayerEntity& Game::getPlayerEntity() {
    return playerEntity;
}

GameState& Game::getGameState() {
    return gameState;
}

PlayerState& Game::getPlayerState() {
    return playerState;
}

// picojson::value TextEntity::toJson() const {
//     picojson::object obj;
//     obj["type"] = picojson::value("TextEntity");
//     obj["text"] = picojson::value(text);
//     obj["position"] = picojson::value(picojson::object{
//         {"x", picojson::value(position.x)},
//         {"y", picojson::value(position.y)}
//     });
//     obj["color"] = picojson::value(picojson::object{
//         {"r", picojson::value(static_cast<double>(color.r))},
//         {"g", picojson::value(static_cast<double>(color.g))},
//         {"b", picojson::value(static_cast<double>(color.b))},
//         {"a", picojson::value(static_cast<double>(color.a))}
//     });
//     return picojson::value(obj);
// }

std::unique_ptr<TextEntity> TextEntity::fromJson(const picojson::object& obj) {
    std::string text = obj.at("text").get<std::string>();
    const auto& posObj = obj.at("position").get<picojson::object>();
    EVec position = {
        static_cast<float>(posObj.at("x").get<double>()),
        static_cast<float>(posObj.at("y").get<double>())
    };
    const auto& colorObj = obj.at("color").get<picojson::object>();
    Color color = {
        static_cast<uint8_t>(colorObj.at("r").get<double>()),
        static_cast<uint8_t>(colorObj.at("g").get<double>()),
        static_cast<uint8_t>(colorObj.at("b").get<double>()),
        static_cast<uint8_t>(colorObj.at("a").get<double>())
    };
    return std::make_unique<TextEntity>(text, position, color);
}

EVec TextEntity::getPosition() {
    return position;
}

std::string TextEntity::getText() {
    return text;
}

Color TextEntity::getColor() {
    return color;
}

void TextEntity::Update() {

}

void TextEntity::Render() {
    DrawText(text.c_str(), position.x, position.y, 24, color);
}