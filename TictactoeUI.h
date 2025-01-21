// TictactoeUI.h
#pragma once

#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include "Game.h"

class TictactoeUI : public QWidget {
    Q_OBJECT

private:
    Game game;
    QStackedWidget* stackedWidget;
    QWidget* mainMenuWidget;
    QWidget* gameWidget;
    QGridLayout* gridLayout;
    std::vector<QPushButton*> buttons;
    QVBoxLayout* mainLayout;
    QLabel* statusLabel;
    QLabel* titleLabel;
    QPushButton* newGameButton;
    QPushButton* crossFirstButton;
    QPushButton* noughtFirstButton;

    void createMainMenu();
    void createGameUI();
    void updateBoard();
    void resetGame();
    void showGameOverMessage();

    private slots:
        void handleButtonClick(int index);
    void onNewGameClicked();
    void startGame(CellState firstPlayer);
    void setCrossFirst();
    void setNoughtFirst();


public:
    explicit TictactoeUI(QWidget* parent = nullptr);
};