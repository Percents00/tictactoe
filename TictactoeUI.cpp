// TictactoeUI.cpp
#include "TictactoeUI.h"
#include <QMessageBox>
#include <QFont>

TictactoeUI::TictactoeUI(QWidget *parent)
        : QWidget(parent), game(3),
          stackedWidget(new QStackedWidget(this)),
          mainMenuWidget(new QWidget),
          gameWidget(new QWidget),
          gridLayout(new QGridLayout),
          titleLabel(new QLabel("Крестики-нолики")),
          newGameButton(new QPushButton("Новая игра")),
          statusLabel(new QLabel), crossFirstButton(new QPushButton("Крестики первыми")),
          noughtFirstButton(new QPushButton("Нолики первыми"))
{
    setWindowTitle("TicTacToe");

    createMainMenu();
    createGameUI();

    stackedWidget->addWidget(mainMenuWidget);
    stackedWidget->addWidget(gameWidget);
    stackedWidget->setCurrentWidget(mainMenuWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);
}


void TictactoeUI::createGameUI()
{
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setStyleSheet("color: #555;");

    newGameButton->setFont(QFont("Arial", 16));
    newGameButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px; border-radius: 7px;");

    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFont(QFont("Arial", 18));



    gridLayout->setHorizontalSpacing(10);
    gridLayout->setVerticalSpacing(10);



    QVBoxLayout* gameLayout = new QVBoxLayout(gameWidget);
    gameLayout->addWidget(statusLabel);
    gameLayout->addWidget(titleLabel);
    gameLayout->addLayout(gridLayout);
    gameLayout->addWidget(newGameButton);

    connect(newGameButton, &QPushButton::clicked, this, &TictactoeUI::onNewGameClicked);
}

void TictactoeUI::createMainMenu() {
    QVBoxLayout* menuLayout = new QVBoxLayout(mainMenuWidget);



    connect(crossFirstButton, &QPushButton::clicked, this, &TictactoeUI::setCrossFirst);
    connect(noughtFirstButton, &QPushButton::clicked, this, &TictactoeUI::setNoughtFirst);




    menuLayout->addStretch();
    menuLayout->addWidget(crossFirstButton);
    menuLayout->addWidget(noughtFirstButton);
    menuLayout->addStretch();
    mainMenuWidget->setLayout(menuLayout);


    mainMenuWidget->setLayout(menuLayout);


}

void TictactoeUI::setCrossFirst() {
    startGame(CellState::Cross);
}

void TictactoeUI::setNoughtFirst() {
    startGame(CellState::Nought);
}

void TictactoeUI::startGame(CellState firstPlayer) {
    game = Game(3);
    game.setCurrentPlayer(firstPlayer);
    resetGame();
    stackedWidget->setCurrentWidget(gameWidget);

    if (firstPlayer == CellState::Nought) {
        game.botMove();
        game.setCurrentPlayer(CellState::Cross);
        updateBoard();
    }
}

void TictactoeUI::resetGame() {

    while (QLayoutItem* item = gridLayout->takeAt(0)) {
        if(item->widget() != nullptr)
        {
            delete item->widget();
        }

        delete item;
    }
    buttons.clear();




    for (int i = 0; i < 9; ++i) {
        QPushButton* button = new QPushButton(".");
        button->setFixedSize(120, 120);
        button->setFont(QFont("Arial", 36, QFont::Bold));
        button->setStyleSheet("background-color: #F0F0F0; border: 2px solid #CCC; border-radius: 10px;");
        buttons.push_back(button);
        gridLayout->addWidget(button, i / 3, i % 3);
        int row = i / 3;
        int col = i % 3;

        connect(button, &QPushButton::clicked, this, [this, row, col](){
            this->handleButtonClick(row * game.getBoard().getSize() + col);
        });


    }
    setFixedSize(400, 540);
    updateBoard();

    statusLabel->setText("");

}



void TictactoeUI::handleButtonClick(int index) {



    try {
        if (game.getGameState() == GameState::InProgress) {
            if(game.getBoard().isCellEmpty(index / game.getBoard().getSize(), index % game.getBoard().getSize()))
            {
                game.makeMove(index / game.getBoard().getSize(), index % game.getBoard().getSize(), game.getCurrentPlayer());

                if (game.getCurrentPlayer() == CellState::Cross)
                    game.setCurrentPlayer(CellState::Nought);
                else
                    game.setCurrentPlayer(CellState::Cross);

                updateBoard();
            }



            if (game.getGameState() != GameState::InProgress) {
                showGameOverMessage();
            } else {
                game.botMove();
                if (game.getCurrentPlayer() == CellState::Cross)
                    game.setCurrentPlayer(CellState::Nought);
                else
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
    CellState winner = CellState::Empty;
    GameState state = game.getGameState();

    switch (state) {
        case GameState::CrossWin:
            resultMessage = "Крестики выиграли!";
        winner = CellState::Cross;
        break;
        case GameState::NoughtWin:
            resultMessage = "Нолики выиграли!";
        winner = CellState::Nought;
        break;
        case GameState::Draw:
            resultMessage = "Ничья!";
        break;
        default:
            resultMessage = "Unknown game state";
    }


    QMessageBox msgBox(this);
    msgBox.setText(resultMessage);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes, "Играть за крестики");
    msgBox.setButtonText(QMessageBox::No, "Играть за нолики");
    int ret = msgBox.exec();



    if (ret == QMessageBox::Yes) {
        startGame(CellState::Cross);
    } else if (ret == QMessageBox::No) {
        startGame(CellState::Nought);
    }
}