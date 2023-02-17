#ifndef OUTAPP_H
#define OUTAPP_H

#include <QWidget>
#include <functional>
#include <QFunctionPointer>
#include <QMenu>
#include <QAction>
namespace Ui {
class OutApp;
}

class OutApp : public QWidget
{
    Q_OBJECT
signals:
    void sendParent(QObject *);
    void sendAction(QAction *);
public:
    explicit OutApp(QWidget *parent = nullptr);
    ~OutApp();
    typedef  void (OutApp::*OASlot)();
    QMap<QString, void (OutApp::*)()> slotmap;
public slots:
    void parent();
    void action(QString);
    void ChangeL1();
    void ChangeL2();
private:
    Ui::OutApp *ui;
};

#endif // OUTAPP_H
