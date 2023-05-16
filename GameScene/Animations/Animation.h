#ifndef Animation_h
#define Animation_h

#include <QPainter>
#include <QStyleOption>
#include <QWidget>

typedef unsigned int uint;

class Animation {
public:
    Animation();
    ~Animation();
    /* Run the animation
     * Must be overridden by subclass
     *
     * @param step How many steps to run
     * @return Whether the animation is finished
    */
    virtual bool tick(uint step)=0;

    virtual void paintHook(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
        // Does nothing by default
    }

    /* Whether this animation totally overrides the rendering of the object
     *
     * An object shouldn't have two animation with override_rendering=true
     *
     * Don't seem to need it currently...
    */
    // bool override_rendering=false;
};

#endif
