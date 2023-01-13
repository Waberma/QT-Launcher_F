#include "calc.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLCDNumber>
#include <QSignalMapper>
#include <QString>
#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QFont>
#include <QColor>
#include <QPalette>
#include <QDebug>
#include <QFunctionPointer>
#include <functional>


void foo()
{
    return;
}


void CalculateMainWindow::CreateWidget()
{
    QGridLayout *Layout = new QGridLayout;
    setLayout(Layout);
    lcdNum = new QLCDNumber;
    lcdNum->setSegmentStyle(QLCDNumber::Flat);
    lcdNum->setDigitCount(15);
    lcdNum->setPalette(QPalette(QColor(105,105,105),QColor(0,225,255)));
    pb1 = new QPushButton("1");
    pb2 = new QPushButton("2");
    pb3 = new QPushButton("3");
    pb4 = new QPushButton("4");
    pb5 = new QPushButton("5");
    pb6 = new QPushButton("6");
    pb7 = new QPushButton("7");
    pb8 = new QPushButton("8");
    pb9 = new QPushButton("9");
    pb0 = new QPushButton("0");
    pbC = new QPushButton("C");
    pbPlusMin = new QPushButton("+/-");
    pbMin = new QPushButton("-");
    pbPlus = new QPushButton("+");
    pbDiv = new QPushButton("/");
    pbMult = new QPushButton("*");
    pbEqual = new QPushButton("=");
    pbPoint = new QPushButton(".");
    sb = new QStatusBar(this);
    sb->setFont(QFont("Helios",12));


    //Положение кнопок
    Layout->addWidget(pbPlus, 1, 0);
    Layout->addWidget(pbMin, 1, 1);
    Layout->addWidget(pbDiv, 1, 2);
    Layout->addWidget(pbMult, 1, 3);
    Layout->addWidget(pbEqual, 5, 3);
    Layout->addWidget(pbPoint, 5, 2);
    Layout->addWidget(lcdNum, 0, 0, 1, 5);
    Layout->addWidget(pb1, 2, 0);
    Layout->addWidget(pb2, 2, 1);
    Layout->addWidget(pb3, 2, 2);
    Layout->addWidget(pb4, 3, 0);
    Layout->addWidget(pb5, 3, 1);
    Layout->addWidget(pb6, 3, 2);
    Layout->addWidget(pb7, 4, 0);
    Layout->addWidget(pb8, 4, 1);
    Layout->addWidget(pb9, 4, 2);
    Layout->addWidget(pb0, 5, 0, 1, 2);
    Layout->addWidget(pbC, 2, 3);
    Layout->addWidget(pbPlusMin, 3, 3, 2, 1);
    Layout->addWidget(sb,6,0,1,4);

    //размер кнопок
    pb1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pb2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pb3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pb4->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pb5->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pb6->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pb7->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pb8->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pb9->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pb0->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbDiv->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbPlus->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbPlusMin->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbMin->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbMult->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbC->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pbPoint->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);


    //кнопки цифр
    mMapper = new QSignalMapper(this);
    connect(pb0,SIGNAL(clicked()), mMapper, SLOT(map()),Qt::UniqueConnection);
    connect(pb1,SIGNAL(clicked()), mMapper, SLOT(map()),Qt::UniqueConnection);
    connect(pb2,SIGNAL(clicked()), mMapper, SLOT(map()),Qt::UniqueConnection);
    connect(pb3,SIGNAL(clicked()), mMapper, SLOT(map()),Qt::UniqueConnection);
    connect(pb4,SIGNAL(clicked()), mMapper, SLOT(map()),Qt::UniqueConnection);
    connect(pb5,SIGNAL(clicked()), mMapper, SLOT(map()),Qt::UniqueConnection);
    connect(pb6,SIGNAL(clicked()), mMapper, SLOT(map()),Qt::UniqueConnection);
    connect(pb7,SIGNAL(clicked()), mMapper, SLOT(map()),Qt::UniqueConnection);
    connect(pb8,SIGNAL(clicked()), mMapper, SLOT(map()),Qt::UniqueConnection);
    connect(pb9,SIGNAL(clicked()), mMapper, SLOT(map()),Qt::UniqueConnection);

    mMapper->setMapping(pb0, (QString)"0");
    mMapper->setMapping(pb1, (QString)"1");
    mMapper->setMapping(pb2, (QString)"2");
    mMapper->setMapping(pb3, (QString)"3");
    mMapper->setMapping(pb4, (QString)"4");
    mMapper->setMapping(pb5, (QString)"5");
    mMapper->setMapping(pb6, (QString)"6");
    mMapper->setMapping(pb7, (QString)"7");
    mMapper->setMapping(pb8, (QString)"8");
    mMapper->setMapping(pb9, (QString)"9");
    connect(mMapper,SIGNAL(mapped(QString)), this, SLOT(DigitNum(QString)), Qt::UniqueConnection);

    //кнопки операций
    connect(pbPlusMin,SIGNAL(clicked()), this, SLOT(PlusMin()));
    connect(pbEqual, SIGNAL(clicked()), this, SLOT(Equal()));
    connect(pbC, SIGNAL(clicked()), this, SLOT(ClearDisplay()));
    connect(pbPoint, SIGNAL(clicked()), this, SLOT(TapPoint()));
    connect(pbPlus,SIGNAL(clicked()), this, SLOT(MathOperations()));
    connect(pbMin,SIGNAL(clicked()), this, SLOT(MathOperations()));
    connect(pbDiv,SIGNAL(clicked()), this, SLOT(MathOperations()));
    connect(pbMult,SIGNAL(clicked()), this, SLOT(MathOperations()));
    pbPlus->setCheckable(true);
    pbMin->setCheckable(true);
    pbDiv->setCheckable(true);
    pbMult->setCheckable(true);

    calcSlots.push_back(&CalculateMainWindow::tab7);
    calcSlots.push_back(&CalculateMainWindow::takeFromKeyboard);

}

