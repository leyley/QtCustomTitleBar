#include "mainwindow.h"
#include "windowframe.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //w.show();
    WindowFrame w(nullptr,new MainWindow());
    w.show();
    return a.exec();
}
