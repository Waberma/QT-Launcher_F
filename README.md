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

## Connect in diff Thread
```Cplusplus
connect(this, &Pagination::GetNewNextEl, nextpagethr, &NewElementsthr::GetNewNextEl, Qt::DirectConnection);
```

### QTabWidget AddTab and setCurrent
```Cplusplus 
ui->tabWidget->addTab(personpage,"Пользователи");
ui->tabWidget->setCurrentWidget(personpage);
``` 

### QThread
