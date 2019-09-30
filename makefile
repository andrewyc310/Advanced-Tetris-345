main: main.cpp
	g++ -c main.cpp -ISFML/include
	g++ main.o -o game -LSFML/lib -lsfml-graphics -lsfml-window -lsfml-system
	export LD_LIBRARY_PATH=SFML/lib && ./game
