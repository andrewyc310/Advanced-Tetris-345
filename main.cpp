/**
 * The Advanced Tetris Project.
 * @date May 2019.
 * @author Ji Liu, Andrew Wang, Bella Gao
 */

#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <set>

using namespace std;
using namespace sf;

/** Horizont Block size.*/
const int M = 20;
/** Verticle block size.*/
const int N = 10;

/** The 2d array of the playfield，20 * 10.*/
int field[M][N] = {0};

/** Elimination counter.*/
int tscore = 0;

/**
 * Define a constructor Point.
 * a[4] and b[4] hold the coordinate of each tetris (contains 4 block each tetris).
 */
struct Point
{
    int x, y;
};
/** initial bloack shape BELLA*/
Point a[4] = { {0,1}, {0,2}, {1,2}, {1,3} };
Point b[4];


/** Define 7 tetris shapes.*/
int figures[7][4] =
    {
        1, 3, 5, 7, // I
        2, 4, 5, 7, // Z
        3, 5, 4, 6, // S
        3, 5, 4, 7, // T
        2, 3, 5, 7, // L
        3, 5, 7, 6, // J
        2, 3, 4, 5, // O
};

/** Init player's and enemy's shape and start pos.*/
sf::Vector2f enemyStartPos = sf::Vector2f(150, 50);
sf::Vector2f playerStartPos = sf::Vector2f(300, 50);
sf::RectangleShape enemyRect(sf::Vector2f(50, 50));
sf::RectangleShape playerRect(sf::Vector2f(50, 50));

/** The window of the game.*/
RenderWindow window(sf::VideoMode(1024, 1000), "Advanced Tetris");

/**
 * Show welcome screen.
 * @param font the font of the text.
 */
void showWelcome(sf::Font font)
{
    Event e;
    sf::Text welcome;
    // select the font
    welcome.setFont(font);
    // set the welcome text
    welcome.setString("          Welcome!\nPress any key to play");
    // set the character size
    welcome.setCharacterSize(24);
    // set the color
    welcome.setFillColor(sf::Color::Red);
    welcome.setPosition(350, 400);
    while (window.isOpen())
    {
        // Event listener
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
                exit(0);
            }
            if (e.type == Event::KeyPressed)
            {
                return;
            }
        }
        // drawing elements
        window.clear();
        window.draw(welcome);

        window.display();
    }
}

/**
 * Show pause screen.
 * @param font the font of the text.
 */
void showPause(sf::Font font)
{
    Event e;
    sf::Text pause;
    // select the font
    pause.setFont(font);
    // set the game over text
    pause.setString("Game pausing, press 'Y' to continue.");
    // set the character size
    pause.setCharacterSize(24);
    // set the color
    pause.setFillColor(sf::Color::Red);
    pause.setPosition(300, 400);
    while (window.isOpen())
    {
        // Event listener
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
                exit(0);
            }
            if (e.key.code == Keyboard::Y)
            {
                return;
            }
        }
        // drawing elements
        window.clear();
        window.draw(pause);
        window.display();
    }
}

/**
 * Show game over screen.
 * @param font the font of the text.
 */
void showOver(sf::Font font)
{
    Event e;
    sf::Text gameOver;
    // select the font
    gameOver.setFont(font);
    // set the game over text
    gameOver.setString("Game Over, press 'Y' to play again.");
    // set the character size
    gameOver.setCharacterSize(24);
    // set the color
    gameOver.setFillColor(sf::Color::Red);
    gameOver.setPosition(300, 400);
    while (window.isOpen())
    {
        // Event listener
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
                exit(0);
            }
            if (e.key.code == Keyboard::Y)
            {
                return;
            }
        }
        // drawing elements
        window.clear();
        window.draw(gameOver);
        window.display();
    }
}

/**
 * Check for boundaries and collision with other blocks.
 * @return 1 if no collisions or out of bounds,
 *          0 if coolisions or out of bounds.
 */
