# Advanced-Tetris-345
Download all the files except those in others folder.

Install SFML for macos:
Uncompress SFML-2.5.1-macOS-clang.tar.gz

Header files and libraries
SFML is available either as dylibs or as frameworks. Only one type of binary is required although both can be installed simultaneously on the same system. We recommend using the frameworks.
    frameworks
Copy the content of Frameworks to /Library/Frameworks.
    dylib
Copy the content of lib to /usr/local/lib and copy the content of include to /usr/local/include.
SFML dependencies
SFML depends on a few external libraries on macOS. Copy the content of extlibs to /Library/Frameworks.

How to run:
Unzip Advanced Tetris.app.zip
Run the program by simply double click on Advanced Tetris.app

To check that our source code matches our program:
Run terminal then navigate to your download position and excute following 3 commands:
g++ -c main.cpp -ISFML/include

g++ main.o -o game -LSFML/lib -lsfml-graphics -lsfml-window -lsfml-system

export LD_LIBRARY_PATH=SFML/lib && ./game

(These commands are given by the SFML official webiste https://www.sfml-dev.org/tutorials/2.5/start-linux.php for Linux
When use these on Mac, the menu doesn't work properly. Therefore, it's only used for test purpose.)
