#ifndef Map_h
#define Map_h

// #include "GameScene.h"
#include "player.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtCore/qrect.h>
#include <QtGui/qimage.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qgraphicsscene.h>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
using json=nlohmann::json;

class GameScene;

using namespace std;

enum block_type{
    ground,
    wall
};

class MapBlock: public QGraphicsItem {
public:
    MapBlock(QGraphicsItem *parent,QRect position,block_type type);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
private:
    QRect position;
    block_type type;
};

class Map: public QGraphicsItem {
public:
    Map(json config,GameScene *scene);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void advance(int step) override;
    unordered_map<block_type, QImage> images;
    vector<MapBlock*> walls;
private:
    vector<vector<block_type>> matrix;
    vector<vector<MapBlock*>> blocks;
    // vector<MapBlock*> walls;
};
#endif
