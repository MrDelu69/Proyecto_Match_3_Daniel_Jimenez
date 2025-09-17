#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

class Gem
{
public:
    enum class Type { RED, BLUE, GREEN, YELLOW, PURPLE, COUNT };

    Gem();
    Gem(Type type, const sf::Vector2f& position);

    void setPosition(const sf::Vector2f& position);
    void setType(Type type);
    Type getType() const;
    sf::Sprite getSprite() const;
    void draw(sf::RenderWindow& window) const;
    bool contains(const sf::Vector2f& point) const;

private:
    Type type;
    sf::Sprite sprite;
    static sf::Texture textures[static_cast<int>(Type::COUNT)];
    static bool texturesLoaded;

    static void loadTextures();
    static string getTexturePath(Type type);
};