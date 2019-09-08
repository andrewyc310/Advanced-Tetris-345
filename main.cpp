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

Texture t1;

Sprite s(t1);

/** The 2d array of the playfield，20 * 10.*/
int field[M][N] = { 0 };

/** Elimination counter.*/
int tscore = 0;

/** previous nType.*/
int pre_n = 11;

/** Font for text.*/
sf::Font font;

/** Colours for text. */
sf::Color fontColours[] = {
    sf::Color::Red,
    sf::Color::Blue,
    sf::Color::Yellow,
    sf::Color::Green,
    sf::Color::Magenta
};

/**
 * Define a constructor Point.
 * a[4] and b[4] hold the coordinate of each tetris (contains 4 block each tetris).
 */
struct Point
{
    int x, y;
};
Point a[4] = { {1,1},{1,2},{0,2},{1,3} };
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

/** The window of the game.*/
RenderWindow window(sf::VideoMode(1024, 1000), "Advanced Tetris");

/**
 * Set text attributs.
 * @param info content of text.
 * @param x, y position of text.
 */
sf::Text setText(std::string info, int x, int y)
{
    // Init text with font, content, size, colour, pos.
    sf::Text text;
    text.setFont(font);
    text.setString(info);
    text.setCharacterSize(24);
    text.setFillColor(fontColours[0]);
    text.setPosition(x, y);
    return text;
}

/**
 * Show welcome screen.
 * @param font the font of the text.
 */
void showWelcome(sf::Font font)
{
    Event e;
    //Set welcome info
    sf::Text welcome = setText("          Welcome!\nPress any key to play", 350, 400);
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
    //Set pause info
    sf::Text pause = setText("Game pausing, press 'Y' to continue.", 300, 400);
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
    sf::Text gameOver = setText("Game Over, press 'Y' to play again.", 300, 400);
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
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
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
    t1.loadFromFile("images/ntiles1.png");
    
    //top bottom left right
    if (nRow - 1 > -1 && field[nRow - 1][nCol] == nValue)
    {
        std::pair<std::set<int>::iterator, bool> ret = sLst.insert((nRow - 1) * 100 + nCol);
        if (ret.second)
        {
            if (bElm)
            {
                field[nRow -1 ][nCol] = 6;
                s.setTextureRect(IntRect(field[nRow-1][nCol] * 40, 0, 40, 40));
                s.setPosition(nCol * 41+180, (nRow-1) * 41+140);
                window.draw(s);
                window.display();
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
                field[nRow + 1][nCol] = 6;
                s.setTextureRect(IntRect(field[nRow+1][nCol] * 40, 0, 40, 40));
                s.setPosition(nCol * 41+180, (nRow+1) * 41+140);
                window.draw(s);
                window.display();
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
                field[nRow][nCol - 1] = 6;
                s.setTextureRect(IntRect(field[nRow][nCol-1] * 40, 0, 40, 40));
                s.setPosition((nCol-1) * 41+180, nRow * 41+140);
                window.draw(s);
                window.display();
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
                field[nRow][nCol + 1] = 6;
                s.setTextureRect(IntRect(field[nRow][nCol+1] * 40, 0, 40, 40));
                s.setPosition((nCol+1) * 41+180, nRow * 41+140);
                window.draw(s);
                window.display();
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
                fullLine();
                adjacentCount2(i, j, true);
                fullLine();
            }
        }
        if (nAdNum > 3)
        {
            i=M-1;
        }
    }
}
/**
 * Check whether the game is over.
 */
