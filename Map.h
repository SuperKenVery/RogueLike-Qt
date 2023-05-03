#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtCore/qrect.h>
#include <QtGui/qimage.h>
#include <QtWidgets/qgraphicsitem.h>
#include <vector>
#include <unordered_map>

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
    Map(vector<vector<block_type>> matrix);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    unordered_map<block_type, QImage> images;
private:
    vector<vector<block_type>> matrix;
    vector<vector<MapBlock*>> blocks;
};