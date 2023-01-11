#include "pagination.h"
#include "ui_pagination.h"
#include <QStandardItemModel>
#include <QSqlRecord>
#include <thread>
#include "pagination.h"

Pagination::Pagination(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pagination)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./menu.db");
    if(db.open())
    {
        qDebug("Open");
    }
    else
    {
        qDebug("NoOpen");
    }


    secondthread = new QThread(this);

    nextpagethr = new NewElementsthr();
    pagopt = new PaginationOption(this);
    connect(this, SIGNAL(destroyed()), secondthread, SLOT(quit()));

    //Связи с потоком
    connect(this, &Pagination::GetNewNextEl, nextpagethr, &NewElementsthr::GetNewNextEl, Qt::DirectConnection);
    connect(nextpagethr, SIGNAL(SetMap(QMap<int,QStandardItemModel *>)), this, SLOT(SetMap(QMap<int,QStandardItemModel *>)));

    connect(this, &Pagination::setMapInv, pagopt, &PaginationOption::setMapInv);
    connect(this, &Pagination::setStep, pagopt, &PaginationOption::setStep);

    //Связи для настройки параметров
    connect(pagopt, &PaginationOption::SetNewPar, this, &Pagination::setPar);


    nextpagethr->moveToThread(secondthread);

    queryview = new QSqlTableModel(this, db);
    queryview->setQuery("SELECT * FROM Pagination;");
    ui->tableView->setModel(queryview);

    while(queryview->canFetchMore())
        queryview->fetchMore();
    step = ui->tableView->rowHeight(0);
    total = ui->tableView->model()->rowCount();


    for(totalpage = 1; totalpage*step < total+step; totalpage++)
    {
        ui->comboBox->addItem(QString::number(totalpage));
    }
    mapinv = totalpage/5;
    //secondthread->start();
}

Pagination::~Pagination()
{
    delete ui;
}

void Pagination::SetMap(QMap<int, QStandardItemModel *> newmap)
{
    pages = newmap;
}

void Pagination::setPar(int step, int mapinv)
{
    this->step = step;
    this->mapinv = mapinv;
    updateForm();
}

void Pagination::openOption()
{
    pagopt->show();
    emit setMapInv(mapinv);
    emit setStep(step);
}

void Pagination::on_comboBox_currentIndexChanged(int index)
{
    if(pages.count(index))
    {
        ui->tableView_2->setModel(pages.value(index));
        emit GetNewNextEl(previousindex, step, mapinv, queryview);
    }
    else
    {
        QStandardItem* item = 0;
        QStandardItemModel* model = new QStandardItemModel();
        QStringList labels = QObject::trUtf8("ID, Hoster, Lot, Name").simplified().split(",");
        model->setHorizontalHeaderLabels(labels);
        int l = 0;
        for(int j = index*step; step*index+step > j; ++j)
        {
            for(int k = 0; k < ui->tableView->model()->columnCount(); k++)
            {
                QModelIndex ind = ui->tableView->model()->index(j,k,QModelIndex());
                QVariant b = ui->tableView->model()->data(ind,Qt::DisplayRole);
                QString a = b.value<QString>();
                item = new QStandardItem(b.value<QString>());
                model->setItem(l,k,item);
            }
            l++;
        }
        ui->tableView_2->setModel(model);
        previousindex = index;
        emit GetNewNextEl(previousindex, step, mapinv, queryview);
    }
}

void NewElementsthr::GetNewNextEl(int startindex, int step, int numOfPages, QSqlQueryModel *mainmodel)
{

    QStandardItem* item = 0;
    QMap<int, QStandardItemModel *> map;
    int l;
    for(int i = startindex; i <= startindex+numOfPages; ++i)
    {
        QStandardItemModel* model = new QStandardItemModel();
        QStringList labels = QObject::trUtf8("ID, Hoster, Lot, Name").simplified().split(",");
        model->setHorizontalHeaderLabels(labels);
        l = 0;
        for(int j = i*step; i*step+step > j; ++j)
        {
            for(int k = 0; k < mainmodel->columnCount(); k++)
            {
                QModelIndex ind = mainmodel->index(j,k,QModelIndex());
                QVariant b = mainmodel->data(ind,Qt::DisplayRole);
                item = new QStandardItem(b.value<QString>());
                model->setItem(l,k,item);
            }
            l++;
        }
        map.insert(i,model);
    }


    for(int i = startindex; i >= startindex - numOfPages*2  && i > 0; --i)
    {
        QStandardItemModel* model = new QStandardItemModel();
        QStringList labels = QObject::trUtf8("ID, Hoster, Lot, Name").simplified().split(",");
        model->setHorizontalHeaderLabels(labels);
        l = 0;
        for(int j = i*step; i*step+step > j; ++j)
        {
            for(int k = 0; k < mainmodel->columnCount(); k++)
            {
                QModelIndex ind = mainmodel->index(j,k,QModelIndex());
                QVariant b = mainmodel->data(ind,Qt::DisplayRole);
                item = new QStandardItem(b.value<QString>());
                model->setItem(l,k,item);
            }
            l++;
        }
        map.insert(i,model);
    }
    emit SetMap(map);;

}

void Pagination::on_pushButton_clicked()
{
    if(ui->comboBox->currentIndex() < totalpage-2)
        ui->comboBox->setCurrentIndex(ui->comboBox->currentIndex()+1);
}

void Pagination::on_pushButton_2_clicked()
{
    if(ui->comboBox->currentIndex() > 0)
        ui->comboBox->setCurrentIndex(ui->comboBox->currentIndex()-1);
}

void Pagination::updateForm()
{
    QStandardItem* item = 0;
    QStandardItemModel* model = new QStandardItemModel();
    QStringList labels = QObject::trUtf8("ID, Hoster, Lot, Name").simplified().split(",");
    model->setHorizontalHeaderLabels(labels);
    int l = 0;
    for(int j = 0; step > j; ++j)
    {
        for(int k = 0; k < ui->tableView->model()->columnCount(); k++)
        {
            QModelIndex ind = ui->tableView->model()->index(j,k,QModelIndex());
            QVariant b = ui->tableView->model()->data(ind,Qt::DisplayRole);
            QString a = b.value<QString>();
            item = new QStandardItem(b.value<QString>());
            model->setItem(l,k,item);
        }
        l++;
    }
    ui->tableView_2->setModel(model);
    ui->comboBox->clear();
    for(totalpage = 1; totalpage*step < total+step; totalpage++)
    {
        ui->comboBox->addItem(QString::number(totalpage));
    }
}
