#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QTextStream>
#include <QList>
#include <QFunctionPointer>
#include <QAction>
#include <QSqlRecord>
#include <QTabWidget>
#include <QFileDialog>
#include <QSettings>
#include <QVariant>
#include <typeinfo>
//Модели и интервью

bool isIn(QList<QString> &list, QString el)
{
    for(size_t i = 0; i < list.size(); i++)
    {
        if(list.at(i) == el)
        {
            return true;
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./menu.db");
    if(db.open())
    {
        qDebug("DB Open");
    }
    else
    {
        qDebug("NoOpen");
    }
    ui->tabWidget->setTabBarAutoHide(true);
    ui->tabWidget->setTabsClosable(true);

    mainpage = new MainPage;
    personpage = new PersonPage;
    rolepage = new RolePage;
    menupage = new Menupage;
    pagin = new Pagination;
    calc = new CalculateMainWindow;
    OA = new OutApp;

    //Связи с RolePage
    connect(rolepage, &RolePage::statusBar, this, &MainWindow::inStatusBar);
    connect(rolepage, &RolePage::StartCreateMenu, this,  static_cast<void (MainWindow::*)(QList<QString> rolelist)> (&MainWindow::StartCreateMenu));
    connect(mainpage,  &MainPage::setUser, rolepage, &RolePage::SetUser);

    //Связи с PersonPage
    connect(personpage, &PersonPage::statusBar, this, &MainWindow::inStatusBar);
    connect(personpage, &PersonPage::StartCreateMenu, this,  static_cast<void (MainWindow::*)(QList<QString> rolelist)> (&MainWindow::StartCreateMenu));
    connect(mainpage,  &MainPage::setUser, personpage, &PersonPage::SetUser);

    //Связи с MainPage
    connect(this, &MainWindow::MainPagesignal, mainpage, &MainPage::menuActions);
    connect(mainpage, &MainPage::statusBar, this, &MainWindow::inStatusBar);
    connect(mainpage, &MainPage::setUser, this, &MainWindow::SetUser);
    connect(mainpage, &MainPage::setUserpass, this, &MainWindow::SetUserpass);
    connect(mainpage, &MainPage::SetCornerMenu, this, &MainWindow::SetCornerMenu);
    connect(mainpage, &MainPage::StartCreateMenu, this,  static_cast<void (MainWindow::*)(QList<QString> rolelist)> (&MainWindow::StartCreateMenu));

    //Связи с MenuPage
    connect(mainpage, &MainPage::setUser, menupage, &Menupage::SetUser);
    connect(menupage, &Menupage::statusBar, this, &MainWindow::inStatusBar);
    connect(menupage, &Menupage::StartCreateMenu, this,  static_cast<void (MainWindow::*)(QList<QString> rolelist)> (&MainWindow::StartCreateMenu));

    connect(this, &MainWindow::openOption, pagin, &Pagination::openOption);

    ui->tabWidget->addTab(mainpage,"Главная страница");
    SetCornerMenu();

    //Связи с CalculateMainWindow
//    connect(this, SIGNAL(getParent()), calc, SLOT(parent()));
//    connect(calc, SIGNAL(sendParent(QObject*)), this, SLOT(takeparent(QObject*)));
//    connect(this, SIGNAL(createaction(QString)), calc, SLOT(action(QString)));
//    connect(calc, SIGNAL(sendAction(QAction*)), this, SLOT(takeaction(QAction*)));
    connect(calc, &CalculateMainWindow::StatusBar, this, &MainWindow::inStatusBar);

}

void MainWindow::makeConnectionForOutapp(QObject *app)
{
    connect(this, SIGNAL(getParent()), app, SLOT(parent()));
    connect(app, SIGNAL(sendParent(QObject*)), this, SLOT(takeparent(QObject*)));
    connect(this, SIGNAL(createaction(QString)), app, SLOT(action(QString)));
    connect(app, SIGNAL(sendAction(QAction*)), this, SLOT(takeaction(QAction*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::bd_clicked()
{
    tv = new TableVier;
    tv->show();
}

void MainWindow::on_LogOut_clicked()
{
    user = "Гость";
    ui->menubar->clear();
    ui->menubar->cornerWidget()->close();
    SetCornerMenu();
}

void MainWindow::menuActions()
{
    QAction *action = (QAction *) sender();
    emit MainPagesignal(action->text());
    if(action->text() == "Автор")
    {
        bd_clicked();
    }
    if(action->text() == "Дневник")
    {
        ui->tabWidget->addTab(mainpage,"Главная страница");
    }
    if(action->text() == "Выход")
    {
        on_LogOut_clicked();
    }
    if(action->text() == "Пользователи")
    {
        ui->tabWidget->addTab(personpage,"Пользователи");
        ui->tabWidget->setCurrentWidget(personpage);
    }
    if(action->text() == "Роли")
    {
        ui->tabWidget->addTab(rolepage,"Роли");
        ui->tabWidget->setCurrentWidget(rolepage);
    }
    if(action->text() == "Объекты меню")
    {
        ui->tabWidget->addTab(menupage,"Объекты меню");
        ui->tabWidget->setCurrentWidget(menupage);
    }
    if(action->text() == "Таблица")
    {
        if(pagin->isHidden())
        {
            ui->tabWidget->addTab(pagin, "Таблица закупок");
            ui->tabWidget->setCurrentWidget(pagin);
            QMenu *menu = new QMenu(NULL);
            ui->menubar->addMenu(AddAppMenu(0,menu,"Pagination"));
        }
    }
    if(action->text() == "Настройки" && ui->tabWidget->currentWidget() == pagin)
    {
        emit openOption();
    }
    if(action->text() == "Импорт")
    {
        QString ininame = QFileDialog::getOpenFileName(
                    this,
                    tr("Open Ini File"),
                    "E://QT Project//build-MainWindow-Desktop_Qt_5_14_2_MinGW_32_bit-Debug//INI//"
                    );
        QSettings sett(ininame, QSettings::IniFormat);
        sett.setIniCodec("UTF-8"); // кодировка
        int size = sett.value("0/Num", "").toInt(); //получить опцию

        QSqlQuery *insertq = new QSqlQuery(db);
        for(int i = 1; i < 1+size; i++)
        {
            QString insert;
            insert += "'" + sett.value(QString::number(i) + "/Title", "").toString() + "'" + ",";
            insert += "'" + sett.value(QString::number(i) + "/Name", "").toString() + "'" + ",";
            insert += sett.value(QString::number(i) + "/Parent", "").toString() + ",";
            insert += "'" + sett.value(QString::number(i) + "/AppName", "").toString() + "'" + ",";
            insert += sett.value(QString::number(i) + "/Role", "").toString();
            insertq->exec("INSERT INTO MenuItems(Title, Name, Parent, AppName, Role) "
                          "Values(" + insert +");");
        }
    }
    if(action->text() == "Калькулятор")
    {
        ui->tabWidget->addTab(calc, "Калькулятор");
        ui->tabWidget->setCurrentWidget(calc);
        makeConnectionForOutapp(calc);
        QMenu *menu = new QMenu(NULL);
        ui->menubar->addMenu(AddAppMenu(NULL, menu));
    }
    if(action->text() == "ВПО")
    {
        ui->tabWidget->addTab(OA, "ВПО");
        ui->tabWidget->setCurrentWidget(OA);
        makeConnectionForOutapp(OA);
        QMenu *menu = new QMenu(NULL);
        ui->menubar->addMenu(AddAppMenu(NULL, menu));
    }
}

QMenu* MainWindow::createMenu(int ID, QMenu *menu)
{
    QSqlQuery *menuq = new QSqlQuery(db);
    menuq->prepare("SELECT ID, title, parent, appName "
                    "FROM MenuItems "
                    "WHERE parent = :ID AND (Role IS NULL OR Role = '');");
    menuq->bindValue(":ID",ID);
    menuq->exec();

    QSqlRecord rec = menuq->record();
    const int titleIndex = rec.indexOf("title");
    const int appNameIndex = rec.indexOf("appName");
    const int IDIndex = rec.indexOf("ID");

    while(menuq->next())
    {
        if(menuq->value(appNameIndex).isNull() || menuq->value(appNameIndex).toString() == "")
        {
                QMenu *menu2 = new QMenu(menuq->value(titleIndex).toString());
                menu->addMenu(createMenu(menuq->value(IDIndex).toInt(), menu2));
        }
        else
        {
                QAction *action = new QAction(menuq->value(titleIndex).toString(), this);
                connect(action, SIGNAL(triggered()), this, SLOT(menuActions()));
                menu->addAction(action);
        }
    }
    return menu;
}

void MainWindow::SetCornerMenu()
{
    if(user != "Гость")
    {
        ui->menubar->cornerWidget()->close();
        QSqlQuery *cornerMenuq = new QSqlQuery(db);
        cornerMenuq->exec("SELECT * FROM CornerMenu "
                          "WHERE Active IS NOT NULL;");
        cornerMenu = new QMenuBar(ui->menubar);
        cornermenu1 = new QMenu("Добро пожаловать, "+user,this);
        while(cornerMenuq->next())
        {
            QAction *userAction = new QAction(cornerMenuq->value(cornerMenuq->record().indexOf("Name")).toString(),this);
            connect(userAction, SIGNAL(triggered()), this, SLOT(menuActions()));
            cornermenu1->addAction(userAction);
        }
        cornerMenu->addMenu(cornermenu1);
        ui->menubar->setCornerWidget(cornerMenu);
    }
    else
    {
        QSqlQuery *cornerMenuq = new QSqlQuery(db);
        cornerMenuq->exec("SELECT * FROM CornerMenu "
                          "WHERE Active IS NULL;");
        cornerMenu = new QMenuBar(ui->menubar);
        cornermenu1 = new QMenu("Добро пожаловать, "+user,this);
        while(cornerMenuq->next())
        {
            QString a = cornerMenuq->value(cornerMenuq->record().indexOf("Name")).toString();
            QAction *userAction = new QAction(cornerMenuq->value(cornerMenuq->record().indexOf("Name")).toString(),this);
            connect(userAction, SIGNAL(triggered()), this, SLOT(menuActions()));
            cornermenu1->addAction(userAction);
        }
        cornerMenu->addMenu(cornermenu1);
        ui->menubar->setCornerWidget(cornerMenu);
    }
    ui->menubar->cornerWidget()->show();
}

QMenu* MainWindow::createMenu(int ID, QMenu *menu, QList<QString> rolelist)
{
    createMenu(ID,menu);
    QSqlQuery *roleMenuq = new QSqlQuery(db);
    QString role;
    QString comma;
    for(auto i = rolelist.begin(); i != rolelist.end(); ++i)
    {
        role += comma + *i;
        comma = ",";
    }
    roleMenuq->prepare("Select DISTINCT MenuItems.ID, MenuItems.Title, MenuItems.parent, MenuItems.AppName "
              "FROM MenuItems "
              "LEFT JOIN RoleItems "
                "ON MenuItems.ID = RoleItems.ItemID "
              "WHERE RoleItems.RoleID IN (" + role + ") AND parent = :ID;");
    roleMenuq->bindValue(":ID", ID);
    roleMenuq->exec();
    QSqlRecord rec = roleMenuq->record();
    int titleIndex = rec.indexOf("title");
    int appNameIndex = rec.indexOf("appName");
    int IDIndex = rec.indexOf("ID");
    while(roleMenuq->next())
    {
        if(roleMenuq->value(roleMenuq->record().indexOf("appName")).isNull() || roleMenuq->value(appNameIndex).toString() == "")
        {
                QMenu *menu2 = new QMenu(roleMenuq->value(titleIndex).toString());
                menu->addMenu(createMenu(roleMenuq->value(IDIndex).toInt(), menu2, rolelist));
        }
        else
        {
                QAction *action = new QAction(roleMenuq->value(titleIndex).toString(), this);
                connect(action, SIGNAL(triggered()), this, SLOT(menuActions()));
                menu->addAction(action);
        }
    }
    return menu;
}

QMenu *MainWindow::AddAppMenu(int ID, QMenu *menu, QString appname)
{
    if(ID == 0)
    {
        QSqlQuery *titleq = new QSqlQuery(db);
        titleq->exec("SELECT ID,title,parent FROM MenuItems "
                     "Where parent IS NULL AND Name = '"+appname+"';");
        titleq->next();
        QString a = titleq->record().value("title").toString();
        menu = new QMenu(titleq->record().value("title").toString());
        ID = titleq->record().value("ID").toInt();
    }
    QSqlQuery *menuq = new QSqlQuery(db);
    menuq->prepare("SELECT ID, title, parent, appName "
                    "FROM MenuItems "
                    "WHERE parent = :ID;");
    menuq->bindValue(":ID",ID);
    menuq->exec();

    while(menuq->next())
    {
        if(menuq->record().value("appName").isNull() || menuq->value("appName").toString() == "")
        {
                QMenu *menu2 = new QMenu(menuq->record().value("title").toString());
                menu->addMenu(AddAppMenu(menuq->record().value("ID").toInt(), menu2, ""));
        }
        else
        {
                QAction *action = new QAction(menuq->record().value("title").toString(), this);
                connect(action, SIGNAL(triggered()), this, SLOT(menuActions()));
                menu->addAction(action);
        }
    }
    return menu;
}

void MainWindow::takeparent(QObject * parent)
{
    outerparent = parent;
}

void MainWindow::takeaction(QAction *action)
{
    outeraction = action;
}

QList<QString> alredyIn;
QMenu *MainWindow::AddAppMenu(QString child, QMenu *menu)
{
    QList<QObject> childrens;
    emit getParent();
    if(child == NULL)
    {
        childrens = outerparent->findChildren<QObject>(QRegularExpression("^(?!.*/).*$"));
        menu = new QMenu(childrens.at(0).objectName());
    }
    else
        childrens = outerparent->findChildren<QObject>(QRegularExpression(child + "/"));

    for(int i = 0; i < childrens.size(); i++)
    {
        if(!isIn(alredyIn, childrens.at(i).objectName()))
        {
            QList<QString> childIr = childrens.at(i).objectName().split("/");
            if(childIr.at(childIr.size() - 1) != "")
            {
                alredyIn.append(childrens.at(i).objectName());
                QMenu *menu2 = new QMenu(childIr.at(childIr.size() - 1));
                QString Menu = childrens.at(i).objectName();
                if(menu->title() != childrens.at(i).objectName())
                    menu->addMenu(AddAppMenu(childrens.at(i).objectName(), menu2));
                else
                    menu = AddAppMenu(childrens.at(i).objectName(), menu2);
            }
            else
            {
                alredyIn.append(childrens.at(i).objectName());
                createaction(childIr.at(childIr.size() - 2));
                //QAction *action = new QAction(childIr.at(childIr.size() - 2), this);
                menu->addAction(outeraction);
            }
        }
    }
    return menu;
}

void MainWindow::StartCreateMenu()
{
    QSqlQuery *menubarq = new QSqlQuery(db);
    menubarq->exec("SELECT ID, title FROM MenuItems "
             "WHERE (parent IS NULL or parent = '') AND Role IS NULL");

    QSqlRecord rec = menubarq->record();
    const int titleIndex = rec.indexOf("title");
    const int IDIndex = rec.indexOf("ID");
    while(menubarq->next())
    {
        menu = menuBar()->addMenu(menubarq->value(titleIndex).toString());
        createMenu(menubarq->value(IDIndex).toInt(),menu);
    }
}

void MainWindow::StartCreateMenu(QList<QString> rolelist)
{
    this->rolelist = rolelist;
    QSqlQuery *menubarq = new QSqlQuery(db);
    menubarq->exec("SELECT ID, title FROM MenuItems "
                   "WHERE (parent IS NULL or parent = '') AND Role IS NULL");
    ui->menubar->clear();
    while(menubarq->next())
    {
        menu = menuBar()->addMenu(menubarq->value(menubarq->record().indexOf("title")).toString());
        createMenu(menubarq->value(menubarq->record().indexOf("ID")).toInt(),menu, rolelist);
    }

    QSqlQuery *additationMenuq = new QSqlQuery(db);
    QString role;
    QString comma;
    for(auto element :rolelist)
    {
        role += comma + element;
        comma = ",";
    }
    additationMenuq->exec("Select DISTINCT MenuItems.ID, MenuItems.Title, MenuItems.parent, MenuItems.AppName "
              "FROM MenuItems "
              "LEFT JOIN RoleItems "
                "ON MenuItems.ID = RoleItems.ItemID "
              "WHERE RoleItems.RoleID IN ("+role+") AND parent IS NULL;");
    while(additationMenuq->next())
    {
        menu = menuBar()->addMenu(additationMenuq->value(additationMenuq->record().indexOf("title")).toString());
        createMenu(additationMenuq->value(additationMenuq->record().indexOf("ID")).toInt(), menu, rolelist);
    }

}

void MainWindow::inStatusBar(QString info)
{
    ui->statusbar->setVisible(true);
    ui->statusbar->showMessage(info);
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
    ui->menubar->clear();
    SetCornerMenu();
    StartCreateMenu(rolelist);
}

