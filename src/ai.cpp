
#include "othello.h"
#include "ai.h"

ai::ai()
{
    //
}

ai::~ai()
{
    //
}

//1

int ai::evaluate(OthelloBoard &board, int color) {
    if (board.terminalState()) {
        return 100000*utility(board, color);
    }

    if (board.discsOnBoard <= 20) {
        // Opening game
        return 5*mobility(board, color)
            + 5*potentialMobility(board, color)
            + 20*squareWeights(board, color)
            + 10000*corners(board, color)
            + 10000*stability(board, color);
    }
    else if (board.discsOnBoard <= 58) {
        // Midgame
        return 10*discDifference(board, color)
            + 2*mobility(board, color)
            + 2*potentialMobility(board, color)
            + 10*squareWeights(board, color)
            + 100*parity(board)
            + 10000*corners(board, color)
            + 10000*stability(board, color);
    }
    else {
        // Endgame
        return 500*discDifference(board, color)
            + 500*parity(board)
            + 10000*corners(board, color)
            + 10000*stability(board, color);
    }
}//1

//2

int ai::utility(OthelloBoard &board, int &color) {
    int util = std::accumulate(board.positions.begin(),
            board.positions.end(), 0);

    if (color == AI_BLACK) {
        return util; 
    }
    else {
        return (AI_WHITE * util);
    }
}//2

//3
// Relative disc difference between the two players
int ai::discDifference(OthelloBoard &board, int &color) {
    int blackCount = std::count(board.positions.begin(),
            board.positions.end(), AI_BLACK);
    int whiteCount = std::count(board.positions.begin(),
            board.positions.end(), AI_WHITE);

    if (color == AI_BLACK) {
        return 100 * (blackCount - whiteCount) / (blackCount + whiteCount);
    }
    else {
        return 100 * (whiteCount - blackCount) / (blackCount + whiteCount);
    }
}//3

//4
// Number of possible moves
int ai::mobility(OthelloBoard &board, int &color) {
    //int x = 0, y = 0;
    std::list<int> flippedDiscs;
    //
    board.findLegalMoves(AI_BLACK, &pMoves);
    int blackMoves = pMoves.size();
    //Debug start
    //std::cout << "Black:" << blackMoves << std::endl;
    //for (auto keyval : pMoves) {
    //    //index2coord(keyval.first, colNum, rowNum);
    //    //std::cout << "\t" << moveNum++ << "\t" << colCoord[colNum] << rowCoord[rowNum];
    //    flippedDiscs = keyval.second;
    //    std::cout << keyval.first;
    //    x = keyval.first % 8;
    //    y = keyval.first / 8;
    //    std::cout << " x:" << x << " y:" << y << " will flip: ";
    //    for (int n : flippedDiscs) {
    //        std::cout << n << ' ';
    //    }
    //    //for (int disc : flippedDiscs) {
    //    //    //index2coord(disc, colNum, rowNum);
    //    //    //std::cout << colCoord[colNum] << rowCoord[rowNum] << " ";
    //    //}
    //    std::cout << std::endl;
    //}
    //std::cout << std::endl;
    //Debug ends
    pMoves.clear();

    board.findLegalMoves(AI_WHITE, &pMoves);
    int whiteMoves = pMoves.size();
    //Debug start
    //std::cout << "White:" << whiteMoves << std::endl;
    //for (auto keyval : pMoves) {
    //    //index2coord(keyval.first, colNum, rowNum);
    //    //std::cout << "\t" << moveNum++ << "\t" << colCoord[colNum] << rowCoord[rowNum];
    //    flippedDiscs = keyval.second;
    //    std::cout << keyval.first;
    //    x = keyval.first % 8;
    //    y = keyval.first / 8;
    //    std::cout << " x:" << x << " y:" << y << " will flip: ";
    //    for (int n : flippedDiscs) {
    //        std::cout << n << ' ';
    //    }
    //    //for (int disc : flippedDiscs) {
    //    //    //index2coord(disc, colNum, rowNum);
    //    //    //std::cout << colCoord[colNum] << rowCoord[rowNum] << " ";
    //    //}
    //    std::cout << std::endl;
    //}
    //std::cout << std::endl;
    //Debug ends
    pMoves.clear();

    if (color == AI_BLACK) {
        return 100 * (blackMoves - whiteMoves) / (blackMoves + whiteMoves + 1);
    }
    else {
        return 100 * (whiteMoves - blackMoves) / (blackMoves + whiteMoves + 1);
    }
}//4

