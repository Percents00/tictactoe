// Game.h
#pragma once

#include "Board.h"
#include "Bot.h"

class Game {
private:
    Board board;
    Bot bot;
    CellState currentPlayer;

public:
    Game(int boardSize);

    void botMove();

    Board& getBoard();

    CellState getCurrentPlayer() const;

    void setCurrentPlayer(CellState player);

    void makeMove(int x, int y, CellState player);

    [[nodiscard]] GameState getGameState() const;
};