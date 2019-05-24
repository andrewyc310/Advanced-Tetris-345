#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <set>
using namespace std;
using namespace sf;

//horizont and vertical block size of the play field
const int M = 20;
const int N = 10;

//the 2d array of the playfield
int field[M][N] = {0};

// the coordinate of the playfield
struct Point
{
    int x, y, r, b;
} a[4], b[4];

//define tetris shapes
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

// init player and enemy
sf::Vector2f enemyStartPos = sf::Vector2f(150, 50);
sf::Vector2f playerStartPos = sf::Vector2f(300, 50);
sf::RectangleShape enemyRect(sf::Vector2f(50, 50));
sf::RectangleShape playerRect(sf::Vector2f(50, 50));


//the window of the game
RenderWindow window(sf::VideoMode(1024, 1000), "Advanced Tetris");

void showWelcome(sf::Font font)
{
    Event e;
    sf::Text welcome;
    // select the font
    welcome.setFont(font);
    // set the welcome text
    welcome.setString("Press any key to play");
    // set the character size
    welcome.setCharacterSize(24);
    // set the color
    welcome.setFillColor(sf::Color::Red);
    welcome.setPosition(512, 500);
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
    pause.setPosition(512, 500);
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
    gameOver.setPosition(512, 500);
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

//check for boundaries and collision with other shapes
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

void rotateBlock()
{
    Point p = a[1]; //center of rotation
    for (int i = 0; i < 4; i++)
    {
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
//Check all empty position and all blocks above the empty space fall
void FullLine()
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
        if(bHaveEmpty) j--;
    }
}

//Finding block with same color in adjacent cells
int AdjacentCount(int nValue, int nRow, int nCol, set<int>& sLst, bool bElm = false)
{
    //top bottom left right
    if (nRow - 1 > -1 && field[nRow - 1][nCol] == nValue)
    {
        std::pair<std::set<int>::iterator,bool> ret = sLst.insert((nRow - 1) * 100 + nCol);
        if (ret.second)
        {
            if(bElm) field[nRow - 1][nCol] = 0;
            AdjacentCount(nValue, nRow - 1, nCol, sLst, bElm);
        }
    }
    if (nRow + 1 < M && field[nRow + 1][nCol] == nValue)
    {
        std::pair<std::set<int>::iterator,bool> ret = sLst.insert((nRow + 1) * 100 + nCol);
        if (ret.second)
        {
            if(bElm) field[nRow + 1][nCol] = 0;
            AdjacentCount(nValue, nRow + 1, nCol, sLst, bElm);
        }
    }
    if (nCol - 1 > -1 && field[nRow][nCol - 1] == nValue)
    {
        std::pair<std::set<int>::iterator,bool> ret = sLst.insert(nRow * 100 + nCol - 1);
        if (ret.second)
        {
            if(bElm) field[nRow][nCol - 1] = 0;
            AdjacentCount(nValue, nRow, nCol - 1, sLst, bElm);
        }
    }
    if (nCol + 1 < N && field[nRow][nCol + 1] == nValue)
    {
        std::pair<std::set<int>::iterator,bool> ret = sLst.insert(nRow * 100 + nCol + 1);
        if (ret.second)
        {
            if(bElm) field[nRow][nCol + 1] = 0;
            AdjacentCount(nValue, nRow, nCol + 1, sLst, bElm);
        }
    }
    return sLst.size();
}

//Finding block with same color in adjacent cells
int AdjacentCount2(int nRow, int nCol, bool bElm = false)
{
    if(0 == field[nRow][nCol]) return 0;
    set<int> sLst;
    sLst.insert(nRow * 100 + nCol);
    int nValue = field[nRow][nCol];
    if(bElm) field[nRow][nCol] = 0;
    return AdjacentCount(nValue, nRow, nCol, sLst, bElm);
}

//Clear block when condition is reached
int checkElimination()
{
    int nAdNum;
    for (int i = M - 1; i > 0; i--)
    {
        nAdNum = 0;
        for (int j = 0; j < N; j++)
        {
            nAdNum = AdjacentCount2(i, j);
            if (nAdNum > 3)
            {
                AdjacentCount2(i, j, true);
                FullLine();
            }
        }
        if (nAdNum > 3)
        {
            i++;
        }
    }
    return nAdNum;
}

// init player and enemy position
void initPlayerPos()
{
    enemyRect.setFillColor(sf::Color::Red);
    enemyRect.setPosition(enemyStartPos);
    enemyRect.setOrigin(enemyRect.getSize() * 0.5f); // The center of the rectangle
    playerRect.setFillColor(sf::Color::Blue);
    playerRect.setPosition(playerStartPos);
    playerRect.setOrigin(playerRect.getSize() * 0.5f); // The center of the rectangle
}

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

void gameplay()
{
    srand(time(0));
    
    sf::Font font;
    if (!font.loadFromFile(resourcePath()+"simplistic_regular.ttf"))
    {
        window.setTitle("Font Error");
    }
    
    Texture t1, t2, t3;
    t1.loadFromFile(resourcePath()+"ntiles.png");
    // t2.loadFromFile("images/background.png");
    Sprite s(t1), background(t2), frame(t3);
    
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
    sf::Text scoreTitle;
    // set score text
    sf::Text strScore;
    // set cancelled title
    sf::Text cancTitle;
    // set cancelled text
    sf::Text cancScore;
    // set level title
    sf::Text levelTitle;
    // set level text
    sf::Text strLevel;
    
    // select the font
    scoreTitle.setFont(font);
    // set the score title
    scoreTitle.setString("SCORE");
    // set the character size
    scoreTitle.setCharacterSize(24);
    // set the color
    scoreTitle.setFillColor(sf::Color::Red);
    scoreTitle.setPosition(800, 50);
    
    // select the font
    strScore.setFont(font);
    // set init score
    strScore.setString("0");
    // set the character size
    strScore.setCharacterSize(24);
    // set the color
    strScore.setFillColor(sf::Color::Red);
    strScore.setPosition(800, 100);
    
    // select the font
    cancTitle.setFont(font);
    // set the cancelled titel
    cancTitle.setString("CANCELLED");
    // set the character size
    cancTitle.setCharacterSize(24);
    // set the color
    cancTitle.setFillColor(sf::Color::Red);
    cancTitle.setPosition(800, 150);
    
    // select the font
    cancScore.setFont(font);
    // set init cancelled
    cancScore.setString("0");
    // set the character size
    cancScore.setCharacterSize(24);
    // set the color
    cancScore.setFillColor(sf::Color::Red);
    cancScore.setPosition(800, 200);
    
    // select the font
    levelTitle.setFont(font);
    // set the level title
    levelTitle.setString("LEVEL");
    // set the character size
    levelTitle.setCharacterSize(24);
    // set the color
    levelTitle.setFillColor(sf::Color::Red);
    levelTitle.setPosition(800, 250);
    
    // select the font
    strLevel.setFont(font);
    // set init level
    strLevel.setString("1");
    // set the character size
    strLevel.setCharacterSize(24);
    // set the color
    strLevel.setFillColor(sf::Color::Red);
    strLevel.setPosition(800, 300);
    
    /*main playing area container*/
    sf::RectangleShape container(sf::Vector2f(411, 830));
    container.setFillColor(sf::Color::White);
    container.setPosition(179, 130);
    
    /*enemy and player*/
    initPlayerPos();
    
    bool showMain = 0;
    int cc[4] = {0};
    int nType = rand() % 7;;
    /*When the game is running*/
    while (window.isOpen())
    {
        if (!showMain)
        {
            showWelcome(font);
            showMain = 1;
        }
        
        
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        
        Event e;
        
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
                if (e.key.code == Keyboard::Up)
                {
                    isRotate = true;
                }
                else if (e.key.code == Keyboard::Left)
                {
                    distanceX = -1;
                }
                else if (e.key.code == Keyboard::Right)
                {
                    distanceX = 1;
                }
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
        
        // Enemy's moving speed
        enemyRect.move(0, 0);
        
        // Enemy got you, game over
        if (playerRect.getGlobalBounds().intersects(enemyRect.getGlobalBounds()))
        {
            showOver(font);
            return;
        }
        
        // enter next level
        if (playerRect.getPosition().x >= 450)
        {
            level++;
            strLevel.setString(std::to_string(level));
            enemyRect.setPosition(enemyStartPos);
            playerRect.setPosition(playerStartPos);
        }
        
        //left and right movement
        horizMove(distanceX);
        
        // block rotation
        if (isRotate)
        {
            rotateBlock();
        }
        
        //Tick
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].y += 1;
            }
            
            if (!checkBlocksPos())
            {
                //for (int i=0;i<4;i++) field[b[i].y][b[i].x]=colorIndex;
                for (int i = 0; i < 4; i++)
                {
                    field[b[i].y][b[i].x] = cc[i];
                    //field[b[i].y][b[i].x]=colorIndex;
                }
                FullLine();
                colorIndex = 1 + rand() % 6; //update color index from 1 to 6
                //colorIndex = rand() % 7; //update color index from 1 to 6
                
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
        
        // elimination
        checkElimination();
        
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
        
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0)
                {
                    continue;
                }
                s.setTextureRect(IntRect(field[i][j] * 40, 0, 40, 40));
                s.setPosition(j * 41, i * 41);
                s.move(180, 140); //offset, the point generate a new shape
                window.draw(s);
            }
        }
        
        switch (nType) {
            case 0:         //line
                cc[0] = 4;
                cc[1] = 4;
                cc[2] = 3;
                cc[3] = 3;
                break;
            case 1:         //S
                cc[0] = 4;
                cc[1] = 5;
                cc[2] = 4;
                cc[3] = 5;
                break;
            case 2:         //Z
                cc[0] = 4;
                cc[1] = 5;
                cc[2] = 4;
                cc[3] = 5;
                break;
            case 3:         //T
                cc[0] = 5;
                cc[1] = 3;
                cc[2] = 5;
                cc[3] = 3;
                break;
            case 4:        //L
                cc[0] = 4;
                cc[1] = 4;
                cc[2] = 3;
                cc[3] = 3;
                break;
            case 5:        //J
                cc[0] = 5;
                cc[1] = 5;
                cc[2] = 4;
                cc[3] = 4;
                break;
            case 6:        //田
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
        }
        
        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(IntRect(cc[i] * 40, 0, 40, 40));
            s.setPosition(a[i].x * 41, a[i].y * 41);
            s.move(180, 140); //offset,the point generate a new shape
            window.draw(s);
        }
        
        window.display();
    }
}

int main(){
    while (1)
    {
        gameplay();
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

