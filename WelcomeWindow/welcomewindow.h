#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);
    ~WelcomeWindow();

public slots:
    void startGame();
    void store();
    void continueGame();

private:
    Ui::WelcomeWindow *ui;
};

#endif // WELCOMEWINDOW_H