//5

int ai::potentialMobility(OthelloBoard &board, int color) {
    int myPotentialMobility = playerPotentialMobility(board, color);
    int opponentPotentialMobility = playerPotentialMobility(board, -color);

    return 100 * (myPotentialMobility - opponentPotentialMobility)
        / (myPotentialMobility + opponentPotentialMobility + 1);
}//5

//6

int ai::playerPotentialMobility(OthelloBoard &board, int color) {
    std::vector<int> boardInterior = {18, 19, 20, 21,
                                      26, 27, 28, 29,
                                      34, 35, 36, 37,
                                      42, 43, 44, 45};

    int here = 0, up = 0, down = 0, left = 0, right = 0,
        upperLeft = 0, upperRight = 0, lowerLeft = 0, lowerRight = 0;
    int potentialMobility = 0;

    for (int square : boardInterior) {
        here = board.positions[square];
        up = board.positions[square-8];
        down = board.positions[square+8];
        left = board.positions[square-1];
        right = board.positions[square+1];
        upperLeft = board.positions[square-9];
        upperRight = board.positions[square-7];
        lowerLeft = board.positions[square+7];
        lowerRight = board.positions[square+9];

        if (here == -color && up == 0)
            potentialMobility++;
        if (here == -color && down == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
        if (here == -color && upperLeft == 0)
            potentialMobility++;
        if (here == -color && upperRight == 0)
            potentialMobility++;
        if (here == -color && lowerLeft == 0)
            potentialMobility++;
        if (here == -color && lowerRight == 0)
            potentialMobility++;
    }

    std::vector<int> topRow = {10, 11, 12, 13};
    for (int square : topRow) {
        here = board.positions[square];
        left = board.positions[square-1];
        right = board.positions[square+1];
        if (here == -color && left == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
    }

    std::vector<int> bottomRow = {50, 51, 52, 53};
    for (int square : bottomRow) {
        here = board.positions[square];
        left = board.positions[square-1];
        right = board.positions[square+1];
        if (here == -color && left == 0)
            potentialMobility++;
        if (here == -color && right == 0)
            potentialMobility++;
    }

    std::vector<int> leftColumn = {17, 25, 33, 41};
    for (int square : leftColumn) {
        here = board.positions[square];
        up = board.positions[square-8];
        down = board.positions[square+8];
        if (here == -color && up == 0)
            potentialMobility++;
        if (here == -color && down == 0)
            potentialMobility++;
    }

    std::vector<int> rightColumn = {22, 30, 38, 46};
    for (int square : leftColumn) {
        here = board.positions[square];
        up = board.positions[square-8];
        down = board.positions[square+8];
        if (here == -color && up == 0)
            potentialMobility++;
        if (here == -color && down == 0)
            potentialMobility++;
    }

    return potentialMobility;
}//6

//7
// Computes a lower bound on the number of stable discs
int ai::stability(OthelloBoard &board, int color) {
    stableDiscs.clear();

    stableDiscsFromCorner(board, 0, color);
    stableDiscsFromCorner(board, 7, color);
    stableDiscsFromCorner(board, 56, color);
    stableDiscsFromCorner(board, 63, color);

    int myStables = stableDiscs.size();

    stableDiscsFromCorner(board, 0, -color);
    stableDiscsFromCorner(board, 7, -color);
    stableDiscsFromCorner(board, 56, -color);
    stableDiscsFromCorner(board, 63, -color);

    int opponentStables = stableDiscs.size();

    return myStables - opponentStables;
}//7

//8 // Finds the number of stable discs given a corner
void ai::stableDiscsFromCorner(OthelloBoard &board, int corner, int color) {
    bool down, right;
    if (corner == 0) {
        down = true;
        right = true;
    }
    else if (corner == 7) {
        down = true;
        right = false;
    }
    else if (corner == 56) {
        down = false;
        right = true;
    }
    else {
        down = false;
        right = false;
    }

    int horizIncr = 1, horizStop = 7, vertIncr = 8, vertStop = 56;
    if (!right) {
        horizIncr *= -1;
        horizStop *= -1;
    }
    if (!down) {
        vertIncr *= -1;
        vertStop *= -1;
    }

    // Iterate horizontally
    for (int i = corner; i != corner + horizIncr + horizStop; i += horizIncr) {
        // If there is a disc of our color on this square
        if (board.positions[i] == color) {
            // Iterate vertically
            for (int j = i; j != i + vertStop; j+= vertIncr) {
                // If there is a disc of our color on this square,
                // and it is not in the set of stable discs
                if (board.positions[j] == color
                        && stableDiscs.find(j) == stableDiscs.end()) {
                    // Insert it to the set
                    stableDiscs.insert(j);
                }
                // If we come across a disc of the opposite color, break
                else {
                    break;
                }
            }
        }
        // Otherwise, break
        else {
            break;
        }
    }
}//8

//9
int ai::parity(OthelloBoard &board) {
    int squaresRemaining = 64 - board.discsOnBoard;

    if (squaresRemaining % 2 == 0) {
        return -1;
    }
    else {
        return 1;
    }
}//9

//10 // Assigns a weight to every square on the board
int ai::squareWeights(OthelloBoard &board, int &color) {
    std::vector<int> weights = {
         200, -100, 100,  50,  50, 100, -100,  200,
        -100, -200, -50, -50, -50, -50, -200, -100,
         100,  -50, 100,   0,   0, 100,  -50,  100,
          50,  -50,   0,   0,   0,   0,  -50,   50,
          50,  -50,   0,   0,   0,   0,  -50,   50,
         100,  -50, 100,   0,   0, 100,  -50,  100,
        -100, -200, -50, -50, -50, -50, -200, -100,
         200, -100, 100,  50,  50, 100, -100,  200,
    };

    if (board.positions[0] != 0) {
        weights[1]  = 0;
        weights[2]  = 0;
        weights[3]  = 0;
        weights[8]  = 0;
        weights[9]  = 0;
        weights[10] = 0;
        weights[11] = 0;
        weights[16] = 0;
        weights[17] = 0;
        weights[18] = 0;
        weights[24] = 0;
        weights[25] = 0;
    }

    if (board.positions[7] != 0) {
        weights[4]  = 0;
        weights[5]  = 0;
        weights[6]  = 0;
        weights[12] = 0;
        weights[13] = 0;
        weights[14] = 0;
        weights[15] = 0;
        weights[21] = 0;
        weights[22] = 0;
        weights[23] = 0;
        weights[30] = 0;
        weights[31] = 0;
    }

    if (board.positions[56] != 0) {
        weights[32] = 0;
        weights[33] = 0;
        weights[40] = 0;
        weights[41] = 0;
        weights[42] = 0;
        weights[48] = 0;
        weights[49] = 0;
        weights[50] = 0;
        weights[51] = 0;
        weights[57] = 0;
        weights[58] = 0;
        weights[59] = 0;
    }

    if (board.positions[63] != 0) {
        weights[38] = 0;
        weights[39] = 0;
        weights[45] = 0;
        weights[46] = 0;
        weights[47] = 0;
        weights[52] = 0;
        weights[53] = 0;
        weights[54] = 0;
        weights[55] = 0;
        weights[60] = 0;
        weights[61] = 0;
        weights[62] = 0;
    }

    if (color == AI_BLACK) {
        return std::inner_product(board.positions.begin(),
                board.positions.end(), weights.begin(), 0);
    } else {
        return -1*std::inner_product(board.positions.begin(),
                board.positions.end(), weights.begin(), 0);
    }
}//10

//11
int ai::corners(OthelloBoard &board, int &color) {
    std::vector<int> corners = {0, 7, 56, 63};
    int blackCorners = 0;
    int whiteCorners = 0;

    for (int corner : corners) {
        if (board.positions[corner] == AI_BLACK) {
            blackCorners++;
        } else if (board.positions[corner] == AI_WHITE) {
            whiteCorners++;
        }
    }

    if (color == AI_BLACK) {
        return 100 * (blackCorners - whiteCorners)
            / (blackCorners + whiteCorners + 1);
    } else {
        return 100 * (whiteCorners - blackCorners)
            / (blackCorners + whiteCorners + 1);
    }
}//11
