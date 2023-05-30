#ifndef HarmAnimation_h
#define HarmAnimation_h

#include "Animation.h"
#include "../Base.h"
#include <QLabel>
#include <QtCore/qstring.h>
#include <QtGui/qpainter.h>
#include <QtWidgets/qlabel.h>

class HarmAnimation: public Animation{
public:
    HarmAnimation(Base* base,uint damage);
    bool tick(uint step) override;
    void paintHook(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    Base *obj;
    constexpr static uint total_frames=100;
    uint frame=0;
    QString text;

    double opacity=1;
    uint hOffset=0;

};

#endif
