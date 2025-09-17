#include "Board.h"
#include <random>
Board::Board() : boardOffset(100, 50) //Con el board offset se centra el tablero
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            board[i][j] = nullptr;
        }
    }
}

void Board::initialize()
{
	cout << "inicializando tablero" << endl;
    generateRandomBoard();
    while (checkHasMatches())
    {
        removeMatchesAndCalculateScore();
       // applyGravity();
        refillBoard();
    }
}

void Board::draw(sf::RenderWindow& window) const
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board[i][j])
            {
                board[i][j]->draw(window);
            }
        }
    }
}

bool Board::isValidAdjacentMove(int row1, int col1, int row2, int col2) const
{
	// revisa si son adyacentes
    bool isAdjacent = (abs(row1 - row2) == 1 && col1 == col2) ||
        (abs(col1 - col2) == 1 && row1 == row2);

	// revisa si estan dentro de los limites del tablero
    bool withinBounds = row1 >= 0 && row1 < BOARD_SIZE &&
        row2 >= 0 && row2 < BOARD_SIZE &&
        col1 >= 0 && col1 < BOARD_SIZE &&
        col2 >= 0 && col2 < BOARD_SIZE;

    return isAdjacent && withinBounds;
}

void Board::performSwap(int row1, int col1, int row2, int col2)
{
    if (isValidAdjacentMove(row1, col1, row2, col2))
    {
        swap(board[row1][col1], board[row2][col2]);

		// actualiza posiciones de las gemas
        if (board[row1][col1])
        {
            board[row1][col1]->setPosition(boardOffset + sf::Vector2f(col1 * GEM_SIZE, row1 * GEM_SIZE));
        }
        if (board[row2][col2])
        {
            board[row2][col2]->setPosition(boardOffset + sf::Vector2f(col2 * GEM_SIZE, row2 * GEM_SIZE));
        }
    }
}

bool Board::checkHasMatches() const
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (checkHorizontalMatch(i, j) || checkVerticalMatch(i, j))
            {
                return true;
            }
        }
    }
    return false;
}

int Board::removeMatchesAndCalculateScore()
{
    vector<pair<int, int>> matches;
    int score = 0;

	// buscar matches horizontales
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE - 2; ++j)
        {
            if (checkHorizontalMatch(i, j))
            {
                matches.push_back({ i, j });
                matches.push_back({ i, j + 1 });
                matches.push_back({ i, j + 2 });
            }
        }
    }

    // busca matches verticales
    for (int i = 0; i < BOARD_SIZE - 2; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (checkVerticalMatch(i, j))
            {
                matches.push_back({ i, j });
                matches.push_back({ i + 1, j });
                matches.push_back({ i + 2, j });
            }
        }
    }

    // Eliminar duplicados
    sort(matches.begin(), matches.end());
    matches.erase(unique(matches.begin(), matches.end()), matches.end());

	// elimina gemas y calcula la puntuacion
    for (const auto& match : matches)
    {
        int row = match.first;
        int col = match.second;

        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col])
        {
            delete board[row][col];
            board[row][col] = nullptr;
            score += 10;
        }
    }

    return score;
}



void Board::applyGravity()
{
    for (int col = 0; col < BOARD_SIZE; ++col)
    {
        int emptySpaces = 0;
        for (int row = BOARD_SIZE - 1; row >= 0; --row)
        {
            if (board[row][col] == nullptr)
            {
                emptySpaces++;
            }
            else if (emptySpaces > 0)
            {
                int newRow = row + emptySpaces;
                board[newRow][col] = board[row][col];
                board[row][col] = nullptr;
                board[newRow][col]->setPosition(boardOffset + sf::Vector2f(col * GEM_SIZE, newRow * GEM_SIZE));
            }
        }
    }
}



void Board::refillBoard() // llena los espacios vacios con nuevas gemas
{
    for (int col = 0; col < BOARD_SIZE; ++col)
    {
        for (int row = 0; row < BOARD_SIZE; ++row)
        {
            if (board[row][col] == nullptr)
            {
                Gem::Type newType = getRandomGemType();
                board[row][col] = new Gem(newType, boardOffset + sf::Vector2f(col * GEM_SIZE, row * GEM_SIZE));
            }
        }
    }
}

Gem* Board::getGemAtPosition(const sf::Vector2f& position) // obtiene la gema en una posicion dada
{
    int row, col;
    getGemCoordinates(position, row, col);

    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
    {
        return board[row][col];
    }
    return nullptr;
}

void Board::getGemCoordinates(const sf::Vector2f& position, int& row, int& col) const
{
    col = (static_cast<int>(position.x) - boardOffset.x) / GEM_SIZE;
    row = (static_cast<int>(position.y) - boardOffset.y) / GEM_SIZE;
}

bool Board::hasValidMoves() const
{
    return true;
}

void Board::generateRandomBoard()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, static_cast<int>(Gem::Type::COUNT) - 1);

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            Gem::Type randomType = static_cast<Gem::Type>(dis(gen));
            board[i][j] = new Gem(randomType, boardOffset + sf::Vector2f(j * GEM_SIZE, i * GEM_SIZE));
        }
    }
}

Gem::Type Board::getRandomGemType() const
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, static_cast<int>(Gem::Type::COUNT) - 1);

    return static_cast<Gem::Type>(dis(gen));
}

bool Board::checkHorizontalMatch(int row, int col) const
{
    if (col > BOARD_SIZE - 3) return false;

    if (!board[row][col] || !board[row][col + 1] || !board[row][col + 2]) return false;

    Gem::Type type = board[row][col]->getType();
    return board[row][col + 1]->getType() == type &&
        board[row][col + 2]->getType() == type;
}

bool Board::checkVerticalMatch(int row, int col) const
{
    if (row > BOARD_SIZE - 3) return false;

    if (!board[row][col] || !board[row + 1][col] || !board[row + 2][col]) return false;

    Gem::Type type = board[row][col]->getType();
    return board[row + 1][col]->getType() == type &&
        board[row + 2][col]->getType() == type;
}