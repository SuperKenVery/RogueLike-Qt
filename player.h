#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include<QImage>
#include<QVector2D>
#include <QtCore/qtmetamacros.h>
#include <QtWidgets/qwidget.h>

class Player : public QGraphicsItem
{
public:
    Player(QImage image);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QImage image;

protected:
    void advance(int step) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QVector2D direction=QVector2D(0,0);

    int speed=10;

};

#endif // PLAYER_H
