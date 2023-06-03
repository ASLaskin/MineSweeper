//
// Created by Andrew Laskin on 4/17/23.
//
#include <vector>
#include "Tile.h"
#include <SFML/Graphics.hpp>

#ifndef MINESWEEPER_SPRING2023_BOARD_H
#define MINESWEEPER_SPRING2023_BOARD_H

using namespace std;
class Board {

public:
    bool isWon;
    bool isLost;
    bool firstClick;
    bool leaderBoardOpened;
    vector<vector<Tile>>tiles;
    Board(int rows,int coloms,int mines);
    Tile getTileAt(int i, int j);
    vector<int> generateMines(int tiles,int mines);
    vector<vector<sf::Sprite>> generateSpriteList(int rows,int coloms);
    void generateNums(int rows, int cols);
    void bangReveal(int rows,int cols,int numRows, int numCols);
};




#endif //MINESWEEPER_SPRING2023_BOARD_H
