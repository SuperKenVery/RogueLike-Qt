#ifndef EnhanceOption_h
#define EnhanceOption_h
#include <QLabel>
#include <QString>
#include <QtCore/qtmetamacros.h>
#include <QtGui/qevent.h>
#include <QtWidgets/qwidget.h>
#include <string>
#include <functional>
#include "EnhancePanel.h"
using namespace std;

class EnhanceOption: public QLabel {
    Q_OBJECT
public:
    EnhanceOption(enhance_action action,QString msg,  EnhancePanel *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    enhance_action action;
    EnhancePanel *panel;
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

#endif
