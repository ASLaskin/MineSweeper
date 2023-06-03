#include "Board.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
using namespace std;

Board::Board(int rows, int coloms,int mines) {
    isLost = false;
    isWon = false;
    firstClick = true;
    leaderBoardOpened = false;
    for (int row = 0; row < rows; row++) {
        vector<Tile> temp;
        for (int col = 0; col < coloms; col++) {
                int posX = col;
                int posY = row;
                Tile newTile(posY,posX,0);
                temp.push_back(newTile);
        }
        tiles.push_back(temp);
    }
   vector<int> minePlaced = generateMines(rows * coloms,mines);
    int b = 0;
    for (int row = 0; row < rows; row++){
        for (int col = 0; col < coloms; col++){
            if (minePlaced[b] == 9){
                tiles[row][col].setValue(9);
            }
            b++;
        }

    }

}

Tile Board::getTileAt(int i, int j) {
    return tiles[i][j];
};

vector<int> Board::generateMines(int tiles, int mines){
    vector<int> vec(tiles, 0);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, tiles - 1);


    for (int i = 0; i < mines; ++i) {
        int c;
        do {
            c = distrib(gen);
        } while (vec[c] == 9);
        vec[c] = 9;
    }
    return vec;
}
vector<vector<sf::Sprite>> Board::generateSpriteList(int rows, int coloms) {
    vector<vector<sf::Sprite>> sprites;
    for (int row = 0; row < tiles.size();row++) {
        vector<sf::Sprite> rowVec;
        for (int col = 0; col < coloms;col++) {
            sf::Sprite tileSprite;
            tileSprite.setPosition(col*32,row*32);
            rowVec.push_back(tileSprite);
        }
        sprites.push_back(rowVec);
    }
    return sprites;
}
void Board::generateNums(int rows, int cols) {
    vector<vector<int>> mineCount;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if (tiles[i][j].getValue() == 0){
                int count = 0;
                for (int x = -1; x <= 1; ++x) {
                    for (int y = -1; y <= 1; ++y) {
                        int adRow = i + x;
                        int adCol = j + y;
                        if (adRow >= 0 && adRow< tiles.size() && adCol >= 0 && adCol < cols
                        && tiles[adRow][adCol].getValue() == 9){
                            count++;
                        }
                    }
                }
                tiles[i][j].setValue(count);
            }
            else {
                tiles[i][j].setValue(9);
            }
        }
    }
}
void Board::bangReveal(int row,int col, int numRows,int numCols) {
    vector<int> checkList= {row - 1, col - 1, row, col - 1, row + 1, col - 1,row - 1, col, row + 1, col,row - 1, col + 1, row, col + 1, row + 1, col + 1};
    tiles[row][col].setRevealed();
    for(int i = 0; i < checkList.size(); i += 2) {
        if(0 <= checkList[i] && checkList[i] < numRows && 0 <= checkList[i + 1] && checkList[i + 1] < numCols and !tiles[checkList[i]][checkList[i + 1]].isFlagged() ) {
            if (tiles[checkList[i]][checkList[i + 1]].getValue() == 0 && !tiles[checkList[i]][checkList[i + 1]].isRevealed()) {
                bangReveal(checkList[i], checkList[i+1], numRows, numCols);
            }
            if(tiles[checkList[i]][checkList[i + 1]].getValue() != 9 and !tiles[checkList[i]][checkList[i + 1]].isFlagged() ) {
                tiles[checkList[i]][checkList[i + 1]].setRevealed();
            }
        }
    }
}













