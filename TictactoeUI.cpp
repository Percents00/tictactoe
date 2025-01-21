// TictactoeUI.cpp
#include "TictactoeUI.h"
#include <QMessageBox>
#include <QFont>

TictactoeUI::TictactoeUI(QWidget *parent)
        : QWidget(parent), game(3), gridLayout(new QGridLayout),
          mainLayout(new QVBoxLayout(this)), titleLabel(new QLabel("Крестики-нолики")),
          newGameButton(new QPushButton("Новая игра")), statusLabel(new QLabel) {

    setWindowTitle("TicTacToe");
    setFixedSize(340, 440);

    createUI();
    resetGame();
}


void TictactoeUI::createUI() {
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setStyleSheet("color: #555;");
    newGameButton->setFont(QFont("Arial", 16));
    newGameButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px; border-radius: 7px;");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFont(QFont("Arial", 18));
    mainLayout->addWidget(statusLabel);
    gridLayout->setHorizontalSpacing(10);
    gridLayout->setVerticalSpacing(10);
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(newGameButton);

    connect(newGameButton, &QPushButton::clicked, this, &TictactoeUI::onNewGameClicked);
}

void TictactoeUI::resetGame() {

    while (QLayoutItem* item = gridLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
    buttons.clear();

    game = Game(3);


    for (int i = 0; i < 9; ++i) {
        QPushButton* button = new QPushButton(".");
        button->setFixedSize(120, 120);
        button->setFont(QFont("Arial", 36, QFont::Bold));
        button->setStyleSheet("background-color: #F0F0F0; border: 2px solid #CCC; border-radius: 10px;");
        buttons.push_back(button);
        gridLayout->addWidget(button, i / 3, i % 3);

        connect(button, &QPushButton::clicked, this, &TictactoeUI::handleButtonClick);

    }
    setFixedSize(400, 540);
    updateBoard();

    statusLabel->setText("");

}



void TictactoeUI::handleButtonClick() {

    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    int index = std::distance(buttons.begin(), std::find(buttons.begin(), buttons.end(), clickedButton));

    if (index == -1 || !game.getBoard().isCellEmpty(index / game.getBoard().getSize(), index % game.getBoard().getSize())) {
        return;
    }



    try {
        if (game.getGameState() == GameState::InProgress) {
            game.makeMove(index / game.getBoard().getSize(), index % game.getBoard().getSize(), CellState::Cross);
            game.setCurrentPlayer(CellState::Nought);
            updateBoard();

            if (game.getGameState() != GameState::InProgress) {
                showGameOverMessage();
            } else {
                game.botMove();
                game.setCurrentPlayer(CellState::Cross);
                updateBoard();

                if (game.getGameState() != GameState::InProgress) {
                    showGameOverMessage();
                }
            }
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
        return;
    }

    if (game.getGameState() != GameState::InProgress) {
        showGameOverMessage();
    }

}




void TictactoeUI::updateBoard() {
    for (int i = 0; i < buttons.size(); ++i) {
        CellState cell = game.getBoard().getCell(i / game.getBoard().getSize(), i % game.getBoard().getSize());
        QString text = "";
        if (cell == CellState::Cross) { text = "X"; }
        if (cell == CellState::Nought) { text = "O"; }

        buttons[i]->setText(text);


        if (cell == CellState::Cross) {
            buttons[i]->setStyleSheet("background-color: #FFEBEE; border: 2px solid #E91E63; border-radius: 10px; color: #E91E63;");
        } else if (cell == CellState::Nought) {
            buttons[i]->setStyleSheet("background-color: #E8F5E9; border: 2px solid #4CAF50; border-radius: 10px; color: #4CAF50;");
        } else {
            buttons[i]->setStyleSheet("background-color: #F0F0F0; border: 2px solid #CCC; border-radius: 10px; color: #555");
        }

        buttons[i]->setEnabled(cell == CellState::Empty);

    }

}


void TictactoeUI::onNewGameClicked() {
   resetGame();
}


void TictactoeUI::showGameOverMessage() {
    QString resultMessage;

    switch (game.getGameState()) {
        case GameState::CrossWin:
            resultMessage = "You win!";
        break;
        case GameState::NoughtWin:
            resultMessage = "Bot wins!";
        break;
        case GameState::Draw:
            resultMessage = "It's a draw!";
        break;
        default:
            resultMessage = "Unknown game state";
    }

    statusLabel->setText(resultMessage);
}