bool checkBlocksPos()
{
    for (int i = 0; i < 4; i++)
    {
        //if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) BELLA
        if (a[i].x < -4 || a[i].x >= 6 || a[i].y >= M)
        {
            return 0;
        }
        if (field[a[i].y][a[i].x])
        {
            return 0;
        }
    }
    return 1;
};

/**
 * Rotation movements.
 */
void rotateBlock()
{
    //center of rotation
    Point p;
    //if the tetris shape is a O
    if (a[0].x == a[1].x && a[0].y == a[2].y && a[2].x == a[3].x)
    {
        //do the rotation
        Point a0 = a[0];
        Point a1 = a[1];
        Point a2 = a[2];
        Point a3 = a[3];

        a[0] = a1;
        a[1] = a3;
        a[2] = a0;
        a[3] = a2;
    }
    //for other shapes
    else
    {
        //center of rotation
        p = a[1];
        for (int i = 0; i < 4; i++)
        {
            //do the rotation
            int x = a[i].y - p.y;
            int y = a[i].x - p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
        }
        if (!checkBlocksPos())
        {
            for (int i = 0; i < 4; i++)
            {
                a[i] = b[i];
            }
        }
    }
}

/**
 * Check all empty position and all blocks above the empty space fall.
 */
void fullLine()
{
    int k = M - 1;
    bool bHaveEmpty = false;
    for (int j = 0; j < N; j++)
    {
        bHaveEmpty = false;
        for (int i = M - 1; i > 0; i--)
        {
            if (field[i][j] == 0 && field[i - 1][j] != 0)
            {
                field[i][j] = field[i - 1][j];
                field[i - 1][j] = 0;
                bHaveEmpty = true;
            }
        }
        if (bHaveEmpty)
        {
            j--;
        }
    }
}

/**
 * Checking the status of all adjacent block from adjacentCount2.
 * @param nRow the row value of the current block.
 * @param nCol the column value of the current block.
 * @param sLst the list to contain all blocks that needs to be elminated
 * @param bElm check whether this block should be elminated
 * @return the size of block getting elminated from adjacentCount.
 */
int adjacentCount(int nValue, int nRow, int nCol, set<int> &sLst, bool bElm = false)
{
    //top bottom left right
    if (nRow - 1 > -1 && field[nRow - 1][nCol] == nValue)
    {
        std::pair<std::set<int>::iterator, bool> ret = sLst.insert((nRow - 1) * 100 + nCol);
        if (ret.second)
        {
            if (bElm)
            {
                field[nRow - 1][nCol] = 0;
            }
            adjacentCount(nValue, nRow - 1, nCol, sLst, bElm);
        }
    }
    if (nRow + 1 < M && field[nRow + 1][nCol] == nValue)
    {
        std::pair<std::set<int>::iterator, bool> ret = sLst.insert((nRow + 1) * 100 + nCol);
        if (ret.second)
        {
            if (bElm)
            {
                field[nRow + 1][nCol] = 0;
            }
            adjacentCount(nValue, nRow + 1, nCol, sLst, bElm);
        }
    }
    if (nCol - 1 > -1 && field[nRow][nCol - 1] == nValue)
    {
        std::pair<std::set<int>::iterator, bool> ret = sLst.insert(nRow * 100 + nCol - 1);
        if (ret.second)
        {
            if (bElm)
            {
                field[nRow][nCol - 1] = 0;
            }
            adjacentCount(nValue, nRow, nCol - 1, sLst, bElm);
        }
    }
    if (nCol + 1 < N && field[nRow][nCol + 1] == nValue)
    {
        std::pair<std::set<int>::iterator, bool> ret = sLst.insert(nRow * 100 + nCol + 1);
        if (ret.second)
        {
            if (bElm)
            {
                field[nRow][nCol + 1] = 0;
            }
            adjacentCount(nValue, nRow, nCol + 1, sLst, bElm);
        }
    }
    return sLst.size();
}

