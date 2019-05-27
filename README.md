# Advanced-Tetris-345
Download all the files except those in others folder.
Run terminal then navigate to your download position and excute following 3 commands:
g++ -c main.cpp -ISFML/include


g++ main.o -o game -LSFML/lib -lsfml-graphics -lsfml-window -lsfml-system


export LD_LIBRARY_PATH=SFML/lib && ./game

To run the program in Xcode, extract AdvancedTetrisAlpha.zip and open Advanced Tetris Alpha.xcodeproj. Then click run in Xcode.

