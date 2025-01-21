// main.cpp
#include <QApplication>
#include "TictactoeUI.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    TictactoeUI w;
    w.show();
    return a.exec();
}