//
// Created by Andrew Laskin on 4/17/23.
//

#include "Tile.h"
Tile::Tile()= default;

Tile::Tile(int row, int colom, int value){
    rowNum = row;
    colNum = colom;
    tileValue = value;
    flagBool = false;
    revealed = false;
}

int Tile::getValue() {
    return tileValue;
}
void Tile::setValue(int value) {
    tileValue = value;
}

void Tile::setFlag(){
    flagBool = true;
}
void Tile::removeFlag(){
    flagBool = false;
}
bool Tile::isFlagged(){
    return flagBool;
}
bool Tile::setRevealed(){
    revealed = true;
}

bool Tile::isRevealed() {
    return revealed;
}