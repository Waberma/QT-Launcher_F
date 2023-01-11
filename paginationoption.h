#ifndef PAGINATIONOPTION_H
#define PAGINATIONOPTION_H

#include <QDialog>

namespace Ui {
class PaginationOption;
}

class PaginationOption : public QDialog
{
    Q_OBJECT

public:
    explicit PaginationOption(QWidget *parent = nullptr);
    ~PaginationOption();
signals:
    void SetNewPar(int step, int totalpage);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

public slots:
    void setStep(int step);

    void setMapInv(int mapinv);
private:
    Ui::PaginationOption *ui;
    int step;
    int mapinv;
};

#endif // PAGINATIONOPTION_H
