#include "manager.h"
#include "ui_manager.h"

manager::manager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::manager)
{
    ui->setupUi(this);
}

manager::~manager()
{
    delete ui;
}


void manager::on_BookQuery_clicked()//点击查询图书
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select BOOKID,category,name,press,year,author,price,collection,stock from book;");

    model->setHeaderData(0, Qt::Horizontal, tr("id"));
    model->setHeaderData(1, Qt::Horizontal, tr("Time"));
    model->setHeaderData(2, Qt::Horizontal, tr("Co"));
    model->setHeaderData(3, Qt::Horizontal, tr("H2s"));
    model->setHeaderData(4, Qt::Horizontal, tr("O2"));
    model->setHeaderData(5, Qt::Horizontal, tr("甲烷"));
    model->setHeaderData(6, Qt::Horizontal, tr("电量"));
    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置成不可编辑
}

