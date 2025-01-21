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
    for (int i = 0; i < size; ++i) {
        CellState first = getCell(i, 0);
        if (first != CellState::Empty &&
            std::all_of(grid.begin() + i * size, grid.begin() + (i + 1) * size, [&](CellState s){ return s == first; })) {
            return (first == CellState::Cross) ? GameState::CrossWin : GameState::NoughtWin;
        }
    }

    for (int j = 0; j < size; ++j) {
        CellState first = getCell(0, j);
         if (first != CellState::Empty) {
                bool win = true;
                for (int i = 1; i < size; ++i) {
                    if (getCell(i, j) != first) {
                        win = false;
                        break;
                    }
                }
                if (win) {
                    return (first == CellState::Cross) ? GameState::CrossWin : GameState::NoughtWin;
                }
            }
    }

    CellState center = getCell(size / 2, size/ 2);
     if (center != CellState::Empty) {
            bool mainDiagWin = true;
            for (int i = 0; i < size; ++i) {
                if (getCell(i, i) != center) {
                    mainDiagWin = false;
                    break;
                }
            }
             if (mainDiagWin) { return (center == CellState::Cross) ? GameState::CrossWin : GameState::NoughtWin; }

            bool antiDiagWin = true;
            for (int i = 0; i < size; ++i) {
                if (getCell(i, size - 1 - i) != center) {
                    antiDiagWin = false;
                    break;
                }
            }

            if (antiDiagWin) { return (center == CellState::Cross) ? GameState::CrossWin : GameState::NoughtWin; }

        }




    if (isFull()) {
        return GameState::Draw;
    }

    return GameState::InProgress;
}

int Board::getEmptyCellsCount() const {
    return std::count(grid.begin(), grid.end(), CellState::Empty);
}