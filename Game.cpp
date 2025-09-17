#include "Game.h"

Game::Game() : window(sf::VideoMode(800, 600), "Gemas Encantadas"),
score(0), movesLeft(20), gameStarted(false), gameOver(false),
firstSelected(-1, -1), secondSelected(-1, -1)
{
    window.setFramerateLimit(60);

    loadFont();
    loadTextures();
    initialize();

	cout << "Juego iniciado" << endl;
}

Game::~Game()
{
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                handleMouseClick(mousePos);
            }
        }
    }
}

void Game::update()
{
    if (gameStarted && !gameOver)
    {
        if (movesLeft <= 0)
        {
            gameOver = true;
        }
    }
}

void Game::render()
{
    window.clear();

    if (!gameStarted)
    {
        window.draw(startScreenSprite);
        drawStartScreen();
    }
    else if (gameOver)
    {
        window.draw(endScreenSprite);
        drawGameOverScreen();
    }
    else
    {
        window.draw(backgroundSprite);
        board.draw(window);
        drawUI();
    }

    window.display();
}

void Game::initialize()
{
    board.initialize();
}

void Game::handleMouseClick(const sf::Vector2f& position)
{
    if (!gameStarted)
    {
        if (isPlayButtonClicked(position))
        {
            gameStarted = true;
			cout << "Iniciando juego" << endl;
        }
        return;
    }

    if (gameOver)
    {
        if (isRestartButtonClicked(position))
        {
            resetGame();
			cout << "Reiniciando juego" << endl;
        }
        else if (isExitButtonClicked(position))
        {
            window.close();
			cout << "Saliendo del juego" << endl;
        }
        return;
    }

    handleGemSelection(position);
}

void Game::resetGame()
{
    score = 0;
    movesLeft = 20;
    gameOver = false;
    firstSelected = sf::Vector2i(-1, -1);
    secondSelected = sf::Vector2i(-1, -1);
    board.initialize();
}

void Game::drawUI()
{
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Puntuacion: " + to_string(score));
    scoreText.setCharacterSize(15); // Reducir el tamaño del texto
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(550, 50);
    window.draw(scoreText);

    sf::Text movesText;
    movesText.setFont(font);
    movesText.setString("Movimientos: " + to_string(movesLeft));
    movesText.setCharacterSize(15);
    movesText.setFillColor(sf::Color::White);
    movesText.setPosition(550, 100);
    window.draw(movesText);
}

void Game::drawStartScreen()
{
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("GEMAS ENCANTADAS");
    titleText.setCharacterSize(40); // Reducir el tamaño del título
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(100, 200);
    titleText.setStyle(sf::Text::Bold);
    window.draw(titleText);

    // Dibujar botón de jugar
    sf::RectangleShape playButton(sf::Vector2f(200, 50));
    playButton.setPosition(300, 300);
    playButton.setFillColor(sf::Color::Black);
    window.draw(playButton);

    sf::Text playText;
    playText.setFont(font);
    playText.setString("JUGAR");
    playText.setCharacterSize(15);
    playText.setFillColor(sf::Color::White);
    playText.setPosition(340, 305);
    window.draw(playText);
}

void Game::drawGameOverScreen()
{
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("JUEGO TERMINADO");
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(100, 200);
    window.draw(gameOverText);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Puntuacion Final: " + to_string(score));
    scoreText.setCharacterSize(15);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(250, 250);
    window.draw(scoreText);

    // Botón Reiniciar
    sf::RectangleShape restartButton(sf::Vector2f(150, 50));
    restartButton.setPosition(200, 350);
    restartButton.setFillColor(sf::Color::Blue);
    window.draw(restartButton);

    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("Reiniciar");
    restartText.setCharacterSize(15);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(210, 360);
    window.draw(restartText);

    // Botón Salir
    sf::RectangleShape exitButton(sf::Vector2f(150, 50));
    exitButton.setPosition(450, 350);
    exitButton.setFillColor(sf::Color::Red);
    window.draw(exitButton);

    sf::Text exitText;
    exitText.setFont(font);
    exitText.setString("Salir");
    exitText.setCharacterSize(15);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(500, 360);
    window.draw(exitText);
}

void Game::loadFont()
{
    if (!font.loadFromFile("assets/font.ttf"))
    {
        cout << "Error cargando fuente" << endl;
    }
}

void Game::loadTextures()
{
    if (!backgroundTexture.loadFromFile("assets/background.png"))
    {
        cout << "ERROR: No se pudo cargar background.png" << endl;
        backgroundTexture.create(800, 600);
    }
    backgroundSprite.setTexture(backgroundTexture);

    if (!startScreenTexture.loadFromFile("assets/start_screen.png"))
    {
        cout << "ERROR: No se pudo cargar start_screen.png" << endl;
        startScreenTexture.create(800, 600);
    }
    startScreenSprite.setTexture(startScreenTexture);

    if (!endScreenTexture.loadFromFile("assets/end_screen.png"))
    {
        cout << "ERROR: No se pudo cargar end_screen.png" << endl;
        endScreenTexture.create(800, 600);
    }
    endScreenSprite.setTexture(endScreenTexture);
}

bool Game::isPlayButtonClicked(const sf::Vector2f& position)
{
    return position.x >= 300 && position.x <= 500 &&
        position.y >= 300 && position.y <= 350;
}

bool Game::isRestartButtonClicked(const sf::Vector2f& position)
{
    return position.x >= 200 && position.x <= 350 &&
        position.y >= 350 && position.y <= 400;
}

bool Game::isExitButtonClicked(const sf::Vector2f& position)
{
    return position.x >= 450 && position.x <= 600 &&
        position.y >= 350 && position.y <= 400;
}

void Game::handleGemSelection(const sf::Vector2f& position)
{
    int row, col;
    board.getGemCoordinates(position, row, col);

    if (row >= 0 && row < Board::BOARD_SIZE && col >= 0 && col < Board::BOARD_SIZE)
    {
        if (firstSelected.x == -1)
        {
            firstSelected = sf::Vector2i(row, col);
        }
        else
        {
            secondSelected = sf::Vector2i(row, col);

            // verifica si el movimiento es valido 
            if (board.isValidAdjacentMove(firstSelected.x, firstSelected.y, secondSelected.x, secondSelected.y))
            {
				// si lo es entonces realiza el intercambio
                board.performSwap(firstSelected.x, firstSelected.y, secondSelected.x, secondSelected.y);
                cout << "Movimiento valido" << endl;
				// llama a checkHasMatches para ver si el intercambio crea un match
                if (board.checkHasMatches())
                {
					// reduce el numero de movimientos y cuenta el match
                    movesLeft--;
                    processMatches();
                }
                else
                {
                    // movimiento invalido 
                    board.performSwap(firstSelected.x, firstSelected.y, secondSelected.x, secondSelected.y);
                    cout << "Movimiento invalido" << endl;
                }
            }

            // reinicia la selección
            firstSelected = sf::Vector2i(-1, -1);
            secondSelected = sf::Vector2i(-1, -1);
        }
    }
}

void Game::processMatches()
{
    int matchScore = board.removeMatchesAndCalculateScore();
    score += matchScore;

   // board.applyGravity();
    board.refillBoard();

    // verifica si hay mas matches despues de rellenar 
    if (board.checkHasMatches())
    {
        processMatches();
    }
}