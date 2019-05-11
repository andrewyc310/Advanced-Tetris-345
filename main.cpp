#include <SFML/Graphics.hpp>

void initShape(sf::RectangleShape &shape, sf::Vector2f const &pos, sf::Color const &color) {
	shape.setFillColor(color);
	shape.setPosition(pos);
	shape.setOrigin(shape.getSize() * 0.5f); // The center of the rectangle
}

int main() {
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Advanced Tetris");
    window.setFramerateLimit(10);
    // sf::WindowHandle handle = window.getSystemHandle();
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-M.ttf")) {
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
    scoreTitle.setPosition(800,50);

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
    strScore.setPosition(800,100);

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
    cancTitle.setPosition(800,150);

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
    cancScore.setPosition(800,200);

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
    levelTitle.setPosition(800,250);

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
    strLevel.setPosition(800,300);

    //main playing area container
    sf::RectangleShape container(sf::Vector2f(330, 600));
    container.setFillColor(sf::Color::White);
    container.setPosition(168,150);

    //enemy and player
    sf::Vector2f enemyStartPos = sf::Vector2f(150, 50);
    sf::RectangleShape enemyRect(sf::Vector2f(50, 50));
    initShape(enemyRect, enemyStartPos, sf::Color::Red);
    sf::Vector2f playerStartPos = sf::Vector2f(300, 50);
    sf::RectangleShape playerRect(sf::Vector2f(50, 50));
    initShape(playerRect, playerStartPos, sf::Color::Blue);

    // run the program as long as the window is open  
    while (window.isOpen())
    {
      // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // check the type of the event...
            switch (event.type) {
                // window closed
                case sf::Event::Closed:
                    window.close();
                    break;
                // we don't process other types of events
                default:
                    break;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            //moveLeft
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            //moveRight
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            //rotate();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            //get down
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Insert)) {
			/* for test only, press insert to get score,
				 cancellation, move player*/
            enemyRect.move(-1, 0);
            playerRect.move(1,0);
            score += 10;
            strScore.setString(std::to_string(score));
            canc++;
            cancScore.setString(std::to_string(canc));
        }
        // Enemy always moving
        enemyRect.move(1, 0); 
        // Enemy got you
        if (playerRect.getGlobalBounds()
	    .intersects(enemyRect.getGlobalBounds())) {
            enemyRect.setPosition(enemyStartPos);
        }
        if(playerRect.getPosition().x >= 450) {
            level++;
            strLevel.setString(std::to_string(level));
            enemyRect.setPosition(enemyStartPos);
            playerRect.setPosition(playerStartPos);
        }

        //Render frame
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
        window.display();
    }

    return 0;
}
