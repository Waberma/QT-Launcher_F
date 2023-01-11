#include "paginationoption.h"
#include "ui_paginationoption.h"

PaginationOption::PaginationOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaginationOption)
{
    ui->setupUi(this);
}

PaginationOption::~PaginationOption()
{
    delete ui;
}

void PaginationOption::on_pushButton_clicked()
{
    this->close();
}

void PaginationOption::on_pushButton_2_clicked()
{
    mapinv = ui->mapunvtb->text().toInt();
    step = ui->steptb->text().toInt();
    emit SetNewPar(step,mapinv);
    this->close();
}

void PaginationOption::setStep(int step)
{
    this->step = step;
    ui->steptb->setText(QString::number(step));
}

void PaginationOption::setMapInv(int mapinv)
{
    this->mapinv = mapinv;
    ui->mapunvtb->setText(QString::number(mapinv));
}
