#include "outapp.h"
#include "ui_outapp.h"


OutApp::OutApp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutApp)
{
    ui->setupUi(this);
    slotmap.insert("Слот 111", &OutApp::ChangeL1);
    slotmap.insert("Слот 1", &OutApp::ChangeL2);
}

OutApp::~OutApp()
{
    delete ui;
    //ChangeL1();

}

void OutApp::parent()
{
    auto *parent = new QObject();
    QList<QString> MenActions;
    MenActions.append("ВПО");
    MenActions.append("ВПО/Слот1");
    MenActions.append("ВПО/Слот1/Слот11");
    MenActions.append("ВПО/Слот1/Слот11/Слот 111/");
    MenActions.append("ВПО/Слот 1/");

    for (std::size_t i = 0; i < MenActions.size(); ++i) {
        auto obj = new QObject(parent);
        if(i < MenActions.size())
            obj->setObjectName(MenActions.at(i));
    }
    sendParent(parent);
}

void OutApp::action(QString name)
{
    QAction *action = new QAction(name, this);
    connect(action, &QAction::triggered, this, slotmap.value(name));
    sendAction(action);
}

void OutApp::ChangeL1()
{
    ui->label->setText("Слот 1");
}

void OutApp::ChangeL2()
{
    ui->label->setText("Слот 2");
}
