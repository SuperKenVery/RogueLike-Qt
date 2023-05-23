#ifndef DIEWINDOW_H
#define DIEWINDOW_H

#include <QDialog>

namespace Ui {
class DieWindow;
}

class DieWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DieWindow(uint seconds, uint coins,QWidget *parent = nullptr);
    ~DieWindow();

private:
    Ui::DieWindow *ui;
};

#endif // DIEWINDOW_H
