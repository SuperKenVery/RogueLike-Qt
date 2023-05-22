// #include "mainwindow.h"
#include "WelcomeWindow/welcomewindow.h"

#include <QApplication>
#include <QGraphicsView>
#include <QtWidgets/qgraphicsview.h>
#include <cstdio>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WelcomeWindow w;
    w.show();

    return a.exec();
}
