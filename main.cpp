#include <iostream>
#include <ctime>

namespace {
    class Minesweeper {
    public:
        Minesweeper(const size_t width, const size_t height)
                : width(width), height(height),
                  table(new char[width * height]) {
            fillTable();
        }

        /* In a real implementation there would also be a
         * - copy constructor
         * - assignment operator
         * - move constructor
         * - move assignment operator
         * We will learn about them later
         */

        virtual ~Minesweeper() {
            delete[] table;
        }

        void countNeighbours() {
            // step 2 goes here
            char counter;
            char * pField;
            bool isAtRightEdge;
            bool isAtLeftEdge;
            for (int i = 0; i < (height*width) ; ++i) {
                counter = '0';
                isAtRightEdge = (i + 1) % width == 0;
                isAtLeftEdge = i % width == 0;
                pField = table+i;
                if(*pField != MINE){
                    if(*(pField-width) == MINE) counter++;
                    if(*(pField-width+1) == MINE && !isAtRightEdge) counter++;
                    if(*(pField+1) == MINE && !isAtRightEdge) counter++;
                    if(*(pField+width+1) == MINE && !isAtRightEdge) counter++;
                    if(*(pField+width) == MINE) counter++;
                    if(*(pField+width-1) == MINE && !isAtLeftEdge) counter++;
                    if(*(pField-1) == MINE && !isAtLeftEdge) counter++;
                    if(*(pField-width-1) == MINE && !isAtLeftEdge) counter++;
                    *pField = counter;
                }
            }
        }

        void printTable() const {
            // step 3 goes here
            std::cout << "\n";
            for (int j = 0; j < (height*width) ; ++j) {
                std::cout << *(table + j) << " ";
                if(0 == (j+1) % width) {
                    std::cout << "\n";
                }
            }

        }

    private:
        int totalNrOfMines = 15;
        const char MINE = '*';
        void fillTable() {
            // step 1 goes here
            srand(time(0));

            for (int i = 0; i < (width*height); ++i) {
                *(table + i) = '.';
            }
            int placedMines = 0;
            while(placedMines < totalNrOfMines) {
                int random = rand() % (width*height);
                char * pRandomSpot;
                pRandomSpot = table;
                //std::cout << "pointer reset to table: " << pRandomSpot << std::endl;
                if(*(pRandomSpot + random) != MINE) {
                    //std::cout << *(pRandomSpot + random) << std::endl;
                    *(pRandomSpot + random) = MINE;
                    placedMines++;
                    // std::cout << "placed mines: "<< placedMines << std::endl;
                }
                // std::cout << "random: "<< random << std::endl;
            }
        }

        const size_t width, height;
        char *table;
    };
}

int main() {
    try {
        Minesweeper ms( 10, 10);
        ms.printTable();
        ms.countNeighbours();
        ms.printTable();
    } catch (const std::bad_alloc &e) {
        std::cerr << "Couldn't allocate enough memory for minesweeper table" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
