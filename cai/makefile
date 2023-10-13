all: chess

chess: main.cpp classes/*.cpp Ai.cpp
	g++ -g main.cpp classes/*.cpp Ai.cpp -o chess

test: test.cpp classes/*.cpp Ai.cpp
	g++ -g test.cpp classes/*.cpp Ai.cpp -o test

clean:
	rm -f chess