all: main.cpp 
	g++ -o infiltrators main.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