bool checkOver()
{
    bool k = true;
    for(int i = 0; i < N; i++){
        if(field[1][i] !=0 ){
            k = false;
            return k;
        }
    }
    return k;
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
    
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        window.setTitle("Font Error");
    }
    
    t1.loadFromFile("images/ntiles1.png");
    
    //define x coordinate of blocks
    int distanceX = 0;
    //define if rotate or not
    bool isRotate = 0;
    //define color index of image
    int colorIndex = 2;
    //difine timer
    float timer = 0;
    //define speed for gravity
    float delay = 0.5;
    
    Clock clock;
    
    window.setFramerateLimit(10);
    
    /* Set the score area*/
    int score = 0, level = 1, canc = 0;
    // set score title
    sf::Text scoreTitle = setText("SCORE", 700, 50);
    // set score text
    sf::Text strScore = setText("0", 700, 100);
    // set cancelled title
    sf::Text cancTitle = setText("CANCELLED", 700, 150);
    // set cancelled text
    sf::Text cancScore = setText("0", 700, 200);
    // set level title
    sf::Text levelTitle = setText("LEVEL", 700, 250);
    // set level text
    sf::Text strLevel = setText("1", 700, 300);
    // set help text
    sf::Text manual = setText("UP: Rotate\nLeft/Right: Move\nDown: Speed Drop\nESC/P: Pause", 700, 350);
    // set test message, for test only
    sf::Text testMsg = setText("", 700, 650);
    
    /*main playing area container*/
    sf::RectangleShape container(sf::Vector2f(411, 830));
    container.setFillColor(sf::Color::White);
    container.setPosition(179, 130);
    
    /*enemy and player*/
    Texture enemyT;
    enemyT.loadFromFile("images/enemy.png");
    Sprite enemy(enemyT);
    enemy.scale(0.2, 0.2);
    Texture playerT;
    playerT.loadFromFile("images/player.png");
    Sprite player(playerT);
    player.scale(0.2, 0.2);
    enemy.setPosition(enemyStartPos);
    player.setPosition(playerStartPos);
    
    // set a flag to decide if to show welcome screen
    bool isShowWelcome = 1;
    int cc[4] = { 0 };
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
                else if (e.key.code == Keyboard::Space)
                {
                    int calc;
                    int temp_y=0;
                    int hor;
                    int col;
                    
                    for (int i = 3; i >= 0; i--)
                    {
                        hor = a[i].x;
                        cout << "hor is " << hor << ". ";
                        if (a[i].y > temp_y) {
                            temp_y = a[i].y;
                        }
                    }
                    if (field[19][hor] == 0) {
                        
                        calc = 18 - temp_y;
                        cout << "calc is " << calc << ". ";
                        for (int i = 3; i >= 0; i--)
                        {
                            a[i].y += calc;
                        }
                    }else {
                        for (int j = 0; j < 20; j++) {
                            if (field[j][hor] != 0) {
                                cout << "j is " << j << ". ";
                                col = j - 2;
                                //field[j][hor] = cc[i];
                                //
                                break;
                            }
                        }
                        calc = col - temp_y;
                        for (int i = 3; i >= 0; i--)
                        {
                            
                            //a[i].y = col;
                            a[i].y += calc;
                            //b[i] = a[i];
                        }
                        
                    }
                    cout << "\t ";
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
                    enemy.move(-1, 0);
                    player.move(1, 0);
                    score += 10;
                    strScore.setString(std::to_string(score));
                    canc++;
                    cancScore.setString(std::to_string(canc));
                }
            }
        }
        
        // Enemy's chasing speed
        enemy.move(0.16 + level * 0.04, 0);
        
        // Enemy got you, game over
        if (player.getGlobalBounds().intersects(enemy.getGlobalBounds()))
        {
            showOver(font);
            return;
        }
        
        // enter next level
        if (player.getPosition().x >= 550)
        {
            level++;
            strLevel.setString(std::to_string(level));
            scoreTitle.setFillColor(fontColours[level]);
            strScore.setFillColor(fontColours[level]);
            cancTitle.setFillColor(fontColours[level]);
            cancScore.setFillColor(fontColours[level]);
            levelTitle.setFillColor(fontColours[level]);
            strLevel.setFillColor(fontColours[level]);
            manual.setFillColor(fontColours[level]);
            testMsg.setFillColor(fontColours[level]);
            enemy.setPosition(enemyStartPos);
            player.setPosition(playerStartPos);
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
                colorIndex = 1 + rand() % 6; //update color index from 1 to 6
                
                nType = rand() % 7;
                
                if (pre_n != nType)
                {
                    pre_n = nType;
                }
                else
                {
                    nType += 2;
                    nType = nType % 8;
                    pre_n = nType;
                }
                
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[nType][i] % 2;
                    a[i].y = figures[nType][i] / 2;
                }
            }
            timer = 0;
        }
        if (!checkOver()){
            showOver(font);
            return;
        }
        // block elimination, player move and scores update
        checkElimination();
        player.move(tscore * tscore / 2, 0);
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
        window.draw(player);
        window.draw(enemy);
        window.draw(container);
        window.draw(scoreTitle);
        window.draw(strScore);
        window.draw(cancTitle);
        window.draw(cancScore);
        window.draw(levelTitle);
        window.draw(strLevel);
        window.draw(manual);
        window.draw(testMsg);
        
        for (int i = 0; i < M; i++)
        {
            //for (int j = 0; j < N; j++)
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0)
                {
                    continue;
                }
                s.setTextureRect(IntRect(field[i][j] * 40, 0, 40, 40));
                s.setPosition(j * 41, i * 41);
                // the top left point generate a new shape
                s.move(180, 140);
                // draw the sprite
                window.draw(s);
            }
        }
        
        //for each block, regenerate the color
        switch (nType)
        {
            case 0: //line
                cc[0] = 1;
                cc[1] = 1;
                cc[2] = 3;
                cc[3] = 3;
                break;
            case 1: //S
                cc[0] = 4;
                cc[1] = 1;
                cc[2] = 4;
                cc[3] = 1;
                break;
            case 2: //Z
                cc[0] = 1;
                cc[1] = 5;
                cc[2] = 1;
                cc[3] = 5;
                break;
            case 3: //T
                cc[0] = 4;
                cc[1] = 3;
                cc[2] = 4;
                cc[3] = 3;
                break;
            case 4: //L
                cc[0] = 5;
                cc[1] = 5;
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
                cc[0] = 1;
                cc[1] = 3;
                cc[2] = 4;
                cc[3] = 5;
                break;
            default:
                cc[0] = colorIndex;
                cc[1] = colorIndex;
                cc[2] = colorIndex;
                cc[3] = colorIndex;
                break;
        }
        
        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(IntRect(cc[i] * 40, 0, 40, 40));
            s.setPosition(a[i].x * 41, a[i].y * 41);
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
