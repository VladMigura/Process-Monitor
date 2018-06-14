#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWin;

    mainWin.setPrivileges();
    mainWin.show();
    mainWin.start();

    return a.exec();
}
