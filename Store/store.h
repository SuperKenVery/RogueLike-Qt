#ifndef STORE_H
#define STORE_H

#include <QDialog>
#include <fstream>
#include <nlohmann/json.hpp>
using json=nlohmann::json;
using namespace std;

namespace Ui {
class Store;
}

class Store : public QDialog
{
    Q_OBJECT

public:
    explicit Store(QWidget *parent = nullptr);
    ~Store();

private:
    void buildEnhanceWidgets();
    Ui::Store *ui;
    json config,storage;
};

#endif // STORE_H
