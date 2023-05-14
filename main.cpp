// #include "mainwindow.h"
#include "GameScene/GameScene.h"
#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsView>
#include <QtWidgets/qgraphicsview.h>
#include <cstdio>

int main(int argc, char *argv[])
{
    printf("main...\n");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
