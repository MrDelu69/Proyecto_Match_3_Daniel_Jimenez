#pragma once
#include "Board.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    sf::RenderWindow window;
    Board board;
    sf::Font font;

    int score;
    int movesLeft;
    bool gameStarted;
    bool gameOver;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture startScreenTexture;
    sf::Sprite startScreenSprite;
    sf::Texture endScreenTexture;
    sf::Sprite endScreenSprite;

    sf::Vector2i firstSelected;
    sf::Vector2i secondSelected;

    void processEvents();
    void update();
    void render();
    void initialize();
    void handleMouseClick(const sf::Vector2f& position);
    void resetGame();
    void drawUI();
    void drawStartScreen();
    void drawGameOverScreen();
    void loadFont();
    void loadTextures();
    bool isPlayButtonClicked(const sf::Vector2f& position);
    bool isRestartButtonClicked(const sf::Vector2f& position);
    bool isExitButtonClicked(const sf::Vector2f& position);
    void handleGemSelection(const sf::Vector2f& position);
    void processMatches();
};