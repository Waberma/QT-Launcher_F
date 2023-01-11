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
