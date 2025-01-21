// Bot.h
#pragma once

#include "Board.h"
#include <unordered_map>
#include <string>

class Game;

class Bot {
private:
    int maxDepth;
    CellState aiPlayer;
    CellState humanPlayer;
    Game* game;
    std::unordered_map<std::string, int> memo;


    int minimax(Board& board, int depth, int alpha, int beta, bool isMaximizing);
    int evaluateCell(const Board& board, int x, int y) const;

    [[nodiscard]] int evaluate(const Board& board) const;

    [[nodiscard]] std::pair<int, int> findForcedMove(const Board& board, CellState player) const;

    std::string boardToString(const Board& board) const;


public:
    Bot(CellState ai, CellState human, Game* game, int maxDepth = 3);

    std::pair<int, int> getBestMove(Board& board);
};