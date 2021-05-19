#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

extern QSqlDatabase db ;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    admin = new manager ;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_in_clicked()
{
    QString account = ui->Account->text() ;
    qDebug()<<account ;
    QString password = ui->Password->text() ;
    //查看管理员表中是否有account
    QSqlQuery query(db) ;
    if(db.open()) qDebug()<<"DB conn" ;
    QString sqlstr ;
    sqlstr = "select password from manager where ID = '"+account+"';" ;
    qDebug()<<sqlstr ;
    //query.prepare(sqlstr) ;
    if(query.exec(sqlstr))
    {
        if(query.first())
        {
            qDebug()<<"Account exists" ;
            QString correct_password = query.value(0).toString() ;
            if(correct_password != password)
            {
                QMessageBox::warning(this,tr("Login Fail!"),tr("Password Wrong!")) ;
            }
            else
            {
                admin->show() ;
            }
        }
        else
        {
            qDebug()<<"No account";
            QMessageBox::warning(this,tr("Login Fail!"),tr("No such Account")) ;
        }
    }
    else
    {
        qDebug()<<"Query not execute!" ;
    }
}

