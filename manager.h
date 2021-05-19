#ifndef MANAGER_H
#define MANAGER_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <string>
#include <QString>
#include <QtDebug>
#include <QStringAlgorithms>
#include <QMessageBox>
#include <QFile>

extern QString ManagerAccount ;

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

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_AddBookFromFile_clicked();

    void on_BorrowBook_clicked();

    void on_QueryCardID_clicked();

private:
    Ui::manager *ui;
};

#endif // MANAGER_H
