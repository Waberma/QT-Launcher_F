#ifndef PAGINATION_H
#define PAGINATION_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QListWidgetItem>
#include <QThread>
#include <thread>
#include <QStandardItemModel>
#include "paginationoption.h"


class NewElementsthr : public QObject
{
    Q_OBJECT
public:
    explicit NewElementsthr(QObject *parent = 0) {};
    ~NewElementsthr() {};
signals:
    void SetMap(QMap<int,QStandardItemModel *>);
    void finished();
public slots:
    void GetNewNextEl(int startindex, int step, int numOfPages, QSqlQueryModel *mainmodel);


private:
    QMap<int,QStandardItemModel *> pages;
    int step;
    int startindex;
    int numOfPages;
    QSqlQueryModel *mainmodel;
};

namespace Ui {
class Pagination;
}

class Pagination : public QWidget
{
    Q_OBJECT

public:
    explicit Pagination(QWidget *parent = nullptr);
    ~Pagination();
    //void GetNewNextEl(int startindex, int endindex);
    //void GetNewPreviousEl(int startindex, int endindex);

public slots:
    void SetMap(QMap<int,QStandardItemModel *>);

    void setPar(int step, int mapinv);

    void openOption();
private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


signals:
    void GetNewNextEl(int startindex, int step, int numOfPages, QSqlQueryModel *mainmodel);

    void setStep(int step);

    void setMapInv(int mapinv);
private:
    Ui::Pagination *ui;
    QSqlDatabase db;
    QSqlQueryModel *queryview;
    int total;
    int step;
    int mapinv;
    int totalpage = 1;
    int previousindex = NULL;
    NewElementsthr *nextpagethr;
    PaginationOption *pagopt;
    void updateForm();
    QMap<int,QStandardItemModel *> pages;
    void update();
    bool isReady;
    QThread *secondthread;
    QThread *mainthread;
};

#endif // PAGINATION_H
