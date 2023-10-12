all: chess

chess: main.cpp classes/*.cpp
	g++ -g main.cpp classes/*.cpp -o chess

test: test.cpp classes/*.cpp
	g++ -g test.cpp classes/*.cpp -o test

clean:
	rm -f chess