#ifndef HarmAnimation_h
#define HarmAnimation_h

#include "Animation.h"
#include "../Base.h"
#include <QLabel>
#include <QtCore/qrect.h>
#include <QtCore/qstring.h>
#include <QtGui/qpainter.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qlabel.h>

class HarmAnimation: public Animation{
public:
    HarmAnimation(Base* base,uint damage,bool debug=false);
    void advance(int step) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    Base *obj;
    constexpr static uint total_frames=100;
    uint frame=0;
    QString text;

    double opacity=1;
    uint hOffset=0;
    const int height=100;
private:
    bool debug=false;

};

#endif
