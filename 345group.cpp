#include "pch.h"
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
using namespace std;
using namespace sf;


const int M = 20;//horizont Block size
const int N = 10;//verticle block size

//define a 20*10 playfield
int field[M][N] = {0};

//defind a contructor Point to store coordinates
//a[4] and b[4] stores coordinates of each tetris which containing 4 blocks.
struct Point
{int x,y;} a[4], b[4];

//define 7 tetris shapes
int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

//check method
bool check()
{
   for (int i=0;i<4;i++)
	   //check if tetris go over the left, right, and bottom boundaries
	  if (a[i].x<0 || a[i].x>=N || a[i].y>=M) return 0;
	//check if tetris has collision with other tetris
      else if (field[a[i].y][a[i].x]) return 0;

   return 1;
};

void initShape(sf::RectangleShape &shape, sf::Vector2f const &pos, sf::Color const &color) {
	shape.setFillColor(color);
	shape.setPosition(pos);
	shape.setOrigin(shape.getSize() * 0.5f); // The center of the rectangle
}



int main()
{
  srand(time(0));	 

	RenderWindow window(sf::VideoMode(1024, 1000), "Advanced Tetris");

  Texture t1,t2,t3;
	t1.loadFromFile("images/ntiles.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame.png");
	Sprite s(t1), background(t2), frame(t3);

//define x coordinate of blocks
  int distance_x=0;
	//define if rotate or not
  bool is_rotate=0; 
	//define color index of image
  int color_index=2;
	//define time
float timer=0;
	 //define speed for gravity
  float delay=0.3;

	Clock clock;


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
    sf::RectangleShape container(sf::Vector2f(411, 830));
    container.setFillColor(sf::Color::White);
    container.setPosition(179,130);

    //enemy and player
    sf::Vector2f enemyStartPos = sf::Vector2f(150, 50);
    sf::RectangleShape enemyRect(sf::Vector2f(50, 50));
    initShape(enemyRect, enemyStartPos, sf::Color::Red);
    sf::Vector2f playerStartPos = sf::Vector2f(300, 50);
    sf::RectangleShape playerRect(sf::Vector2f(50, 50));
    initShape(playerRect, playerStartPos, sf::Color::Blue);

    while (window.isOpen())
    {
	    	//to retrieve the time elapsed since the clock started, per second
		float time = clock.getElapsedTime().asSeconds();
	    	//restart the clock.
		clock.restart();
	    	//update time
		timer+=time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
			//check if any key has been pressed
			if (e.type == Event::KeyPressed)
				//when up key pressed, use is_rotate to triggle the rotation method
			  if (e.key.code==Keyboard::Up) is_rotate=true;
				//when left key pressed, move object to the 1 distance left
			  else if (e.key.code==Keyboard::Left) distance_x=-1;
				//when right key pressed, move object to the 1 distance right
			  else if (e.key.code==Keyboard::Right) distance_x=1;
		}
	//if down key pressed, accelerate the gravity
	if (Keyboard::isKeyPressed(Keyboard::Down)) delay=0.05;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Insert)) {
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

	//left and right movement
	for (int i=0;i<4;i++)  { 
    b[i]=a[i]; 
    a[i].x+=distance_x;
  }
  if (!check()){
    for (int i=0;i<4;i++) {
      a[i]=b[i];
    }
  }

	//rotation
	if (is_rotate){
		//center of rotation
		Point p = a[1]; 
		//do the rotation
		for (int i=0;i<4;i++){
			int x = a[i].y-p.y;
			int y = a[i].x-p.x;
			a[i].x = p.x - x;
			a[i].y = p.y + y;
			}
		if (!check()) {
	      for (int i=0;i<4;i++) {
		a[i]=b[i];
	      }
	    }
	}

	///////Tick//////
	int cc[4];
	if (timer>delay){
	    for (int i=0;i<4;i++) { 
        b[i]=a[i]; 
        a[i].y+=1; 
      }

      if (!check()){
       //for (int i=0;i<4;i++) field[b[i].y][b[i].x]=color_index;
        for (int i=0;i<4;i++) {
          field[b[i].y][b[i].x]=cc[i];
        }
        color_index=1+rand()%5;//update color index from 1 to 6
        
        //initialise different tetris, n ranged from 0-6
        int n=rand()%7;
        for (int i=0;i<4;i++){
            a[i].x = figures[n][i] % 2;
            a[i].y = figures[n][i] / 2;
        }
      }
	  	timer=0;
	  }

	//elimination
  int k=M-1;
	for (int i=M-1;i>0;i--){
		int count=0;
		for (int j=0;j<N;j++){
		    if (field[i][j]) {
          count++;
        }
		    field[k][j]=field[i][j];
		}
		if (count<N) {
      k--;
    }
	}

  distance_x=0; 
  is_rotate=0; 
  delay=0.3;

  //drawing elements
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
		  
	for (int i=0;i<M;i++)
	 for (int j=0;j<N;j++)
	   {
         if (field[i][j]==0) {
           continue;
         }
		 s.setTextureRect(IntRect(field[i][j]*100,40,40,40));
		 s.setPosition(j*41,i*41);
		 s.move(180, 140); //offset, the point generate a new shape
		 window.draw(s);
	   }

	for (int i=0;i<4;i++)
	  {
        cc[i] = color_index;
          if(i==0){
            if(cc[i]<=2){
              cc[i]+=2;
            }else{
              cc[i]--;
            }
          }else if(i==2){
            if(cc[i]>=2){
              cc[i]--;
            }else{
              cc[i]+=4;
            }
          }else if(i==1){
            if(cc[i]<=3){
            cc[i]+=3;
            }
          }else if(i==3){
            if(cc[i]>=3){
            cc[i]-=2;
            }else{
              cc[i]+=4;
            }
          }
		
    s.setTextureRect(IntRect(cc[i]*100,40,40,40));
		s.setPosition(a[i].x*41,a[i].y*41);
		s.move(180,140); //offset,the point generate a new shape
		window.draw(s);
	  }

	//window.draw(frame);
 	window.display();
	}

    return 0;
}
