#include "mainwindow.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QApplication>
#include <QDebug>

QSqlDatabase db ;

int main(int argc, char *argv[])
{
    //连接MySql数据库
    db = QSqlDatabase::addDatabase("QODBC");
        db.setHostName("127.0.0.1");
        db.setPort(3306);
        db.setDatabaseName("library");
        db.setUserName("root");
        db.setPassword("");
        bool ok = db.open();
        if (ok){
            qDebug()<<"MySql connect!!" ;
        }
        else {
            qDebug()<<"wrong" ;
        }
    //主页面
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

