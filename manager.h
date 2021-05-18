#ifndef MANAGER_H
#define MANAGER_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <string>
#include <QtDebug>

namespace Ui {
class manager;
}

class manager : public QMainWindow
{
    Q_OBJECT

public:
    explicit manager(QWidget *parent = nullptr);
    ~manager();

private slots:
    void on_BookQuery_clicked();

    void on_addOneBook_clicked();

private:
    Ui::manager *ui;
};

#endif // MANAGER_H
