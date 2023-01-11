# QT-Launcher

## QSqlDatabase

### Connect Database
```Cplusplus
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
```

## Connect objects

### Connect QWidget
```Cplusplus 
MainPage *mainpage;
mainpage = new MainPage;
connect(mainpage,  &MainPage::setUser, rolepage, &RolePage::SetUser);
connect(this, &MainWindow::MainPagesignal, mainpage, &MainPage::menuActions);
```

### Connect butt
```Cplusplus 
connect(pbPlusMin,SIGNAL(clicked()), this, SLOT(PlusMin()));
```

### Connect with QList
```Cplusplus 
connect(mainpage, &MainPage::StartCreateMenu, this,  static_cast<void (MainWindow::*)(QList<QString> rolelist)> (&MainWindow::StartCreateMenu));
```

### Connect in diff Thread
```Cplusplus
connect(this, &Pagination::GetNewNextEl, nextpagethr, &NewElementsthr::GetNewNextEl, Qt::DirectConnection);
```

## QTabWidget AddTab and setCurrent
```Cplusplus 
ui->tabWidget->addTab(personpage,"Пользователи");
ui->tabWidget->setCurrentWidget(personpage);
``` 

## QThread
```Cplusplus 
secondthread = new QThread(this);
nextpagethr->moveToThread(secondthread);
```
## CornerWidget
```Cplusplus 
ui->menubar->setCornerWidget(cornerMenu);
```

## QSqlQuery
### QSqlRecord
```Cplusplus
cornerMenuq->value(cornerMenuq->record().indexOf("Name")).toString();
```
#### Name - ColumnName
