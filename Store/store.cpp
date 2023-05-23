#include "store.h"
#include "ui_store.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QtCore/qnamespace.h>
#include <QtCore/qstring.h>
#include <QtCore/qtypes.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qwidget.h>
#include <algorithm>
#include <filesystem>
#include <QGroupBox>
#include <QTabWidget>
#include <string>

QLayout* wrapper(QWidget *w){
    auto layout=new QVBoxLayout();
    layout->addWidget(w);
    return layout;
}

QWidget* wrapper(QLayout *l){
    auto w=new QWidget();
    w->setLayout(l);
    return w;
}

/*
    buildEnhanceOptionLayout: build a layout containing buttons for enhancements

    QString namer(QString): Given config item name, returns the enhance button name
        Example:
        [&](QString name){
            return name+"+50%";
        }

    callback(int index)
        index: the index of the config item in given config.
        Example callback:
        [&](int index){
            auto optionStr=playerEnhanceOptions[index].toStdString();
            storage["players"][playerIndex][optionStr]*=1.5;
        }
*/
QLayout* buildEnhanceOptionLayout(QWidget *parent,vector<QString> options,function<QString(QString)> namer,function<void(int index)> callback){
    const auto maxWidth=5;
    auto grid=new QGridLayout(parent);

    auto y=0,x=0;
    for(auto optionIndex=0;optionIndex<options.size();optionIndex++){
        auto option=options[optionIndex];
        auto button=new QPushButton(namer(option),parent);
        parent->connect(button,&QPushButton::clicked,parent,[=](){
            callback(optionIndex);
        });
        grid->addWidget(button,y,x);
        x++;
        if(x==maxWidth){
            x=0;
            y++;
        }
    }

    return grid;
}

/*
    getEnhanceOptions: extract keys from config, filtering items in blackList
*/
vector<QString> getEnhanceOptions(json config, vector<string> blackList){
    auto enhanceOptions=vector<QString>();
    for(auto i:config.items()){
        string key=i.key();
        if(std::find(blackList.begin(),blackList.end(),key) != blackList.end())
            continue;

        enhanceOptions.push_back(key.c_str());
    }
    return enhanceOptions;
}

Store::Store(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Store)
{
    // When you're in store you won't be in game
    // So just open the files.

    // Reload everything on next open
    this->setAttribute(Qt::WA_DeleteOnClose);

    auto configFileStream=ifstream("config.json");
    configFileStream >> this->config;

    this->storageFileStream.open("storage.json");
    this->storageFileStream >> this->storage;

    ui->setupUi(this);

    this->buildEnhanceWidgets();

    uint coins=this->storage["coins"];
    ui->remainingCoinLabel->setText("Coins: "+QString::number(coins));
}

void Store::buildEnhanceWidgets(){
    auto &config=this->config;
    auto &storage=this->storage["configOverride"];

    auto playerTabber=new QTabWidget(this);

    auto playerEnhanceOptions=getEnhanceOptions(config["players"][1], {"enhance_hp","weapon","image","name","size"});
    auto weaponEnhanceOptions=getEnhanceOptions(config["players"][1]["weapon"], {});
    const auto useCoins=10; // How many coins does an enhancement use

    // Iterate through players
    for(auto playerIndex=1;playerIndex<config["players"].size();playerIndex++){
        auto vbox=new QVBoxLayout(this);

        // Player enhance options
        auto playerEnhancements=buildEnhanceOptionLayout(this, playerEnhanceOptions,
            [](QString optionName){
                return optionName+" 1.5x";
            }
            ,[&](int index){
                if(this->storage["coins"]<useCoins){
                    // Can't enhance
                }else{
                    auto optionStr=playerEnhanceOptions[index].toStdString();
                    double originalValue=storage["players"][playerIndex][optionStr];
                    storage["players"][playerIndex][optionStr]=1.5*originalValue;

                    uint oriCoins=this->storage["coins"];
                    this->storage["coins"]=oriCoins-useCoins;

                    uint coins=this->storage["coins"];
                    ui->remainingCoinLabel->setText("Coins: "+QString::number(coins));
                }
            }
        );
        string playerName=config["players"][playerIndex]["name"];
        vbox->addLayout(playerEnhancements);

        // Weapon enhance options
        auto weaponEnhancements=buildEnhanceOptionLayout(this, weaponEnhanceOptions,
            [](QString optionName){
                return optionName+" 1.5x";
            },
            [=,&storage](int index){
                if(this->storage["coins"]<useCoins){
                    // Can't enhance
                }else{
                    auto optionStr=weaponEnhanceOptions[index].toStdString();
                    double originalValue=storage["players"][playerIndex]["weapon"][optionStr];
                    storage["players"][playerIndex]["weapon"][optionStr]=1.5*originalValue;

                    uint oriCoins=this->storage["coins"];
                    this->storage["coins"]=oriCoins-useCoins;

                    uint coins=this->storage["coins"];
                    ui->remainingCoinLabel->setText("Coins: "+QString::number(coins));
                }
            }
        );
        vbox->addLayout(weaponEnhancements);

        playerTabber->addTab(wrapper(vbox),playerName.c_str());
    }

    auto centralize=new QBoxLayout(QBoxLayout::LeftToRight,this);
    centralize->addWidget(playerTabber);
    centralize->setAlignment(Qt::AlignCenter);

}

Store::~Store()
{
    delete ui;
    this->storageFileStream << this->storage.dump();
    this->storageFileStream.close();
}
