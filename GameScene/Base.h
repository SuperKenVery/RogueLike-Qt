#ifndef Base_h
#define Base_h

#include <QGraphicsItem>
#include <QtCore/qrect.h>
#include <QtGui/qimage.h>
#include <QtWidgets/qgraphicsitem.h>
#include <vector>
using namespace std;

class Base: public QGraphicsItem{
public:
    Base(uint life, uint size, QImage image, QGraphicsItem* parent=nullptr);

    uint harm(uint damage);
    bool valid();
    virtual void die()=0;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;
    QRectF boundingRect() const override final;
protected:
    uint life,size;
    QImage image;
};
#endif
