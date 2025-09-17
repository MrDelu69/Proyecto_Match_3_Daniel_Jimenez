#pragma once
#include "Gem.h"
#include <vector>
using namespace std;

class Board
{
public:
    static const int BOARD_SIZE = 8;
    static const int GEM_SIZE = 55; // Aumentar el tamaño de las gemas

    Board();

    void initialize();
    void draw(sf::RenderWindow& window) const;
    bool isValidAdjacentMove(int row1, int col1, int row2, int col2) const;
    void performSwap(int row1, int col1, int row2, int col2);
    bool checkHasMatches() const;
    int removeMatchesAndCalculateScore();
   
    void refillBoard();
    Gem* getGemAtPosition(const sf::Vector2f& position);
    void getGemCoordinates(const sf::Vector2f& position, int& row, int& col) const;
    bool hasValidMoves() const;

    // void applyGravity(); // descomentar para hacer la animacion de gravedad



private:
    Gem* board[BOARD_SIZE][BOARD_SIZE];
    sf::Vector2f boardOffset; // Desplazamiento para centrar el tablero

    void generateRandomBoard();
    Gem::Type getRandomGemType() const;
    bool checkHorizontalMatch(int row, int col) const;
    bool checkVerticalMatch(int row, int col) const;
};