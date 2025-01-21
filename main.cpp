#include <QApplication>
#include "TictactoeUI.h"
#include "tests/tests.h"

int main(int argc, char *argv[]) {
    try {
        //runBoardTests();
        //runBotTests();

        QApplication a(argc, argv);
        TictactoeUI w;
        w.show();
        return a.exec();
    }
    catch (const char* message)
    {
        std::cerr << message << std::endl;
        return 1;
    }

}