#ifndef RESUMEFAILEDWINDOW_H
#define RESUMEFAILEDWINDOW_H

#include <QDialog>

namespace Ui {
class ResumeFailedWindow;
}

class ResumeFailedWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ResumeFailedWindow(QWidget *parent = nullptr);
    ~ResumeFailedWindow();

private:
    Ui::ResumeFailedWindow *ui;
};

#endif // RESUMEFAILEDWINDOW_H
