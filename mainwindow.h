#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include "personpage.h"
#include "rolepage.h"
#include "mainpage.h"
#include "menupage.h"
#include "pagination.h"
#include "calc.h"
#include "tablevier.h"
#include "outapp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    typedef  void (CalculateMainWindow::*calcSlot)();  // Please do this!
signals:
    void openDB(QString);
    void MainPagesignal(QString);
    void openOption();
    void otherAppMenuAction(QString);
    void setSlots(QString);
    void getParent();
    void createaction(QString);

public slots:
    void inStatusBar(QString info);

    void takeparent(QObject *);

    void takeaction(QAction *);


private slots:
    void bd_clicked();

    void on_LogOut_clicked();

    void menuActions();

    void on_tabWidget_tabCloseRequested(int index);




private:
    Ui::MainWindow *ui;
    TableVier *tv;
    //MainPage *mg;
    QSqlDatabase db;
    QSqlTableModel *tm;
    QString proff;
    QString user = "Гость";

    QList<QString> rolelist;
    void SetUser(QString user)
    {
        this->user = user;
    };

    QString userpass;
    void SetUserpass(QString userpass)
    {
        this->userpass = userpass;
    };
    QMenuBar *cornerMenu;
    QMenu *menu;
    QMenu *cornermenu1;
    QMenu *cornermenu2;
    QTabWidget *tabwidget;

    PersonPage *personpage;
    MainPage *mainpage;
    RolePage *rolepage;
    Menupage *menupage;
    Pagination *pagin;
    CalculateMainWindow *calc;
    OutApp *OA;

    QMenu* createMenu(int ID, QMenu *menu);
    QMenu* createMenu(int ID, QMenu *menu, QList<QString> rolelist);
    QMenu* AddAppMenu(int ID, QMenu *menu,QString appname);
    QMenu* AddAppMenu(QString, QMenu *menu);
    QObject* outerparent;
    QAction* outeraction;
    void SetCornerMenu();
    void StartCreateMenu();
    void StartCreateMenu(QList<QString> rolelist);

    void ClearWindow();
    QAction *PassChangerAction;
    QAction *RegAction;

    void makeConnectionForOutapp(QObject *app);
};
#endif // MAINWINDOW_H
