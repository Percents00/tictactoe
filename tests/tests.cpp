#include "tests.h"
#include <stdexcept>
#include <iostream>

void check(bool condition, const std::string& message = "Test failed!") {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void runBoardTests() {
    Board board(3);
    check(board.getSize() == 3, "Board size should be 3");

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            check(board.getCell(i, j) == CellState::Empty, "Cell should be empty initially");
        }
    }

    board.setCell(0, 0, CellState::Cross);
    check(board.getCell(0, 0) == CellState::Cross, "Cell (0, 0) should be Cross");

    board.setCell(1, 2, CellState::Nought);
    check(board.getCell(1, 2) == CellState::Nought, "Cell (1, 2) should be Nought");

    check(!board.isFull(), "Board should not be full yet");

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board.setCell(i, j, CellState::Cross);
        }
    }
    check(board.isFull(), "Board should be full");

    Board board2(3);
    board2.setCell(0, 0, CellState::Cross);
    board2.setCell(1, 1, CellState::Cross);
    board2.setCell(2, 2, CellState::Cross);
    check(board2.checkGameState() == GameState::CrossWin, "Cross should win");

    Board board3(3);
    board3.setCell(0, 0, CellState::Nought);
    board3.setCell(0, 1, CellState::Nought);
    board3.setCell(0, 2, CellState::Nought);
    check(board3.checkGameState() == GameState::NoughtWin, "Nought should win");

    Board board4(3);
    board4.setCell(0, 0, CellState::Cross);
    board4.setCell(0, 1, CellState::Cross);
    board4.setCell(0, 2, CellState::Nought);
    board4.setCell(1, 0, CellState::Nought);
    board4.setCell(1, 1, CellState::Nought);
    board4.setCell(1, 2, CellState::Cross);
    board4.setCell(2, 0, CellState::Cross);
    board4.setCell(2, 1, CellState::Cross);
    board4.setCell(2, 2, CellState::Nought);
    check(board4.checkGameState() == GameState::Draw, "Game should be a draw");

    check(board2.getEmptyCellsCount() == 6, "Empty cells count should be 6");

    std::cout << "Board tests passed!" << std::endl;
}

void runBotTests() {
    Game game(3);

    Board board(3);
    Bot bot(CellState::Cross, CellState::Nought, &game);

    std::pair<int, int> move = bot.getBestMove(board);
    check(board.isCellEmpty(move.first, move.second), "Bot should choose an empty cell");

    board.setCell(0, 0, CellState::Cross);
    board.setCell(0, 1, CellState::Cross);
    move = bot.getBestMove(board);
    check(move == std::make_pair(0, 2), "Bot should block the winning move");

    board.setCell(0, 2, CellState::Nought);
    board.setCell(1, 0, CellState::Nought);
    board.setCell(1, 1, CellState::Nought);
    move = bot.getBestMove(board);
    check(move == std::make_pair(1, 2), "Bot should block the winning move");

    Board board1(3);
    board1.setCell(0, 0, CellState::Cross);
    board1.setCell(0, 1, CellState::Nought);
    board1.setCell(1, 1, CellState::Cross);
    board1.setCell(1, 0, CellState::Nought);
    board1.setCell(2, 0, CellState::Cross);
    board1.setCell(2, 1, CellState::Nought);
    board1.setCell(2, 2, CellState::Cross);

    Bot bot1(CellState::Nought, CellState::Cross, &game);
    move = bot1.getBestMove(board1);
    check(board1.isCellEmpty(move.first, move.second), "Bot should choose an empty cell");

    std::cout << "Bot tests passed!" << std::endl;
}