CalculateMainWindow::CalculateMainWindow(QWidget *parent):QWidget(parent)
{
    resize(300,220);
    setWindowTitle("Calculate");
    CreateWidget();
    sb->setVisible(false);
    WId id = this->winId();
    QString str = "123";
    pb1->setText(QString::number(id));

}

void CalculateMainWindow::ClearDisplay()
{
    lcdNum->display(0);
    //labelNum->setText(0);
    allnums.clear();
    NextNum.clear();
    pbPlus->setChecked(false);
    pbMin->setChecked(false);
    pbMult->setChecked(false);
    pbDiv->setChecked(false);
    pbEqual->setVisible(true);
    sb->setVisible(false);
}

void CalculateMainWindow::DigitNum(QString PNum)
{
    if(pbDiv->isChecked())
    {
        if(PNum.toDouble() != 0)
        {
            pbEqual->setVisible(true);
            sb->setVisible(false);
        }
    }

    if(!pbPlus->isChecked() && !pbMin->isChecked() && !pbDiv->isChecked() && !pbMult->isChecked())
    {
        if(allnums.contains(".") && PNum == "0")
        {
            allnums += PNum;
            lcdNum->display(allnums);
        }
        else
        {
            allnums.setNum((allnums+PNum).toDouble(),'g',15);
            lcdNum->display(allnums.toDouble());
        }
    }
    else
    {
        if(NextNum.contains(".") && PNum == "0")
        {
            NextNum += PNum;
            lcdNum->display(NextNum);
        }
        else
        {
            NextNum.setNum((NextNum+PNum).toDouble(),'g',15);
            lcdNum->display(NextNum.toDouble());
        }
    }

}

void CalculateMainWindow::PlusMin()
{
    if(!pbPlus->isChecked() && !pbMin->isChecked() && !pbDiv->isChecked() && !pbMult->isChecked())
    {
        allnums.setNum(allnums.toDouble()*(-1),'g',15);
        lcdNum->display(allnums.toDouble());
    }
    else
    {
        NextNum.setNum(NextNum.toDouble()*(-1),'g',15);
        lcdNum->display(NextNum.toDouble());
    }
}

void CalculateMainWindow::MathOperations()
{
    QPushButton *butt = (QPushButton *) sender();
    lcdNum->display(0);
    butt->setChecked(true);
    if(pbDiv->isChecked())
    {
        sb->setVisible(true);
        sb->showMessage("Деление на 0 запрещено!");
        pbEqual->setVisible(false);
    }
}

void CalculateMainWindow::Equal()
{
    if(pbPlus->isChecked())
    {
        allnums.setNum(allnums.toDouble()+NextNum.toDouble(),'g',15);
        lcdNum->display(allnums.toDouble());
        pbPlus->setChecked(false);
    }else if(pbMin->isChecked())
    {
        allnums.setNum(allnums.toDouble()-NextNum.toDouble(),'g',15);
        lcdNum->display(allnums.toDouble());
        pbMin->setChecked(false);
    }else if(pbMult->isChecked())
    {
        allnums.setNum(allnums.toDouble()*NextNum.toDouble(),'g',15);
        lcdNum->display(allnums.toDouble());
        pbMult->setChecked(false);
    }else if(pbDiv->isChecked())
    {
        if(NextNum.toDouble() == 0)
        {

        }
        else
        {
            sb->showMessage("");
            allnums.setNum(allnums.toDouble()/NextNum.toDouble(),'g',15);
            lcdNum->display(allnums.toDouble());
            pbDiv->setChecked(false);
        }
    }
    NextNum.clear();
}


void CalculateMainWindow::TapPoint()
{
    if(!pbPlus->isChecked() && !pbMin->isChecked() && !pbDiv->isChecked() && !pbMult->isChecked())
    {
        if(!allnums.contains(".") && allnums.contains("0"))
        {
            allnums += ".";
            lcdNum->display(allnums);
        }else if(!allnums.contains(".") && !allnums.contains("0"))
        {
            allnums += "0.";
            lcdNum->display(allnums);
        }
    }
    else
    {
        if(!NextNum.contains(".") && NextNum.contains("0"))
        {
            NextNum += ".";
            lcdNum->display(NextNum);
        }else if(!NextNum.contains(".") && !NextNum.contains("0"))
        {
            NextNum += "0.";
            lcdNum->display(NextNum);
        }
    }
}

void CalculateMainWindow::tab7()
{
    if(!pbPlus->isChecked() && !pbMin->isChecked() && !pbDiv->isChecked() && !pbMult->isChecked())
    {
        allnums += "7";
        lcdNum->display(allnums);
    }
    else
    {
        NextNum += "7";
        lcdNum->display(NextNum);
    }
}

void CalculateMainWindow::takeFromKeyboard()
{
    emit StatusBar("Ввод с клавиатуры");
}

void CalculateMainWindow::getSlots()
{
    emit setSlots(calcSlots);
}

void CalculateMainWindow::MenuActions(QString action)
{
    if(action == "Ввести 7")
    {
        pb7->click();
    }
}