/**
 * Check a block's status and find its adjacent blocks.
 * @param nRow the row value of the current block.
 * @param nCol the column value of the current block.
 * @param bElm check whether this block should be elminated
 * @return the size of block getting elminated from adjacentCount.
 */
int adjacentCount2(int nRow, int nCol, bool bElm = false)
{
    if (0 == field[nRow][nCol])
    {
        return 0;
    }
    set<int> sLst;
    sLst.insert(nRow * 100 + nCol);
    int nValue = field[nRow][nCol];
    if (bElm)
    {
        field[nRow][nCol] = 0;
    }
    return adjacentCount(nValue, nRow, nCol, sLst, bElm);
}

/**
 * Clear block when condition is reached.
 */
void checkElimination()
{
    int nAdNum;
    for (int i = M - 1; i > 0; i--)
    {
        nAdNum = 0;
        for (int j = 0; j < N; j++)
        {
            nAdNum = adjacentCount2(i, j);
            if (nAdNum > 3)
            {
                tscore += nAdNum;
                adjacentCount2(i, j, true);
                fullLine();
            }
        }
        if (nAdNum > 3)
        {
            i++;
        }
    }
}

/**
 * Init player and enemy position.
 */
void initPlayerPos()
{
    enemyRect.setFillColor(sf::Color::Red);
    enemyRect.setPosition(enemyStartPos);
    enemyRect.setOrigin(enemyRect.getSize() * 0.5f); // The center of the rectangle
    playerRect.setFillColor(sf::Color::Blue);
    playerRect.setPosition(playerStartPos);
    playerRect.setOrigin(playerRect.getSize() * 0.5f); // The center of the rectangle
}

/**
 * Left and right movement.
 * @param distanceX the amount to move.
 */
void horizMove(int distanceX)
{
    for (int i = 0; i < 4; i++)
    {
        b[i] = a[i];
        a[i].x += distanceX;
    }
    if (!checkBlocksPos())
    {
        for (int i = 0; i < 4; i++)
        {
            a[i] = b[i];
        }
    }
}

/**
 * The game.
 */
