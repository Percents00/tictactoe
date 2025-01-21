// TictactoeUI.cpp
#include "TictactoeUI.h"
#include <QMessageBox>
#include <QFont>
#include <QIntValidator>
#include <QApplication>
#include <QScreen>

TictactoeUI::TictactoeUI(QWidget *parent)
        : QWidget(parent), game(3),
          stackedWidget(new QStackedWidget(this)),
          mainMenuWidget(new QWidget),
          gameWidget(new QWidget),
          gridLayout(new QGridLayout),
          titleLabel(new QLabel("Крестики-нолики")),
          statusLabel(new QLabel), crossFirstButton(new QPushButton("Крестики первыми")),
          noughtFirstButton(new QPushButton("Нолики первыми")),
          sizeInput(new QLineEdit)
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


void TictactoeUI::createMainMenu() {
    QVBoxLayout* menuLayout = new QVBoxLayout(mainMenuWidget);

    sizeInput->setPlaceholderText("Введите размер поля (например, 3)");
    sizeInput->setValidator(new QIntValidator(3, 10, this));
    crossFirstButton->setText("Играть первым ходом");
    noughtFirstButton->setText("Играть вторым ходом");

    crossFirstButton->setFont(QFont("Arial", 16));
    crossFirstButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px; border-radius: 7px;");

    noughtFirstButton->setFont(QFont("Arial", 16));
    noughtFirstButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px; border-radius: 7px;");

    menuLayout->addStretch();
    menuLayout->addWidget(new QLabel("Выберите размер поля:"));
    menuLayout->addWidget(sizeInput);
    menuLayout->addWidget(crossFirstButton);
    menuLayout->addWidget(noughtFirstButton);
    menuLayout->addStretch();

    connect(crossFirstButton, &QPushButton::clicked, this, &TictactoeUI::setCrossFirst);
    connect(noughtFirstButton, &QPushButton::clicked, this, &TictactoeUI::setNoughtFirst);

    mainMenuWidget->setLayout(menuLayout);
}


void TictactoeUI::startGame(CellState firstPlayer) {
    int size = sizeInput->text().toInt();
    game = Game(size);
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
        if(item->widget() != nullptr) {
            delete item->widget();
        }
        delete item;
    }
    buttons.clear();

    int size = game.getBoard().getSize();

    int buttonSize = (size <= 5) ? 120 : (size <= 7) ? 80 : 60;

    for (int i = 0; i < size * size; ++i) {
        QPushButton* button = new QPushButton(".");
        button->setFixedSize(buttonSize, buttonSize);
        button->setFont(QFont("Arial", 24, QFont::Bold));
        button->setStyleSheet("background-color: #F0F0F0; border: 2px solid #CCC; border-radius: 10px;");
        buttons.push_back(button);
        gridLayout->addWidget(button, i / size, i % size);

        connect(button, &QPushButton::clicked, this, [this, i, size](){
            this->handleButtonClick(i);
        });
    }

    int windowWidth = buttonSize * size + 50;
    int windowHeight = buttonSize * size + 150;

    setMinimumSize(windowWidth, windowHeight);
    setMaximumSize(windowWidth * 2, windowHeight * 2);

    adjustSize();
    move(QApplication::primaryScreen()->geometry().center() - rect().center());

    updateBoard();
    statusLabel->setText("");
}

void TictactoeUI::handleButtonClick(int index) {
    try {
        if (game.getGameState() == GameState::InProgress) {
            int size = game.getBoard().getSize();
            if(game.getBoard().isCellEmpty(index / size, index % size)) {
                game.makeMove(index / size, index % size, game.getCurrentPlayer());

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
    int size = game.getBoard().getSize();
    for (int i = 0; i < buttons.size(); ++i) {
        CellState cell = game.getBoard().getCell(i / size, i % size);
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
            resultMessage = "Неизвестное состояние игры";
    }

    QMessageBox msgBox(this);
    msgBox.setText(resultMessage);

    QPushButton* playCrossButton = msgBox.addButton("Играть за крестики", QMessageBox::YesRole);
    QPushButton* playNoughtButton = msgBox.addButton("Играть за нолики", QMessageBox::NoRole);
    QPushButton* returnToMenuButton = msgBox.addButton("В главное меню", QMessageBox::ActionRole);
    msgBox.setDefaultButton(playCrossButton);

    msgBox.exec();

    QPushButton* clickedButton = qobject_cast<QPushButton*>(msgBox.clickedButton());

    if (clickedButton == playCrossButton) {
        startGame(CellState::Cross);
    } else if (clickedButton == playNoughtButton) {
        startGame(CellState::Nought);
    } else if (clickedButton == returnToMenuButton) {
        stackedWidget->setCurrentWidget(mainMenuWidget);
    } else {
        return;
    }

    msgBox.close();
}

void TictactoeUI::setCrossFirst() {
    QString sizeText = sizeInput->text().trimmed();
    if (sizeText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Поле размера не может быть пустым.");
        return;
    }

    int size = sizeText.toInt();
    if (size >= 3 && size <= 10) {
        startGame(CellState::Cross);
    } else {
        QMessageBox::warning(this, "Ошибка", "Размер поля должен быть от 3 до 10.");
    }
}

void TictactoeUI::setNoughtFirst() {
    QString sizeText = sizeInput->text().trimmed();
    if (sizeText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Поле размера не может быть пустым.");
        return;
    }

    int size = sizeText.toInt();
    if (size >= 3 && size <= 10) {
        startGame(CellState::Nought);
    } else {
        QMessageBox::warning(this, "Ошибка", "Размер поля должен быть от 3 до 10.");
    }
}

void TictactoeUI::createGameUI() {
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setStyleSheet("color: #555;");

    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFont(QFont("Arial", 18));

    gridLayout->setHorizontalSpacing(10);
    gridLayout->setVerticalSpacing(10);

    QVBoxLayout* gameLayout = new QVBoxLayout(gameWidget);
    gameLayout->addWidget(statusLabel);
    gameLayout->addWidget(titleLabel);
    gameLayout->addLayout(gridLayout);
}
