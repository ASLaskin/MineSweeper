#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <vector>
#include "Board.h"
#include <chrono>
using namespace std;

//Bugs
// Program still registers clicks when in leaderboard mode


//MAIN IS BELOW THIS FUNCTION
bool leaderBoardDisplay(int numRows,int numColoms,string newTimestamp,bool gameWon){
    cout << "opening leader" << "\n";
    float width = 16 * numColoms;
    float height = (numRows * 16) + 50;
    sf::RenderWindow leaderboardWindow(sf::VideoMode(width, height ), "Minesweeper");
    cout << "reading leadeboard" << "\n";
    
    ifstream leaderFile;
    leaderFile.open("files/leaderboard.txt");
    vector<string> timestamps;
    if (leaderFile.is_open()) {
        string line;
        while (getline(leaderFile, line)) {
            timestamps.push_back(line);
        }
        leaderFile.close();
    }
    sort(timestamps.begin(), timestamps.end());

    //This splits
    auto commaIndex = newTimestamp.find(",");
    string timestamp = newTimestamp.substr(0, commaIndex);
    //Finds where to insert
    if (gameWon) {

        int insI = 0;
        for (int i = 0; i < timestamps.size(); i++) {
            auto commaIndex = timestamps[i].find(",");
            string curTimestamp = timestamps[i].substr(0, commaIndex);
            if (curTimestamp > timestamp) {
                insI = i;
                break;
            } else {
                insI = i + 1;
            }
        }
        timestamps.insert(timestamps.begin() + insI, newTimestamp);
        timestamps.pop_back();
    }

    ofstream outputLeader("files/leaderboard.txt");
    if (outputLeader.is_open()) {
        for (int i = 0; i < timestamps.size(); i++) {
            outputLeader << timestamps[i] << "\n";
        }
        outputLeader.close();
    }


    string sortedTimestamps = "";
    for (int i = 0; i < 5; i++) {
        string temp;
        size_t commaIndex = timestamps[i].find(",");
        string timestamp = timestamps[i].substr(0, commaIndex);
        string name = timestamps[i].substr(commaIndex + 1);
        string index = to_string(i + 1);
        temp += index + ".    " + timestamp + "      " + name;
        sortedTimestamps += temp + "\n\n";
    }

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        cout << "ERROR!";
    }

    sf::Text rank(sortedTimestamps,font, 18);
    rank.setStyle(sf::Text::Bold);
    rank.setFillColor(sf::Color::White);

    sf::Text title("LEADERBOARD",font,20);
    title.setStyle(sf::Text::Bold);
    title.setStyle(sf::Text::Underlined);

    sf::FloatRect rankRect = rank.getLocalBounds();
    rank.setOrigin(rankRect.left + rankRect.width / 2.0f, rankRect.top + rankRect.height / 2.0f);
    rank.setPosition(sf::Vector2f(width / 2.0f, height / 2.0f + 20 ));

    sf::FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    title.setPosition(sf::Vector2f(width / 2.0f, height / 2.0f - 120));

    while (leaderboardWindow.isOpen()) {
        sf::Event event3;
        while (leaderboardWindow.pollEvent(event3)) {
            if (event3.type == sf::Event::Closed) {
                cout << "closing leader";
                leaderboardWindow.close();
                return false;
            }
        }
        leaderboardWindow.clear(sf::Color::Blue);
        leaderboardWindow.draw(rank);
        leaderboardWindow.draw(title);
        leaderboardWindow.display();
    }
    return false;
}




