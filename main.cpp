#include <iostream>
#include <ctime>
#include <cmath>

namespace {
    class Minesweeper {
    public:
        Minesweeper(const size_t width, const size_t height)
                : width(width), height(height),
                  table(new char[width * height]),
                  visible_table(new char[width * height]) {
            //fillTable();
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


        void printVerticalDelimiter() const {
            std::cout << std::string(ROW_NR_COMPENSATION, ' ');
            std::cout << ascii3wayVerticalRight;
            for (int i = 0; i < width; ++i) {
                std::cout << std::string(3, asciiHorizontal);
                if((i+1) % width !=0) std::cout << ascii4way;
            }
            std::cout << ascii3wayVerticalLeft << std::endl;

        }

        void printBottomBorder() const {
            std::cout << std::string(ROW_NR_COMPENSATION, ' ');
            std::cout << asciiBottomLeftCorner;
            for (int i = 0; i < width; ++i) {
                std::cout << std::string(3, asciiHorizontal);
                if((i+1) % width !=0) std::cout << ascii3wayHorizontalUp;
            }
            std::cout << asciiBootomRightCorner << std::endl;
            std::cout << std::string(ROW_NR_COMPENSATION + 1, ' ');
            for (int j = 1; j <= width ; ++j) {
                std::cout.width(4); std::cout << std::left << j;
            }
            std::cout << "\n";
        }

        void printTable() const {
            // step 3 goes here
            std::cout << "\n";
            printTopBorder();
            for (int j = 0, row = 1; j < (height*width) ; ++j) {
                if(0 == j % width) {
                    std::cout.width(4); std::cout << std::right << row;
                }
                std::cout << asciiVertical << " " << *(table + j) << " " ;
                if(0 == (j+1) % width) {
                    std::cout << asciiVertical;
                    std::cout.width(4); std::cout << std::left << row;
                    std::cout << "\n";
                    ++row;
                    if(j+1 == (height * width)) printBottomBorder();
                    else printVerticalDelimiter();
                }
            }
        }

        void printTopBorder() const {
            std::cout << std::string(ROW_NR_COMPENSATION + 1, ' ');
            for (int j = 1; j <= width ; ++j) {
                std::cout.width(4); std::cout << std::left << j;
            }
            std::cout << "\n";
            std::cout << std::string(ROW_NR_COMPENSATION, ' ');
            std::cout << asciiTopLeftCorner;
            for (int i = 0; i < width; ++i) {
                std::cout << std::string(COLUMN_WIDTH, asciiHorizontal);
                if((i+1) % width !=0) std::cout << ascii3wayHorizontalDown;
            }
            std::cout << asciiTopRightCorner << std::endl;
        }


        void printVisibleTable() const {
            std::cout << "\n";
            printTopBorder();
            for (int j = 0, row = 1; j < (height*width) ; ++j) {
                if(0 == j % width) {
                    std::cout.width(4); std::cout << std::right << row;
                }
                std::cout << asciiVertical << " " << *(visible_table + j) << " " ;
                if(0 == (j+1) % width) {
                    std::cout << asciiVertical;
                    std::cout.width(4); std::cout << std::left << row;
                    std::cout << "\n";
                    ++row;
                    if(j+1 == (height * width)) printBottomBorder();
                    else printVerticalDelimiter();
                }
            }
        }

        //Doesn't work in CLion, only when running compiled exe outside the IDE. And I know that system() should not be used :)
        void clearScreen() {
            if (system("CLS")) system("clear");
        }

        bool checkVictoryCondition() {
            int counter = 0;
            for (int i = 0; i < (width * height) ; ++i) {
                if(*(visible_table + i) == ' ' && *(table + i) != '*'){
                    ++counter;
                }
            }
            isGameWon = counter == 0;
            return counter == 0;
        }

        bool isPlayerPlayingAgain() {
            char input;
            do
            {
                std::cout << "\nWould you like to play again? [y/n] ";
                std::cin >> input;
            }
            while( !std::cin.fail() && input != 'y' && input != 'n' && input != 'Y' && input != 'N');
            return input == 'Y' || input == 'y';
        }

        void gameLoop() {
            bool newGame = true;
            while(newGame) {
                 clearScreen();
                initGame();
                while (!isGameOver && !isGameWon) {
                    clearScreen();
                    printVisibleTable();
                    isGameOver = handleNextMove(getNextMove());
                    ++nrOfMoves;
                    isGameWon = checkVictoryCondition();
                }
                std::string moveOrMoves;
                (nrOfMoves == 1)  ? moveOrMoves = "move" : moveOrMoves = "moves";
                if(isGameWon) {
                    clearScreen();
                    printTable();
                    std::cout << "\nGame won in " << nrOfMoves << " " << moveOrMoves << " on \"" << chosenDifficulty << "\" difficulty! Congratulations! You get to live, because you didn't step on a mine!\n";
                } else {
                    clearScreen();
                    *(table + fatalMove) = explodedLandmine;
                    printTable();
                    std::cout << "\nAfter " << nrOfMoves << " " << moveOrMoves << ", you stepped on a landmine on \""<< chosenDifficulty <<"\" difficulty and successfully kicked your air addiction...\n";
                }
                newGame = isPlayerPlayingAgain();
            }
        }

        std::string getChosenDifficulty() {
            double mineRatio;
            mineRatio = floor((double(totalNrOfMines) / (double(width) * double(height))) * 10);
            std::cout << "mineRatio: " << mineRatio;
            switch (int(mineRatio)){
                case 0: return "I have a hard time dealing with defeat";
                case 1: return "I'm just here for the story";
                case 2: return "I like a challenge";
                case 3: return "This is actually hard...";
                case 4: return "Hardcore";
                case 5: return "Insane";
                case 6: return "Masochist";
                case 7: return "I love defeat because I hate myself";
                case 8: return "I chose this difficulty as a cry for help";
                case 9: return "Near impossible";
                default: return "Error setting difficulty";
            }
        }

        void initGame() {
            isGameOver = false;
            isGameWon = false;
            nrOfMoves = 0;
            size_t input;
            std::cout << "\nPlease enter the desired width of the playing field (2-" << MAX_WIDTH << "):";
            while(!(std::cin >> input) || input < 2 || input > MAX_WIDTH) {
                std::cout << "input out of range! \n";
                std::cin.clear();
                std::cin.ignore(100, '\n'); //TODO figue out how much to ignore
            }
            this->width = input;

            std::cout << "\nPlease enter the desired height of the playing field (2-" << MAX_HEIGHT << "):";
            while(!(std::cin >> input) || input < 2 || input > MAX_WIDTH) {
                std::cout << "input out of range! \n";
                std::cin.clear();
                std::cin.ignore(100, '\n'); //TODO figue out how much to ignore
            }
            this->height = input;

            std::cout << "\nPlease enter the desired number of mines (1-" << height*width-1 << "):";
            while(!(std::cin >> input) || input < 1 || input > (height*width-1)) {
                std::cout << "input out of range! \n";
                std::cin.clear();
                std::cin.ignore(100, '\n'); //TODO figue out how much to ignore
            }
            this->totalNrOfMines = input;
            fillTable();
            countNeighbours();
            chosenDifficulty = getChosenDifficulty();
        }

    private:
        const char MINE = '*';
        void fillTable() {
            // step 1 goes here
            srand(time(0));

            for (int i = 0; i < (width*height); ++i) {
                *(table + i) = '.';
                *(visible_table + i) = ' ';
            }
            int placedMines = 0;
            while(placedMines < totalNrOfMines) {
                int random = rand() % (width*height);
                char * pRandomSpot;
                pRandomSpot = table;
                if(*(pRandomSpot + random) != MINE) {
                    *(pRandomSpot + random) = MINE;
                    placedMines++;
                }
            }
        }

        void revealWithNeighbouringZeros(int move) {
            *(visible_table + move) = *(table + move);

            bool isAtRightEdge = (move + 1) % width == 0;
            bool isAtLeftEdge = move % width == 0;
            char * pPosition = table + move;
            char * pVisiblePos = visible_table + move;
            if(*pPosition == '0'){

                // check above
                if((move - width) >= 0 &&
                    *(pVisiblePos -width) != '0') revealWithNeighbouringZeros(move - width);

                // check above to the right
                if((move - width + 1) > 0 && !isAtRightEdge &&
                        *(pVisiblePos - width + 1) != '0') revealWithNeighbouringZeros(move - width + 1);

                // check right
                if((move + 1) < (height*width) && !isAtRightEdge &&
                        *(pVisiblePos + 1) != '0') revealWithNeighbouringZeros(move +1);

                // check below to the right
                if((move + width + 1) < (height*width) && !isAtRightEdge &&
                        *(pVisiblePos + width + 1) != '0') revealWithNeighbouringZeros(move + width + 1);

                // check below
                if((move + width) < (height*width) &&
                        *(pVisiblePos + width) != '0') revealWithNeighbouringZeros(move + width);

                // check below to the left
                if((move + width - 1) < (height*width) && !isAtLeftEdge &&
                        *(pVisiblePos + width - 1) != '0') revealWithNeighbouringZeros(move + width - 1);

                // check left
                if((move - 1) >= 0 && !isAtLeftEdge &&
                        *(pVisiblePos - 1) != '0') revealWithNeighbouringZeros(move - 1);

                //check above to the left
                if((move - width - 1) >= 0 && !isAtLeftEdge &&
                        *(pVisiblePos - width -1) != '0') revealWithNeighbouringZeros(move - width - 1);
            }
        }

        bool handleNextMove(int move) {
            revealWithNeighbouringZeros(move);
            if(*(visible_table + move) == '*'){
                fatalMove = move;
            }
            return *(visible_table + move) == '*';
        }

        int getNextMove() {
            bool isMoveValid = false;
            int target = 0;
            int input;
            while(!isMoveValid){
                std::cout << "\nPlease enter X position of target(1-" << width << "):";
                while(!(std::cin >> input) || input < 1 || input > width) {
                    std::cout << "input out of range! \n";
                    std::cin.clear();
                    std::cin.ignore(100, '\n'); //TODO figue out how much to ignore
                }
                target = (input-1);

                std::cout << "\nPlase enter Y position of target(1-" << height << "):";
                while(!(std::cin >> input) || input < 1 || input > height) {
                    std::cout << "input out of range! \n";
                    std::cin.clear();
                    std::cin.ignore(100, '\n'); //TODO figue out how much to ignore
                }
                target += (input-1) * width;

                if(*(visible_table + target) != ' ') {
                    std::cout << "Target coordinates already revealed.\n";
                } else isMoveValid = true;
            }
            return target;
        }


        const char asciiTopLeftCorner = 201;
        const char asciiTopRightCorner = 187;
        const char asciiBottomLeftCorner = 200;
        const char asciiBootomRightCorner = 188;
        const char asciiHorizontal = 205;
        const char asciiVertical = 186;
        const char ascii3wayHorizontalDown = 203;
        const char ascii3wayHorizontalUp = 202;
        const char ascii3wayVerticalLeft = 185;
        const char ascii3wayVerticalRight = 204;
        const char ascii4way = 206;
        const char explodedLandmine = 177;
        const int COLUMN_WIDTH = 3;
        const int ROW_NR_COMPENSATION = 4;
        const int MAX_WIDTH = 30; //display breaks in Clion above 20
        const int MAX_HEIGHT = 30; //display breaks in Clion above 20
        size_t width, height;
        char *table;
        char *visible_table;
        int totalNrOfMines = 5;
        bool isGameWon = false;
        bool isGameOver = false;
        int nrOfMoves = 0;
        int fatalMove;
        std::string chosenDifficulty;
    };
}

int main() {
    try {
        Minesweeper ms( 30, 30);
        //ms.initGame();
        //ms.printTable();
        //ms.countNeighbours();
        // ms.printTable();
        //ms.printVisibleTable();
        ms.gameLoop();
    } catch (const std::bad_alloc &e) {
        std::cerr << "Couldn't allocate enough memory for minesweeper table" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
