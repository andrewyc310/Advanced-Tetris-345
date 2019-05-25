# Advanced-Tetris-345
Download all the files except those in others folder\n
Run terminal and excute following 3 commands:\n
g++ -c main.cpp -ISFML/include
g++ main.o -o game -LSFML/lib -lsfml-graphics -lsfml-window -lsfml-system
export LD_LIBRARY_PATH=SFML/lib && ./game
