// Bot.cpp
#include "Bot.h"
#include "Game.h"
#include <sstream>
#include <limits>
#include <algorithm>
#include <string>

Bot::Bot(CellState ai, CellState human, Game* game) : aiPlayer(ai), humanPlayer(human), game(game) {}


int Bot::evaluate(const Board& board) const {
    GameState state = board.checkGameState();
    if (state == GameState::CrossWin) {
        return (aiPlayer == CellState::Cross) ? 10 : -10;
    } else if (state == GameState::NoughtWin) {
        return (aiPlayer == CellState::Nought) ? 10 : -10;
    }
    return 0;
}


std::string Bot::boardToString(const Board& board) const {
     std::ostringstream oss;
    for (int i = 0; i < board.getSize(); ++i) {
        for (int j = 0; j < board.getSize(); ++j) {
            oss << static_cast<int>(board.getCell(i, j));
        }
    }
    return oss.str();
}


std::pair<int, int> Bot::findForcedMove(const Board& board, CellState player) const {
    for (int i = 0; i < board.getSize(); ++i) {
        for (int j = 0; j < board.getSize(); ++j) {
            if (board.isCellEmpty(i, j)) {
                Board tempBoard = board;
                tempBoard.setCell(i, j, player);
                if (tempBoard.checkGameState() == ((player == aiPlayer) ? GameState::NoughtWin : GameState::CrossWin))
                    return {i, j};
            }
        }
    }
    return {-1, -1};

}


int Bot::minimax(Board& board, int depth, bool isMaximizing) {
    std::string boardStr = boardToString(board);
    if (memo.count(boardStr)) {
        return memo[boardStr];
    }


    int score = evaluate(board);

    if (score != 0 || board.checkGameState() == GameState::Draw) {
        memo[boardStr] = score;
        return score;
    }


    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < board.getSize(); i++) {
            for (int j = 0; j < board.getSize(); j++) {
                if (board.isCellEmpty(i, j)) {
                    board.setCell(i, j, aiPlayer);
                    bestScore = std::max(bestScore, minimax(board, depth + 1, !isMaximizing));
                    board.setCell(i, j, CellState::Empty);
                }
            }
        }
        memo[boardStr] = bestScore;
        return bestScore;


    } else {
        int bestScore = 1000;
        for (int i = 0; i < board.getSize(); i++) {
            for (int j = 0; j < board.getSize(); j++) {
                if (board.isCellEmpty(i, j)) {
                    board.setCell(i, j, humanPlayer);
                    bestScore = std::min(bestScore, minimax(board, depth + 1, !isMaximizing));
                    board.setCell(i, j, CellState::Empty);
                }
            }
        }
        memo[boardStr] = bestScore;
        return bestScore;
    }


}


std::pair<int, int> Bot::getBestMove(Board& board) {

    std::pair<int, int> forcedMoveAi = findForcedMove(board, aiPlayer);
    if (forcedMoveAi.first != -1)
        return forcedMoveAi;

    std::pair<int, int> forcedMoveHuman = findForcedMove(board, humanPlayer);
    if (forcedMoveHuman.first != -1)
        return forcedMoveHuman;


    int bestScore = (aiPlayer == this->game->getCurrentPlayer()) ? -1000 : 1000;
    std::pair<int, int> bestMove = {-1, -1};
    for (int i = 0; i < board.getSize(); ++i) {
        for (int j = 0; j < board.getSize(); ++j) {
            if (board.isCellEmpty(i, j)) {
                board.setCell(i, j, aiPlayer);
                int moveScore = minimax(board, 0, aiPlayer != this->game->getCurrentPlayer());
                board.setCell(i, j, CellState::Empty);

                if ((aiPlayer == this->game->getCurrentPlayer() && moveScore > bestScore) || (aiPlayer != this->game->getCurrentPlayer() && moveScore < bestScore))
                {
                    bestScore = moveScore;
                    bestMove = {i, j};
                }


            }
        }
    }
    return bestMove;
}