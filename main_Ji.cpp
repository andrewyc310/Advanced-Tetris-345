//#include "pch.h"
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
using namespace std;
using namespace sf;

//horizont Block size
const int M = 20;
//verticle block size
const int N = 10;

//the 2d array of the playfield
//20 * 10 size of the playfield
int field[M][N] = {0};

//define a constructor Point
// a[4] and b[4] hold the coordinate of each tetris (contains 4 block each tetris)
struct Point
{
    int x, y;
} a[4], b[4];

//define 7 tetris shapes
int figures[7][4] =
{
  // I
  1,3,5,7,
  // Z 
  2,4,5,7, 
  // S
  3,5,4,6, 
  // T
  3,5,4,7, 
  // L
  2,3,5,7, 
  // J
  3,5,7,6,
   // O 
  2,3,4,5,
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
		//check if tetris go over the left, right, and bottom boundaries
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
        {
            return 0;
        }
		//check if tetris has collision with other tetris
        if (field[a[i].y][a[i].x])
        {
            return 0;
        }
    }
    return 1;
};

//rotation movements
void rotateBlock()
{
	//center of rotation
    Point p;
    //if the tetris shape is a O
	if(a[0].x == a[1].x && a[0].y == a[2].y && a[2].x == a[3].x) 
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
    //for other shapes  
    }
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

//elimination method
void checkElimination()
{
    int k = M - 1;
    for (int i = M - 1; i > 0; i--)
    {
        int count = 0;
        for (int j = 0; j < N; j++)
        {
            if (field[i][j])
            {
                count++;
            }
            field[k][j] = field[i][j];
        }
        if (count < N)
        {
            k--;
        }
    }
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

//left and right movement
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
    if (!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-M.ttf"))
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
    int colorIndex = 2;
    //define timer
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

    // set a flag to decide if to show welcome screen
    bool isShowWelcome = 1;
    /*When the game is running*/
    while (window.isOpen())
    {
        // show welcome screen, if is shown, set flag to 0
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
      	timer+=time;

        Event e;
        /* Event listener, set close and press key operations
         * Press up to rotate blocks, press left/right/down to move left/right/down
         * Press P or esc to pause the game
         */
        while (window.pollEvent(e))
        {
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
                    isRotate = true;
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
        // can we remove the flag and move the rotateBlock() into event listener?
        if (isRotate)
        {
            rotateBlock();
        }

        //Gravity movements
  		//cc[4] is the array to store 4 differ color values, then apply on each block of each tetris
        int cc[4];
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++)
            {	
				//b[] is recording the tetris shape when it going down, and when it touched ground or other block
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!checkBlocksPos())
            {
                //set field[b[i].y][b[i].x] to cc[i] color.
                for (int i = 0; i < 4; i++)
                {
                    field[b[i].y][b[i].x] = cc[i];
                }
				//update color index from 1 to 6
                colorIndex = 1 + rand() % 5; 

                //initialise different tetris, n ranged from 0-6
                int n = rand() % 7;
				//for each block, set their coordinate based on their shape
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }
			//set timer back to 0
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
				// the top left point generate a new shape
                s.move(180, 140); 
				// draw the sprite
                window.draw(s);
            }
        }
        for (int i = 0; i < 4; i++)
        {
			//for each block, regenerate the color 
            cc[i] = colorIndex;
            if (i == 0)
            {
                if (cc[i] <= 2)
                {
                    cc[i] += 2;
                }
                else
                {
                    cc[i]--;
                }
            }
            else if (i == 2)
            {
                if (cc[i] >= 2)
                {
                    cc[i]--;
                }
                else
                {
                    cc[i] += 4;
                }
            }
            else if (i == 1)
            {
                if (cc[i] <= 3)
                {
                    cc[i] += 3;
                }
            }
            else if (i == 3)
            {
                if (cc[i] >= 3)
                {
                    cc[i] -= 2;
                }
                else
                {
                    cc[i] += 4;
                }
            }
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

int main()
{
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
