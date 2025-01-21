// Bot.cpp
#include "Bot.h"
#include "Game.h"
#include <sstream>
#include <limits>
#include <algorithm>
#include <string>

Bot::Bot(CellState ai, CellState human, Game* game, int maxDepth)
    : aiPlayer(ai), humanPlayer(human), game(game), maxDepth(maxDepth) {}

int Bot::evaluate(const Board& board) const {
    GameState state = board.checkGameState();
    if (state == GameState::CrossWin) {
        return (aiPlayer == CellState::Cross) ? 10 : -10;
    } else if (state == GameState::NoughtWin) {
        return (aiPlayer == CellState::Nought) ? 10 : -10;
    }

    int score = 0;
    int size = board.getSize();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board.getCell(i, j) == aiPlayer) {
                score += evaluateCell(board, i, j);
            } else if (board.getCell(i, j) == humanPlayer) {
                score -= evaluateCell(board, i, j);
            }
        }
    }
    return score;
}

int Bot::evaluateCell(const Board& board, int x, int y) const {
    int score = 0;
    int size = board.getSize();
    CellState cell = board.getCell(x, y);

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int count = 1;
            for (int step = 1; step < 3; ++step) {
                int nx = x + dx * step;
                int ny = y + dy * step;
                if (nx >= 0 && nx < size && ny >= 0 && ny < size && board.getCell(nx, ny) == cell) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == 2) score += 1;
            if (count == 3) score += 5;
        }
    }
    return score;
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

int Bot::minimax(Board& board, int depth, int alpha, int beta, bool isMaximizing) {
    std::string boardStr = boardToString(board);
    if (memo.count(boardStr)) {
        return memo[boardStr];
    }

    int score = evaluate(board);

    if (score != 0 || board.checkGameState() == GameState::Draw || depth == 0) {
        memo[boardStr] = score;
        return score;
    }

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < board.getSize(); i++) {
            for (int j = 0; j < board.getSize(); j++) {
                if (board.isCellEmpty(i, j)) {
                    board.setCell(i, j, aiPlayer);
                    int currentScore = minimax(board, depth - 1, alpha, beta, false);
                    board.setCell(i, j, CellState::Empty);
                    bestScore = std::max(bestScore, currentScore);
                    alpha = std::max(alpha, bestScore);
                    if (beta <= alpha) {
                        break;
                    }
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
                    int currentScore = minimax(board, depth - 1, alpha, beta, true);
                    board.setCell(i, j, CellState::Empty);
                    bestScore = std::min(bestScore, currentScore);
                    beta = std::min(beta, bestScore);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        memo[boardStr] = bestScore;
        return bestScore;
    }
}

std::pair<int, int> Bot::getBestMove(Board& board) {
    int size = board.getSize();
    int depth = (size <= 3) ? 5 : (size <= 5) ? 4 : 3;

    std::pair<int, int> forcedMoveAi = findForcedMove(board, aiPlayer);
    if (forcedMoveAi.first != -1)
        return forcedMoveAi;

    std::pair<int, int> forcedMoveHuman = findForcedMove(board, humanPlayer);
    if (forcedMoveHuman.first != -1)
        return forcedMoveHuman;

    int bestScore = (aiPlayer == this->game->getCurrentPlayer()) ? -1000 : 1000;
    std::pair<int, int> bestMove = {-1, -1};
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board.isCellEmpty(i, j)) {
                board.setCell(i, j, aiPlayer);
                int moveScore = minimax(board, depth, -1000, 1000, aiPlayer != this->game->getCurrentPlayer());
                board.setCell(i, j, CellState::Empty);

                if ((aiPlayer == this->game->getCurrentPlayer() && moveScore > bestScore) ||
                    (aiPlayer != this->game->getCurrentPlayer() && moveScore < bestScore)) {
                    bestScore = moveScore;
                    bestMove = {i, j};
                    }
            }
        }
    }
    return bestMove;
}