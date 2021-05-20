#ifndef VISITOR_H
#define VISITOR_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <string>
#include <QString>
#include <QtDebug>
#include <QStringAlgorithms>
#include <QMessageBox>
#include <QFile>

namespace Ui {
class visitor;
}

class visitor : public QMainWindow
{
    Q_OBJECT

public:
    explicit visitor(QWidget *parent = nullptr);
    QString Condition ;
    ~visitor();

private slots:
    void on_BookQuery_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::visitor *ui;
};

#endif // VISITOR_H
