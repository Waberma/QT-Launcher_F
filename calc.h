#ifndef CALCULATEMAINWINDOW_H
#define CALCULATEMAINWINDOW_H

#include <QWidget>
#include <functional>
#include <QFunctionPointer>
#include <QMenu>
#include <QAction>
class QPushButton;
class QLCDNumber;
class QSignalMapper;
class QLabel;
class QStatusBar;

class CalculateMainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CalculateMainWindow(QWidget *parent = 0);
    QString ininame = "calc.ini";
    //~CalculateMainWindow();
    void MenuActions(QString action);

    //pointer-to-member-function
    typedef  void (CalculateMainWindow::*calcSlot)();  // Please do this!
    QList<void (CalculateMainWindow::*)()> calcSlots;
    QMap<QString, calcSlot> slotmap;

signals:
    void StatusBar(QString);
    void setSlots(QList<void (CalculateMainWindow::*)()> calcSlots);
    void sendParent(QObject *);
    void sendAction(QAction *);

private:
    void CreateWidget();
    QString NextNum;
    QSignalMapper *mMapper;
    QString allnums;
private:
    QPushButton *pb1;
    QPushButton *pb2;
    QPushButton *pb3;
    QPushButton *pb4;
    QPushButton *pb5;
    QPushButton *pb6;
    QPushButton *pb7;
    QPushButton *pb8;
    QPushButton *pb9;
    QPushButton *pb0;
    QPushButton *pbPlusMin;
    QPushButton *pbPlus;
    QPushButton *pbMin;
    QPushButton *pbMult;
    QPushButton *pbDiv;
    QPushButton *pbC;
    QPushButton *pbEqual;
    QPushButton *pbPoint;
    QLCDNumber *lcdNum;
    QStatusBar *sb;
private slots:
    void DigitNum(QString PNum);
    void MathOperations();
    void Equal();
    void PlusMin();
    void ClearDisplay();
    void TapPoint();
public slots:
    void getSlots(QString a);
    void tab7();
    void takeFromKeyboard();
    void parent();
    void action(QString);
};

#endif // CALCULATEMAINWINDOW_H
