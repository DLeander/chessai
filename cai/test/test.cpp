#include <iostream>

int main() {
    int numArtPieces = 8; // Change this to the number of art pieces you want to print

    for (int i = 0; i < numArtPieces; i++) {
        std::cout << "_ _ _ _ _ _ _ ";
    }
    std::cout << std::endl;

    for (int i = 0; i < numArtPieces; i++) {
        std::cout << "|     	     | ";
    }
    std::cout << std::endl;

    for (int i = 0; i < numArtPieces; i++) {
        std::cout << "|        o     | ";
    }
    std::cout << std::endl;

    for (int i = 0; i < numArtPieces; i++) {
        std::cout << "|       (*)    | ";
    }
    std::cout << std::endl;

    for (int i = 0; i < numArtPieces; i++) {
        std::cout << "|     (***)  | ";
    }
    std::cout << std::endl;

    for (int i = 0; i < numArtPieces; i++) {
        std::cout << "|_ _ _ _ _ _ _ | ";
    }
    std::cout << std::endl;

    return 0;
}