void gameplay()
{
    srand(time(0));

    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        window.setTitle("Font Error");
    }
    
    Texture t1;
    t1.loadFromFile("images/ntiles.png");
    Sprite s(t1);

    //define x coordinate of blocks
    int distanceX = 0;
    //define if rotate or not
    bool isRotate = 0;
    //define color index of image
    int colorIndex [] = {2,3,4,1};
    //difine timer
    float timer = 0;
    //define speed for gravity
    float delay = 0.5;

    Clock clock;

    window.setFramerateLimit(10);

    /* Set the score area*/
    int score = 0, level = 1, canc = 0;

    // set score title, select the font,set the score title,character size,color,position
    sf::Text scoreTitle;
    scoreTitle.setFont(font);
    scoreTitle.setString("SCORE");
    scoreTitle.setCharacterSize(24);
    scoreTitle.setFillColor(sf::Color::Red);
    scoreTitle.setPosition(700, 50);

    // set score text,select the font, set the init score,character size,color,position
    sf::Text strScore;
    strScore.setFont(font);
    strScore.setString("0");
    strScore.setCharacterSize(24);
    strScore.setFillColor(sf::Color::Red);
    strScore.setPosition(700, 100);

    // set cancelled title,select the font,set the cancelled titel,character size,color,position
    sf::Text cancTitle;
    cancTitle.setFont(font);
    cancTitle.setString("CANCELLED");
    cancTitle.setCharacterSize(24);
    cancTitle.setFillColor(sf::Color::Red);
    cancTitle.setPosition(700, 150);

    // set score title, select the font,set the score title,character size,color,position
    sf::Text cancScore;
    cancScore.setFont(font);
    cancScore.setString("0");
    cancScore.setCharacterSize(24);
    cancScore.setFillColor(sf::Color::Red);
    cancScore.setPosition(700, 200);

    // set level title,select the font,set the level title,character size,color,position
    sf::Text levelTitle;
    levelTitle.setFont(font);
    levelTitle.setString("LEVEL");
    levelTitle.setCharacterSize(24);
    levelTitle.setFillColor(sf::Color::Red);
    levelTitle.setPosition(700, 250);

    // set level text,select the font,set init level,character size,color,position
    sf::Text strLevel;
    strLevel.setFont(font);
    strLevel.setString("1");
    strLevel.setCharacterSize(24);
    strLevel.setFillColor(sf::Color::Red);
    strLevel.setPosition(700, 300);

    // set help text,select the font,set control manual,character size,color,position
    sf::Text helpText;
    helpText.setFont(font);
    helpText.setString("UP: Rotate\nLeft/Right: Move\nDown: Speed Drop\nESC/P: Pause");
    helpText.setCharacterSize(24);
    helpText.setFillColor(sf::Color::Red);
    helpText.setPosition(700, 350);

    // set test message, for test only
    sf::Text testMsg;
    testMsg.setFont(font);
    testMsg.setString("");
    testMsg.setCharacterSize(24);
    testMsg.setFillColor(sf::Color::Red);
    testMsg.setPosition(700, 650);

    /*main playing area container*/
    sf::RectangleShape container(sf::Vector2f(411, 830));
    container.setFillColor(sf::Color::White);
    container.setPosition(179, 130);

    /*enemy and player*/
    initPlayerPos();
    // set a flag to decide if to show welcome screen
    bool isShowWelcome = 1;
    int cc[4] = {0};
    int nType = rand() % 7;
    /*When the game is running*/
    while (window.isOpen())
    {
        if (isShowWelcome)
        {
            showWelcome(font);
            isShowWelcome = 0;
        }

        //to retrieve the time elapsed since the clock started, per second
        float time = clock.getElapsedTime().asSeconds();

        //restart the clock.
        clock.restart();

        //update time
        timer += time;

        Event e;

        /* Event listener, set close and press key operations
         * Press up to rotate blocks, press left/right/down to move left/right/down
         * Press P or esc to pause the game
         */
        while (window.pollEvent(e))
        {
            //Close window
            if (e.type == Event::Closed)
            {
                window.close();
                exit(0);
            }
            //check if any key has been pressed
            if (e.type == Event::KeyPressed)
            {
                //when up key pressed, use is_rotate to triggle the rotation method
                if (e.key.code == Keyboard::Up)
                {
                    rotateBlock();
                }
                //when left key pressed, move object to the 1 distance left
                else if (e.key.code == Keyboard::Left)
                {
                    distanceX = -1;
                }
                //when right key pressed, move object to the 1 distance right
                else if (e.key.code == Keyboard::Right)
                {
                    distanceX = 1;
                }
                //if down key pressed, accelerate the gravity
                else if (e.key.code == Keyboard::Down)
                {
                    delay = 0.05;
                }
                else if (e.key.code == Keyboard::P)
                {
                    showPause(font);
                }
                else if (e.key.code == Keyboard::Escape)
                {
                    showPause(font);
                }
                else if (e.key.code == Keyboard::Insert)
                {
                    /* for test only, press insert to get score,
                     cancellation, move player*/
                    enemyRect.move(-1, 0);
                    playerRect.move(1, 0);
                    score += 10;
                    strScore.setString(std::to_string(score));
                    canc++;
                    cancScore.setString(std::to_string(canc));
                }
            }
        }

        // Enemy's chasing speed
        enemyRect.move(0.2, 0);

        // Enemy got you, game over
        if (playerRect.getGlobalBounds().intersects(enemyRect.getGlobalBounds()))
        {
            showOver(font);
            return;
        }

        // enter next level
        if (playerRect.getPosition().x >= 550)
        {
            level++;
            strLevel.setString(std::to_string(level));
            enemyRect.setPosition(enemyStartPos);
            playerRect.setPosition(playerStartPos);
        }

        //left and right movement
        horizMove(distanceX);

        //Gravity movements
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!checkBlocksPos())
            {
                for (int i = 0; i < 4; i++)
                {
                    field[b[i].y][b[i].x] = cc[i];
                }
                fullLine();
                for (int i = 0; i < 4; i++){
                colorIndex[i] = 1 + rand() % 6;
                }//update color index for first block
                 //update color index for 4th block

                //initialise different tetris, n ranged from 0-6
                nType = rand() % 7;
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[nType][i] % 2;
                    a[i].y = figures[nType][i] / 2;
                }
            }
            timer = 0;
        }

        // block elimination, player move and scores update
        checkElimination();
        playerRect.move(tscore * tscore / 2, 0);
        score += tscore * tscore * 2;
        strScore.setString(std::to_string(score));
        canc += tscore;
        cancScore.setString(std::to_string(canc));
        tscore = 0;
        // reset distanceX, isRotate, delay
        distanceX = 0;
        isRotate = 0;
        delay = 0.5;

        // drawing elements
        window.clear();
        window.draw(playerRect);
        window.draw(enemyRect);
        window.draw(container);
        window.draw(scoreTitle);
        window.draw(strScore);
        window.draw(cancTitle);
        window.draw(cancScore);
        window.draw(levelTitle);
        window.draw(strLevel);
        window.draw(helpText);
        window.draw(testMsg);

        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0)
                {
                    continue;
                }
                //s.setTextureRect(IntRect(field[i][j] * 40, 0, 40, 40)); BELLA
                s.setTextureRect(IntRect(field[i][j] * 100, 40, 40, 40));//初始位置改这里BELLA
                s.setPosition(164 + j * 41, i * 41);//BELLA
                // the top left point generate a new shape
                s.move(180, 140);
                // draw the sprite
                window.draw(s);
            }
        }

        //for each block, regenerate the color
      /**  switch (nType)
        {
        case 0: //line
            cc[0] = 4;
            cc[1] = 4;
            cc[2] = 3;
            cc[3] = 3;
            break;
        case 1: //S
            cc[0] = 4;
            cc[1] = 5;
            cc[2] = 4;
            cc[3] = 5;
            break;
        case 2: //Z
            cc[0] = 4;
            cc[1] = 5;
            cc[2] = 4;
            cc[3] = 5;
            break;
        case 3: //T
            cc[0] = 5;
            cc[1] = 3;
            cc[2] = 5;
            cc[3] = 3;
            break;
        case 4: //L
            cc[0] = 4;
            cc[1] = 4;
            cc[2] = 3;
            cc[3] = 3;
            break;
        case 5: //J
            cc[0] = 5;
            cc[1] = 5;
            cc[2] = 4;
            cc[3] = 4;
            break;
        case 6: //O
            cc[0] = 5;
            cc[1] = 3;
            cc[2] = 3;
            cc[3] = 5;
            break;
        default:
            cc[0] = colorIndex;
            cc[1] = colorIndex;
            cc[2] = colorIndex;
            cc[3] = colorIndex;
            break;
        }  BELLA*/

        for (int i = 0; i < 4; i++)
        {
            cc[i] = colorIndex[i];
            //s.setTextureRect(IntRect(cc[i] * 40, 0, 40, 40));
            s.setTextureRect(IntRect(cc[i] * 100, 40, 40, 40)); //初始位置改这里BELLA
            s.setPosition(164 + a[i].x * 41, a[i].y * 41);//位置位置改这里BELLA
            //the top left point generate a new shape
            s.move(180, 140);
            // draw the sprite
            window.draw(s);
        }

        window.display();
    }
}

/**
 * The main function of the game.
 * @return 0 if the program exits normally
 */
int main()
{
    while (1)
    {
        gameplay();
        // if game over, reset the playing container and blocks
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                field[i][j] = 0;
            }
        }
        for (int i = 0; i < 4; i++)
        {
            a[i].x = 0;
            b[i].x = 0;
            a[i].y = 0;
            b[i].y = 0;
        }
    }

    return 0;
}
