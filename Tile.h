//
// Created by Andrew Laskin on 4/17/23.
//

#ifndef MINESWEEPER_SPRING2023_TILE_H
#define MINESWEEPER_SPRING2023_TILE_H


class Tile {
private:
    int rowNum;
    int colNum;
    int tileValue;
    bool flagBool;
    bool revealed;
public:
    Tile();
    Tile(int row,int colom,int value);
    int getValue();
    void setValue(int value);
    void removeFlag();
    void setFlag();
    bool isFlagged();
    bool isRevealed();
    bool setRevealed();
};


#endif //MINESWEEPER_SPRING2023_TILE_H
