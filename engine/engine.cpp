#include <engine.h>

#include <random>

PlayerColor generateRandomPlayerColor() {
    std::random_device randomDevice;
    // Use Mersenne Twister engine for generating random numbers
    std::mt19937 gen(randomDevice());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255);

    PlayerColor randomColor;
    randomColor.r = dist(gen);
    randomColor.g = dist(gen);
    randomColor.b = dist(gen);
    randomColor.a = 255; // Player colors can't be transparent.

    return randomColor;
}

std::string PlayerEntity::getName() const {
    return name;
}

const char* PlayerEntity::getNameCStr() const {
    return name.c_str();
}

PlayerColor PlayerEntity::getColor() const {
    return color;
}

void PlayerEntity::setColor(const PlayerColor& c) {
    color = c;
}

EVec PlayerEntity::getPos() const {
    return pos;
}

void PlayerEntity::setPos(const EVec& vec) {
    pos = vec;
}

float PlayerEntity::getHealth() const {
    return health;
}

void PlayerEntity::setHealth(float h) {
    health = h;
}

Inventory PlayerEntity::getInventory() {
    return inventory;
}

void PlayerEntity::move(const EVec& vec) {
    pos = Lerp(pos, vec, 0.1f);  // Smooth out movement using interpolation
}

bool PlayerEntity::takeDamage(float amount) {
    health -= amount;
    if (health <= 0.0f) {
        health = 0.0f;
        return true;
    }
    return false;
}


EVec Lerp(const EVec& start, const EVec& end, float t) {
    EVec result;
    result.x = start.x + t * (end.x - start.x);
    result.y = start.y + t * (end.y - start.y);
    return result;
}


