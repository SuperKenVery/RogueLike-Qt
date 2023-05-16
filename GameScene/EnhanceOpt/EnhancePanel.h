#ifndef EnhancePanel_h
#define EnhancePanel_h
#include <QWidget>
#include <QtCore/qstring.h>
#include <QtWidgets/qwidget.h>
#include <vector>

using namespace std;

class Player;
class Weapon;
class EnhanceOption;
typedef function<void()> enhance_action;
typedef function<void()> enhance_callback;

class EnhancePanel: public QWidget {
    Q_OBJECT
public:
    EnhancePanel(vector<pair<QString,enhance_action>> enhancements,enhance_callback cb,QWidget *parent=nullptr);
    ~EnhancePanel();
    /* enhanced()
    *  Enhancement has bee chosen, close the panel
    */
    void enhanced();
    vector<EnhanceOption*> options;
    enhance_callback cb;
};

#endif
