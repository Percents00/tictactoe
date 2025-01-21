// Board.cpp
#include "Board.h"
#include <algorithm>
#include <stdexcept>

Board::Board(int boardSize) : size(boardSize), grid(boardSize * boardSize, CellState::Empty) {}

bool Board::isFull() const {
    return std::all_of(grid.begin(), grid.end(), [](CellState cell){ return cell != CellState::Empty; });
}

bool Board::isCellEmpty(int x, int y) const {
    if (x < 0 || x >= size || y < 0 || y >= size) {
        throw std::out_of_range("Invalid cell coordinates");
    }
    return grid[x * size + y] == CellState::Empty;
}

void Board::setCell(int x, int y, CellState state) {
    if (x < 0 || x >= size || y < 0 || y >= size) {
        throw std::out_of_range("Invalid cell coordinates");
    }
    grid[x * size + y] = state;
}

CellState Board::getCell(int x, int y) const {
    if (x < 0 || x >= size || y < 0 || y >= size) {
        throw std::out_of_range("Invalid cell coordinates");
    }
    return grid[x * size + y];
}

int Board::getSize() const {
    return size;
}

GameState Board::checkGameState() const {
    int k = size;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j <= size - k; ++j) {
            CellState first = getCell(i, j);
            if (first != CellState::Empty) {
                bool win = true;
                for (int l = 1; l < k; ++l) {
                    if (getCell(i, j + l) != first) {
                        win = false;
                        break;
                    }
                }
                if (win) {
                    return (first == CellState::Cross) ? GameState::CrossWin : GameState::NoughtWin;
                }
            }
        }
    }

    for (int j = 0; j < size; ++j) {
        for (int i = 0; i <= size - k; ++i) {
            CellState first = getCell(i, j);
            if (first != CellState::Empty) {
                bool win = true;
                for (int l = 1; l < k; ++l) {
                    if (getCell(i + l, j) != first) {
                        win = false;
                        break;
                    }
                }
                if (win) {
                    return (first == CellState::Cross) ? GameState::CrossWin : GameState::NoughtWin;
                }
            }
        }
    }

    for (int i = 0; i <= size - k; ++i) {
        for (int j = 0; j <= size - k; ++j) {
            CellState first = getCell(i, j);
            if (first != CellState::Empty) {
                bool win = true;
                for (int l = 1; l < k; ++l) {
                    if (getCell(i + l, j + l) != first) {
                        win = false;
                        break;
                    }
                }
                if (win) {
                    return (first == CellState::Cross) ? GameState::CrossWin : GameState::NoughtWin;
                }
            }
        }
    }

    for (int i = 0; i <= size - k; ++i) {
        for (int j = k - 1; j < size; ++j) {
            CellState first = getCell(i, j);
            if (first != CellState::Empty) {
                bool win = true;
                for (int l = 1; l < k; ++l) {
                    if (getCell(i + l, j - l) != first) {
                        win = false;
                        break;
                    }
                }
                if (win) {
                    return (first == CellState::Cross) ? GameState::CrossWin : GameState::NoughtWin;
                }
            }
        }
    }

    if (isFull()) {
        return GameState::Draw;
    }

    return GameState::InProgress;
}

int Board::getEmptyCellsCount() const {
    return std::count(grid.begin(), grid.end(), CellState::Empty);
}