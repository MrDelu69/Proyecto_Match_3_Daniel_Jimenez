#include "Gem.h"

sf::Texture Gem::textures[static_cast<int>(Gem::Type::COUNT)];
bool Gem::texturesLoaded = false;

Gem::Gem() : type(Type::RED) {}

Gem::Gem(Type type, const sf::Vector2f& position) : type(type)
{
    if (!texturesLoaded)
    {
        loadTextures();
        texturesLoaded = true;
    }

    sprite.setTexture(textures[static_cast<int>(type)]);
    sprite.setPosition(position);
   
}

void Gem::setPosition(const sf::Vector2f& position)
{
    sprite.setPosition(position);
}

void Gem::setType(Type type)
{
    this->type = type;
    sprite.setTexture(textures[static_cast<int>(type)]);
}

Gem::Type Gem::getType() const
{
    return type;
}

sf::Sprite Gem::getSprite() const
{
    return sprite;
}

void Gem::draw(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

bool Gem::contains(const sf::Vector2f& point) const
{
    return sprite.getGlobalBounds().contains(point);
}

void Gem::loadTextures()
{
    for (int i = 0; i < static_cast<int>(Type::COUNT); ++i)
    {
        Type currentType = static_cast<Type>(i);
        string path = getTexturePath(currentType);

        if (!textures[i].loadFromFile(path))
        {
            cout << "Error cargando textura: " << path << endl;
        }
    }
}

string Gem::getTexturePath(Type type)
{
    switch (type)
    {
    case Type::RED: return "assets/red_gem.png";
    case Type::BLUE: return "assets/blue_gem.png";
    case Type::GREEN: return "assets/green_gem.png";
    case Type::YELLOW: return "assets/yellow_gem.png";
    case Type::PURPLE: return "assets/purple_gem.png";
    default: return "";
    }
}