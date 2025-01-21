
#include "Game.h"
#include <stdexcept>

Game::Game(int boardSize) : board(boardSize), bot(CellState::Nought, CellState::Cross, this), currentPlayer(CellState::Cross) {}

void Game::botMove() {
    std::pair<int, int> bestMove = bot.getBestMove(board);
    board.setCell(bestMove.first, bestMove.second, currentPlayer);
}

Board& Game::getBoard() {
    return board;
}



CellState Game::getCurrentPlayer() const {
    return currentPlayer;
}

void Game::setCurrentPlayer(CellState player) {
    currentPlayer = player;
}

void Game::makeMove(int x, int y, CellState player) {
    if (board.isCellEmpty(x, y)) {
        board.setCell(x, y, player);
    } else {
        throw std::runtime_error("Cell is not empty");
    }
}

GameState Game::getGameState() const {
    return board.checkGameState();
}