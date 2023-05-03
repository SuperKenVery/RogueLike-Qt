#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtCore/qtmetamacros.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qwidget.h>

class GameScene: public QGraphicsScene{
    Q_OBJECT
public:
    GameScene(QWidget *parent=nullptr);
};