int main() {
    //reading board config there is probably a better way to do it,but it works
    string tempColoms;
    string tempRows;
    string tempMines;

    ifstream configFile;
    configFile.open("files/board_config.cfg");
    if (configFile.is_open()) {
        configFile >> tempColoms;
        configFile >> tempRows;
        configFile >> tempMines;
    } else {
        cout << "failed to open";
    }
    configFile.close();
    float numColoms = stof(tempColoms);
    float numRows = stof(tempRows);


    float width = numColoms * 32;
    float height = (numRows * 32) + 100;
    float numMines = stof(tempMines);

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        cout << "ERROR!";
    }

    sf::RenderWindow welcomeWindow(sf::VideoMode(width, height), "Minesweeper");

    sf::Text welcome("WELCOME TO MINESWEEPER!", font, 24);
    welcome.setStyle(sf::Text::Bold);
    welcome.setStyle(sf::Text::Underlined);
    welcome.setFillColor(sf::Color::White);

    sf::Text enterName("Enter your name:", font, 20);
    enterName.setStyle(sf::Text::Bold);
    welcome.setFillColor(sf::Color::White);

    //For input text make a text that updates on event
    sf::Text name("|", font, 18);
    name.setStyle(sf::Text::Bold);
    name.setFillColor(sf::Color::Yellow);

    sf::String playerInput = "";
    string fixed;

    while (welcomeWindow.isOpen()) {
        sf::Event event;
        while (welcomeWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                welcomeWindow.close();
                abort();
            }
            if (event.type == sf::Event::TextEntered) {
                name.setFillColor(sf::Color::Yellow);
                string cursor = playerInput;
                cursor.pop_back();
                name.setString(cursor);

                playerInput = name.getString();
                if ((isalpha(event.text.unicode)) and playerInput.getSize() < 10){
                    playerInput += event.text.unicode;
                }
                name.setString(playerInput);
                if (sf::Keyboard::isKeyPressed((sf::Keyboard::BackSpace))) {
                    if (playerInput == ""){}
                    else {
                        string temp = playerInput;
                        temp.pop_back();
                        temp.pop_back();
                        playerInput = temp;
                        name.setFillColor(sf::Color::Yellow);
                        name.setString(playerInput);
                    }
                }
                if (sf::Keyboard::isKeyPressed((sf::Keyboard::Enter))) {
                    if (playerInput == ""){

                    }else {
                        welcomeWindow.close();
                        cout << "Opening game window" << "\n";
                        cursor = playerInput;
                        playerInput = cursor;
                        break;
                    }
                }
                playerInput += "|";
                name.setString(playerInput);
                name.setFillColor(sf::Color::Yellow);
            }
        }

        welcomeWindow.clear(sf::Color::Blue);

        //looks weird but works
        sf::FloatRect welcomeRect = welcome.getLocalBounds();
        welcome.setOrigin(welcomeRect.left + welcomeRect.width / 2.0f, welcomeRect.top + welcomeRect.height / 2.0f);
        welcome.setPosition(sf::Vector2f(width / 2.0f, height / 2.0f - 75));

        sf::FloatRect enterNameRect = enterName.getLocalBounds();
        enterName.setOrigin(enterNameRect.left + enterNameRect.width / 2.0f,enterNameRect.top + enterNameRect.height / 2.0f);
        enterName.setPosition(sf::Vector2f(width / 2.0f, height / 2.0f - 45));

        sf::FloatRect nameRect = name.getLocalBounds();
        name.setOrigin(nameRect.left + nameRect.width / 2.0f, nameRect.top + nameRect.height / 2.0f);
        name.setPosition(sf::Vector2f(width / 2.0f, height / 2.0f));

        welcomeWindow.draw(welcome);
        welcomeWindow.draw(enterName);
        welcomeWindow.draw(name);
        welcomeWindow.display();

    }

    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Game Window");

    string basicString;
    basicString = playerInput;
    char tempChar;
    for (int i = 0; i < basicString.size(); i++) {
        if (i == 11) {
            break;
        }
        if (isalpha(basicString[i])) {
            if (i == 0) {
                tempChar = toupper(basicString[i]);
                fixed += tempChar;
            } else {
                tempChar = tolower(basicString[i]);
                fixed += tempChar;
            }
        } else {
            continue;
        }
    }
    //FIXED IS THE NAME OF THE PLAYER AFTER REMOVING CHAR
    cout << fixed << "\n";

    //*******************************GAME WINDOW CODE***********************************
    //SPRITES LOADED WITH TEXTURES DEFINITLY COULDVE USED A MAP BUT WAS LAZY
    sf::Texture tileTex;
    if (!tileTex.loadFromFile("files/images/tile_hidden.png")) {
        cout << "Tile failed to load";
    }
    sf::Texture revealTex;
    revealTex.loadFromFile("files/images/tile_revealed.png");

    sf::Texture debugTex;
    sf::Sprite debugSprite;
    if (!debugTex.loadFromFile("files/images/debug.png")) {
        cout << "Tile failed to load";
    } else {
        debugSprite.setTexture(debugTex);
    }

    sf::Texture mineTex;
    sf::Sprite mineSprite;
    if (!mineTex.loadFromFile("files/images/mine.png")) {
        cout << "Tile failed to load";
    } else{
        mineSprite.setTexture(mineTex);
    }
    sf::Texture defaultFaceTex;
    sf::Sprite faceSprite;
    if (!defaultFaceTex.loadFromFile("files/images/face_happy.png")) {
        cout << "Tile failed to load";
    } else{
        faceSprite.setTexture(defaultFaceTex);
    }
    sf::Texture faceLost;
    faceLost.loadFromFile("files/images/face_lose.png");
    sf::Texture faceWin;
    faceWin.loadFromFile("files/images/face_win.png");

    sf::Texture flagTex;
    if (!flagTex.loadFromFile("files/images/flag.png")) {
        cout << "Tile failed to load";}

    vector<sf::Texture> textures;
    sf::Texture num1;
    num1.loadFromFile("files/images/number_1.png");
    textures.push_back(num1);
    sf::Texture num2;
    num2.loadFromFile("files/images/number_2.png");
    textures.push_back(num2);
    sf::Texture num3;
    num3.loadFromFile("files/images/number_3.png");
    textures.push_back(num3);
    sf::Texture num4;
    num4.loadFromFile("files/images/number_4.png");
    textures.push_back(num4);
    sf::Texture num5;
    num5.loadFromFile("files/images/number_5.png");
    textures.push_back(num5);
    sf::Texture num6;
    num6.loadFromFile("files/images/number_6.png");
    textures.push_back(num6);
    sf::Texture num7;
    num7.loadFromFile("files/images/number_7.png");
    textures.push_back(num7);
    sf::Texture num8;
    num8.loadFromFile("files/images/number_8.png");
    textures.push_back(num8);

    sf::Sprite pauseButton;
    sf::Texture pauseTex;

    if (!pauseTex.loadFromFile("files/images/pause.png")) {
        cout << "Tile failed to load";
    } else {
        pauseButton.setTexture(pauseTex);
    }

    sf::Sprite leaderboardButton;
    sf::Texture leadTex;
    if (!leadTex.loadFromFile("files/images/leaderboard.png")) {
        cout << "Tile failed to load";
    } else {
        leaderboardButton.setTexture(leadTex);
    }
    leaderboardButton.setPosition((numColoms * 32) - 176, 32 * (numRows+0.5f));

    sf::Texture playTex;
    playTex.loadFromFile("files/images/play.png");

    sf::Texture digitTex;

    sf::Sprite counter[4];
    counter[0].setTexture(digitTex);
    counter[0].setPosition(33, 32 * (numRows+0.5f)+16);
    counter[1].setTexture(digitTex);
    counter[1].setPosition(33 + 21, 32 * (numRows+0.5f)+16);
    counter[2].setTexture(digitTex);
    counter[2].setPosition(33 + 42, 32 * (numRows+0.5f)+16);
    counter[3].setTexture(digitTex);
    counter[3].setPosition(12, 32 * (numRows+0.5f)+16);




    //CLOCK
    digitTex.loadFromFile("files/images/digits.png");
    sf::Sprite timerSprites[4];
    sf::Clock clock;
    timerSprites[0].setTexture(digitTex);
    timerSprites[0].setPosition(((numColoms)*32)-97,32*((numRows)+0.5f) + 16);

    timerSprites[1].setTexture(digitTex);
    timerSprites[1].setPosition(((numColoms)*32)-76,32*((numRows)+0.5f) + 16);

    timerSprites[2].setTexture(digitTex);
    timerSprites[2].setPosition(((numColoms)*32)-54,32*((numRows)+0.5f) + 16);

    timerSprites[3].setTexture(digitTex);
    timerSprites[3].setPosition(((numColoms)*32)-33,32*((numRows)+0.5f) + 16);

    //This is where board is created
    Board currentBoard(numRows, numColoms, numMines);
    vector<vector<sf::Sprite>> spriteVec = currentBoard.generateSpriteList(numRows, numColoms);
    currentBoard.generateNums(numRows,numColoms);

    faceSprite.setPosition(((numColoms / 2.0) * 32) - 32, 32 *(numRows+0.5f));
    debugSprite.setPosition((numColoms*32)-304,32 * (numRows+0.5f));
    pauseButton.setPosition((numColoms * 32) - 240, 32 * (numRows+0.5f));

    //This sets all tiles to default
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numColoms; col++) {
            spriteVec[row][col].setTexture(tileTex);
        }
    }

    bool debugMode = false;
    bool paused = false;
    bool leaderBoard = false;
    int iRevealed;
    bool gameRestarted = false;
    int counterNum;
    int flagged = 0;
    sf::Clock Clock;
    sf::Time ElapsedTime;
    int seconds;
    int minutes;

    chrono::system_clock::time_point pauseTime;
    int elapsedPauseTime = 0;
    int minutesOut;
    int secondsOut;
    auto pauseB = pauseButton.getGlobalBounds();
    auto debugB = debugSprite.getGlobalBounds();
    auto faceB = faceSprite.getGlobalBounds();
    auto leaderB = leaderboardButton.getGlobalBounds();

    while (gameWindow.isOpen()) {
        sf::Event event2;
        while (gameWindow.pollEvent(event2) and !leaderBoard) {
            if (event2.type == sf::Event::Closed) {
                gameWindow.close();
            }
            if (event2.type == sf::Event::MouseButtonPressed) {
                if (event2.mouseButton.button == sf::Mouse::Left) {
                    int mouseX = event2.mouseButton.x;
                    int mouseY = event2.mouseButton.y;


                    //THIS RUNS IF THEY CLICK A TILE
                    if (mouseY <= height - 100) {
                        int tempX = mouseX / 32;
                        int tempY = mouseY / 32;
                        if (!paused) {
                            if (currentBoard.firstClick and !leaderBoard) {
                                //MAKE SURE THE FIRST TILE WE CLICK HAS VALUE 0 CANNOT START WITH NUMBER
                                if (currentBoard.tiles[tempY][tempX].getValue() != 0) {
                                    //This checks so the first click isnt a mine
                                    if (currentBoard.tiles[tempY][tempX].isFlagged()){}else {
                                        while (currentBoard.tiles[tempY][tempX].getValue() != 0) {
                                            Board newBoard(numRows, numColoms, numMines);
                                            currentBoard = newBoard;
                                            vector<vector<sf::Sprite>> spritesVec = newBoard.generateSpriteList(numRows,
                                                                                                                numColoms);
                                            currentBoard.generateNums(numRows, numColoms);
                                        }
                                        currentBoard.bangReveal(tempY, tempX, numRows, numColoms);
                                        currentBoard.firstClick = false;
                                    }
                                } else {
                                    if (!currentBoard.tiles[tempY][tempX].isFlagged()) {
                                        currentBoard.bangReveal(tempY, tempX, numRows, numColoms);
                                        currentBoard.firstClick = false;
                                    }
                                }

                            }
                        }
                        if (currentBoard.tiles[tempY][tempX].getValue() == 9) {
                            if (currentBoard.isWon or currentBoard.tiles[tempY][tempX].isFlagged()) {}
                            else {
                                currentBoard.isLost = true;
                                debugMode = true;
                                spriteVec[tempY][tempX].setTexture(tileTex);
                                gameWindow.draw(spriteVec[tempY][tempX]);
                                cout << "game lost" << "\n";
                                faceSprite.setTexture(faceLost);
                            }
                        }

                        if (currentBoard.isLost) {
                            cout << "game is lost stop trying to click" << "\n";
                        } else if (paused) {
                            cout << "game is paused stop trying to click";
                        } else if (currentBoard.isWon) {
                            cout << "game is won stop trying to click";
                        } else {
                            if (currentBoard.tiles[tempY][tempX].getValue() == 0 and !currentBoard.tiles[tempY][tempX].isFlagged()) {
                                currentBoard.tiles[tempY][tempX].setRevealed();
                                currentBoard.bangReveal(tempY, tempX, numRows, numColoms);
                            }
                            if (currentBoard.tiles[tempY][tempX].isFlagged()) {

                            } else {
                                currentBoard.tiles[tempY][tempX].setRevealed();
                            }
                        }
                    }

                    //DEBUG MODE PRESSED
                    if (debugB.contains(event2.mouseButton.x, event2.mouseButton.y)) {
                        if (currentBoard.isWon or currentBoard.isLost or paused) {
                        } else {
                            cout << "debug toggled" << "\n";
                            debugMode = !debugMode;
                        }
                    }
                    //PAUSE BUTTON
                    if (pauseB.contains(event2.mouseButton.x, event2.mouseButton.y)) {
                        cout << "pause toggled";
                        if (currentBoard.isWon or currentBoard.isWon){

                        }else {
                            paused = !paused;
                        }

                        if (paused) {
                            pauseTime = chrono::system_clock::now();
                            pauseButton.setTexture(playTex);
                        } else {
                            auto resume_time = chrono::system_clock::now();
                            elapsedPauseTime += chrono::duration_cast<chrono::seconds>(resume_time - pauseTime).count();
                            pauseButton.setTexture(pauseTex);
                        }
                    }

                    //LEADERBOARD BUTTON
                    if (leaderB.contains(event2.mouseButton.x, event2.mouseButton.y)) {
                        leaderBoard = !leaderBoard;
                        if (leaderBoard) {
                            cout << "pausing all tiles" << "\n";
                            if (currentBoard.isWon) {

                            } else {
                                for (int row = 0; row < numRows; row++) {
                                    for (int col = 0; col < numColoms; col++) {
                                        spriteVec[row][col].setTexture(revealTex);
                                        gameWindow.draw(spriteVec[row][col]);
                                    }
                                }
                            }

                            gameWindow.display();

                            pauseTime = chrono::system_clock::now();
                            pauseButton.setTexture(playTex);
                            gameWindow.draw(pauseButton);
                            leaderBoardDisplay(numRows, numColoms, "", false);
                            auto resume_time = chrono::system_clock::now();
                            elapsedPauseTime += chrono::duration_cast<chrono::seconds>(resume_time - pauseTime).count();
                            leaderBoard = false;
                        }
                    }

                    //FACE SPRITE PRESSED
                    if (faceB.contains(event2.mouseButton.x, event2.mouseButton.y)) {
                        if (currentBoard.isLost or currentBoard.isWon) {
                            currentBoard.isWon = false;
                            currentBoard.isLost = false;
                            faceSprite.setTexture(defaultFaceTex);

                        }
                        if (paused) {
                            paused = false;
                            pauseButton.setTexture(pauseTex);
                        }
                        cout << "New Board Generated" << "\n";
                        minutes = 0;
                        seconds = 0;
                        Board newBoard(numRows, numColoms, numMines);
                        currentBoard = newBoard;
                        vector<vector<sf::Sprite>> spritesVec = newBoard.generateSpriteList(numRows, numColoms);
                        currentBoard.generateNums(numRows, numColoms);
                        gameRestarted = true;
                        debugMode = false;
                    }
                }

                //THIS IS WHAT FLAGS TILES
                if (event2.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
                    for (int row = 0; row < numRows; row++) {
                        for (int col = 0; col < numColoms; col++) {
                            sf::FloatRect bounds = spriteVec[row][col].getGlobalBounds();
                            if (bounds.contains(sf::Vector2f(mousePosition))) {
                                if (currentBoard.tiles[row][col].isFlagged()) {
                                    currentBoard.tiles[row][col].removeFlag();
                                } else {
                                    currentBoard.tiles[row][col].setFlag();
                                }
                            }
                        }
                    }
                }
            }
        }


        gameWindow.clear(sf::Color::White);

        if (!paused and !currentBoard.isWon and !currentBoard.isLost){
            if (gameRestarted){
                clock.restart();
                gameRestarted = false;

            }
            sf::Time elapsed = clock.getElapsedTime();
            seconds = elapsed.asSeconds();
            seconds -= elapsedPauseTime;
            minutes = seconds/60;
            seconds %= 60;

            timerSprites[0].setTextureRect(sf::IntRect(21 * (minutes / 10),0,21,32));
            timerSprites[1].setTextureRect(sf::IntRect(21 * (minutes % 10),0,21,32));
            timerSprites[2].setTextureRect(sf::IntRect(21 * (seconds / 10),0,21,32));
            timerSprites[3].setTextureRect(sf::IntRect(21 * (seconds % 10),0,21,32));


            minutesOut = minutes;
            secondsOut = seconds;

        }

//This is what actually draws the spriteVec uses sprite 2d vector
        iRevealed = 0;
        counterNum = numMines;
        counterNum -= flagged;
        flagged = 0;
        for (int row = 0; row < numRows; row++) {

                for (int col = 0; col < numColoms; col++) {
                    spriteVec[row][col].setTexture(tileTex);
                    gameWindow.draw(spriteVec[row][col]);

                    if (currentBoard.tiles[row][col].isRevealed()){
                        iRevealed++;
                        spriteVec[row][col].setTexture(revealTex);
                        gameWindow.draw(spriteVec[row][col]);

                        if (currentBoard.tiles[row][col].getValue() > 0){
                            int number = currentBoard.tiles[row][col].getValue();
                            spriteVec[row][col].setTexture(textures[number-1]);
                        }
                        gameWindow.draw(spriteVec[row][col]);
                    }else if (currentBoard.tiles[row][col].isFlagged() and !currentBoard.isWon and !currentBoard.isLost){
                            gameWindow.draw(spriteVec[row][col]);
                            spriteVec[row][col].setTexture(flagTex);
                            flagged ++;
                            gameWindow.draw(spriteVec[row][col]);

                    }if(debugMode){
                        if ((currentBoard.tiles[row][col].getValue()) == 9) {
                            gameWindow.draw(spriteVec[row][col]);
                            spriteVec[row][col].setTexture(mineTex);
                            gameWindow.draw(spriteVec[row][col]);
                        }
                    }
                    if (paused){
                        spriteVec[row][col].setTexture(revealTex);
                        gameWindow.draw(spriteVec[row][col]);
                        for (int i = 0; i < 4;i++){
                            gameWindow.draw(timerSprites[i]);
                        }
                    }else{
                        gameWindow.draw(spriteVec[row][col]);
                    }

                    for (int i = 0; i < 4;i++){
                        timerSprites[0].setTextureRect(sf::IntRect(21 * (minutes / 10),0,21,32));
                        timerSprites[1].setTextureRect(sf::IntRect(21 * (minutes % 10),0,21,32));
                        timerSprites[2].setTextureRect(sf::IntRect(21 * (seconds / 10),0,21,32));
                        timerSprites[3].setTextureRect(sf::IntRect(21 * (seconds % 10),0,21,32));
                        gameWindow.draw(timerSprites[i]);
                    }


                    if (counterNum < 0){
                        counterNum = abs(counterNum);
                        counter[0].setTextureRect(sf::IntRect(21 * (counterNum / 100), 0, 21, 32));
                        counter[1].setTextureRect(sf::IntRect(21 * ((counterNum / 10) % 10), 0, 21, 32));
                        counter[2].setTextureRect(sf::IntRect(21 * (counterNum % 10), 0, 21, 32));
                        counter[3].setTextureRect(sf::IntRect(210,0,21,32));
                    }else {
                        counter[0].setTextureRect(sf::IntRect(21 * (counterNum / 100), 0, 21, 32));
                        counter[1].setTextureRect(sf::IntRect(21 * ((counterNum / 10) % 10), 0, 21, 32));
                        counter[2].setTextureRect(sf::IntRect(21 * (counterNum % 10), 0, 21, 32));
                        counter[3].setTextureRect(sf::IntRect(0,0,0,0));
                    }
                    for (int i = 0; i < 4; i++) {
                        gameWindow.draw(counter[i]);
                    }
                }
        }
            if (iRevealed == numRows * numColoms - numMines){

                for (int row = 0; row < numRows; row++) {
                    for (int col = 0; col < numColoms; col++) {
                        if ((currentBoard.tiles[row][col].getValue()) == 9) {
                            spriteVec[row][col].setTexture(tileTex);
                            gameWindow.draw(spriteVec[row][col]);
                            spriteVec[row][col].setTexture(flagTex);
                            gameWindow.draw(spriteVec[row][col]);
                        }
                    }
                }
                faceSprite.setTexture(faceWin);
                gameWindow.draw(faceSprite);
                gameWindow.draw(leaderboardButton);
                gameWindow.draw(pauseButton);
                gameWindow.draw(debugSprite);
                gameWindow.display();
                currentBoard.isWon = true;
                if (!currentBoard.leaderBoardOpened) {
                    string temp = "";
                    string minuteStr = "";
                    string secondsStr = "";
                    if (minutesOut < 10){
                        minuteStr += "0" + to_string(minutesOut);
                    } else{
                        minuteStr += to_string(minutesOut);
                    }
                    if (secondsOut < 10){
                        secondsStr += "0" + to_string(secondsOut);
                    }else{
                        secondsStr += to_string(secondsOut);
                    }
                    temp += minuteStr + ":" + secondsStr + "," + fixed;
                    leaderBoardDisplay(numRows, numColoms,temp, true);
                    currentBoard.leaderBoardOpened = true;
                }
                leaderBoard = false;

            }else{
                gameWindow.draw(leaderboardButton);
                gameWindow.draw(pauseButton);
                gameWindow.draw(debugSprite);
                gameWindow.draw(faceSprite);
                gameWindow.display();

            }

    }
////This was used to write to file with all tiles and their values
//Became obscure with actual debugging stuff
//            ofstream boardFile("boardMines.txt");
//            for (int i = 0; i < currentBoard.tiles.size(); i++) {
//                for (int j = 0; j < numColoms; j++) {
//                    Tile temp = currentBoard.getTileAt(i, j);
//                    boardFile << temp.getValue() << " ";
//                }
//                boardFile << "\n";
//            }
//            boardFile.close();
//


}
