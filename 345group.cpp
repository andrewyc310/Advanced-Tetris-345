#include "pch.h"
#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = {0};

struct Point
{
    int x, y;
} a[4], b[4];

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

bool check()
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return 0;
        else if (field[a[i].y][a[i].x])
            return 0;

    return 1;
};

void initShape(sf::RectangleShape &shape, sf::Vector2f const &pos, sf::Color const &color)
{
    shape.setFillColor(color);
    shape.setPosition(pos);
    shape.setOrigin(shape.getSize() * 0.5f); // The center of the rectangle
}

int main()
{
    srand(time(0));

    RenderWindow window(sf::VideoMode(350, 450), "Advanced Tetris"); //(VideoMode(320, 480), "The Game!");

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");
    Sprite s(t1), background(t2), frame(t3);

    int dx = 0;
    bool rotate = 0;
    int colorNum = 1;
    float timer = 0, delay = 0.3;

    Clock clock;

    window.setFramerateLimit(10);
    // sf::WindowHandle handle = window.getSystemHandle();
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-M.ttf"))
    {
        window.setTitle("Font Error");
    }

    // set score title
    sf::Text scoreTitle;
    // select the font
    scoreTitle.setFont(font);
    // set the score title
    scoreTitle.setString("SCORE");
    // set the character size
    scoreTitle.setCharacterSize(24);
    // set the color
    scoreTitle.setFillColor(sf::Color::Red);
    scoreTitle.setPosition(240, 15);

    // set score text
    sf::Text strScore;
    // select the font
    strScore.setFont(font);
    // set the score
    int score = 0;
    strScore.setString(std::to_string(score));
    // set the character size
    strScore.setCharacterSize(24);
    // set the color
    strScore.setFillColor(sf::Color::Red);
    strScore.setPosition(240, 30);

    // set cancelled title
    sf::Text cancTitle;
    // select the font
    cancTitle.setFont(font);
    // set the cancelled titel
    cancTitle.setString("CANCELLED");
    // set the character size
    cancTitle.setCharacterSize(24);
    // set the color
    cancTitle.setFillColor(sf::Color::Red);
    cancTitle.setPosition(240, 45);

    // set cancelled text
    sf::Text cancScore;
    // select the font
    cancScore.setFont(font);
    // set the cancelled
    int canc = 0;
    cancScore.setString(std::to_string(canc));
    // set the character size
    cancScore.setCharacterSize(24);
    // set the color
    cancScore.setFillColor(sf::Color::Red);
    cancScore.setPosition(240, 60);

    // set level title
    sf::Text levelTitle;
    // select the font
    levelTitle.setFont(font);
    // set the level title
    levelTitle.setString("LEVEL");
    // set the character size
    levelTitle.setCharacterSize(24);
    // set the color
    levelTitle.setFillColor(sf::Color::Red);
    levelTitle.setPosition(240, 75);

    // set level text
    sf::Text strLevel;
    // select the font
    strLevel.setFont(font);
    // set the score
    int level = 1;
    strLevel.setString(std::to_string(level));
    // set the character size
    strLevel.setCharacterSize(24);
    // set the color
    strLevel.setFillColor(sf::Color::Red);
    strLevel.setPosition(240, 90);

    //main playing area container
    sf::RectangleShape container(sf::Vector2f(203, 400));
    container.setFillColor(sf::Color::White);
    container.setPosition(25, 30);

    //enemy and player
    sf::Vector2f enemyStartPos = sf::Vector2f(45, 15);
    sf::RectangleShape enemyRect(sf::Vector2f(15, 15));
    initShape(enemyRect, enemyStartPos, sf::Color::Red);
    sf::Vector2f playerStartPos = sf::Vector2f(90, 15);
    sf::RectangleShape playerRect(sf::Vector2f(15, 15));
    initShape(playerRect, playerStartPos, sf::Color::Blue);

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Up)
                    rotate = true;
                else if (e.key.code == Keyboard::Left)
                    dx = -1;
                else if (e.key.code == Keyboard::Right)
                    dx = 1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
            delay = 0.05;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Insert))
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
        // Enemy always moving
        enemyRect.move(1, 0);
        // Enemy got you
        if (playerRect.getGlobalBounds()
                .intersects(enemyRect.getGlobalBounds()))
        {
            enemyRect.setPosition(enemyStartPos);
        }
        if (playerRect.getPosition().x >= 450)
        {
            level++;
            strLevel.setString(std::to_string(level));
            enemyRect.setPosition(enemyStartPos);
            playerRect.setPosition(playerStartPos);
        }

        //// <- Move -> ///
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!check())
            for (int i = 0; i < 4; i++)
                a[i] = b[i];

        //////Rotate//////
        if (rotate)
        {
            Point p = a[1]; //center of rotation
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check())
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
        }

        ///////Tick//////

        if (timer > delay)
        {
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check())
            {
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + rand() % 7;
                int n = rand() % 7;
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }

            timer = 0;
        }

        ///////check lines//////////

        int k = M - 1;
        for (int i = M - 1; i > 0; i--)
        {
            int count = 0;
            for (int j = 0; j < N; j++)
            {
                if (field[i][j])
                    count++;
                field[k][j] = field[i][j];
            }
            if (count < N)
                k--;
        }

        dx = 0;
        rotate = 0;
        delay = 0.3;

        /////////draw//////////
        //window.clear(Color::White);
        window.clear();
        //window.draw(background);
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
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0)
                    continue;
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * 20, i * 20);
                //s.move(28,31); //offset
                s.move(28, 31); //offset
                window.draw(s);
            }

        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(a[i].x * 20, a[i].y * 20);
            s.move(28, 31); //offset
            window.draw(s);
        }

        //window.draw(frame);
        window.display();
    }

    return 0;
}
