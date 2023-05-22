#ifndef Base_h
#define Base_h

#include "Animations/Animation.h"
#include "GameScene.h"
#include <QGraphicsItem>
#include <QtCore/qrect.h>
#include <QtGui/qimage.h>
#include <QtWidgets/qgraphicsitem.h>
#include <vector>
#include <nlohmann/json.hpp>
using namespace std;
using json=nlohmann::json;

class Base: public QGraphicsItem{
public:
    Base(uint life, uint size, QImage image, QGraphicsItem* parent=nullptr);

    virtual uint harm(uint damage);
    bool valid();
    virtual void die()=0;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;
    virtual json dumpState()=0;
    virtual void resumeState(json storage)=0;
    QRectF boundingRect() const override final;
    void advance(int step) override;
    vector<Animation*> animations;
protected:
    uint life,size;
    QImage image;
};
#endif
