#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QMessageBox>
#include "manager.h"
#include "visitor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_Login_in_clicked();

    void on_VisitorLogin_clicked();

private:
    Ui::MainWindow *ui;
    manager *admin ;
    visitor *Visitor ;
};
#endif // MAINWINDOW_H
