// TictactoeUI.h
#pragma once


#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "Game.h"


class TictactoeUI : public QWidget {
    Q_OBJECT

private:
    Game game;
    QGridLayout* gridLayout;
    std::vector<QPushButton*> buttons;
    QVBoxLayout* mainLayout;
    QLabel* statusLabel;
    QLabel* titleLabel;
    QPushButton* newGameButton;

    void createUI();
    void updateBoard();
    void resetGame();
    void showGameOverMessage();

    private slots:
        void handleButtonClick();
        void onNewGameClicked();

public:
    explicit TictactoeUI(QWidget* parent = nullptr);
};