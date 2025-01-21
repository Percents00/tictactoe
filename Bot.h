// Bot.h
#pragma once

#include "Board.h"
#include <unordered_map>
#include <string>

class Game;

class Bot {
private:
    CellState aiPlayer;
    CellState humanPlayer;
    Game* game;
    std::unordered_map<std::string, int> memo;


    int minimax(Board& board, int depth, bool isMaximizing);

    [[nodiscard]] int evaluate(const Board& board) const;

    [[nodiscard]] std::pair<int, int> findForcedMove(const Board& board, CellState player) const;

    std::string boardToString(const Board& board) const;


public:
    Bot(CellState ai, CellState human, Game* game);

    std::pair<int, int> getBestMove(Board& board);
};