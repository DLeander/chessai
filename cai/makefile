all: chess test

chess: main.cpp classes/*.cpp Ai.cpp
	g++ -g -std=c++11 main.cpp classes/*.cpp Ai.cpp -o chess

test: test.cpp classes/*.cpp Ai.cpp
	g++ -g -std=c++11 test.cpp classes/*.cpp Ai.cpp -o test

clean:
	rm -f chess test