#ifndef HarmAnimation_h
#define HarmAnimation_h

#include "Animation.h"
#include "../Base.h"
#include <QLabel>
#include <QtWidgets/qlabel.h>

class HarmAnimation: public Animation{
public:
    HarmAnimation(Base* base,uint damage);
    bool tick(uint step) override;

    Base *obj;
    constexpr static uint total_frames=100;
    uint frame=0;
    QLabel *label;

};

#endif
