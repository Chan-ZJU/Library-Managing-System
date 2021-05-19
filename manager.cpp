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

QString condition ;
QString CARDID ;
extern QSqlDatabase db ;

void manager::on_BookQuery_clicked()//点击查询图书
{
    QString order1 = ui->QueryOrder1->text(); //order1是查询条件，用在where中
    QString order2 = ui->QueryOrder2->text(); //order2是区间用到，也是在where中
    //QString condition = ui->QueryCondition->text(); //condition是排序顺序

    /*QString BOOKID = "BOOKID" ;
    QString CATEGORY = "CATEGORY" ;
    QString NAME = "NAME" ;
    QString PRESS = "PRESS" ;
    QString YEAR = "YEAR" ;
    QString AUTHOR = "AUTHOR" ;
    QString PRICE = "PRICE" ;
    QString COLLECTION = "COLLECTION" ;
    QString STOCK = "STOCK" ;*/

    if(order2 == nullptr)//没有区间，只用到condition条件进行排序
    {
        if(condition == nullptr)//默认按书号排序
        {
            condition = "BOOKID" ;
        }
        if(order1 == nullptr)//没有查询条件
        {
        QSqlQueryModel *model = new QSqlQueryModel;
        QString sqlstr = "select BOOKID,category,name,press,year,author,price,collection,stock from book order by ";
        sqlstr += condition ;
        sqlstr += " ;";
        model->setQuery(sqlstr);
        qDebug()<<sqlstr ;
        model->setHeaderData(0, Qt::Horizontal, tr("id"));
        model->setHeaderData(1, Qt::Horizontal, tr("category"));
        model->setHeaderData(2, Qt::Horizontal, tr("name"));
        model->setHeaderData(3, Qt::Horizontal, tr("press"));
        model->setHeaderData(4, Qt::Horizontal, tr("year"));
        model->setHeaderData(5, Qt::Horizontal, tr("author"));
        model->setHeaderData(6, Qt::Horizontal, tr("price"));
        model->setHeaderData(7, Qt::Horizontal, tr("collection"));
        model->setHeaderData(8, Qt::Horizontal, tr("stock"));

        ui->tableView->setModel(model);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置成不可编辑
        }
        else //有查询条件
        {
            QSqlQueryModel *model = new QSqlQueryModel;
            QString sqlstr = "select * from book" ;
            sqlstr += " where " ;
            sqlstr += order1 ;
            sqlstr += " order by " ;
            sqlstr += condition ;
            sqlstr += " ;";
            model->setQuery(sqlstr);
            qDebug()<<sqlstr ;
            model->setHeaderData(0, Qt::Horizontal, tr("id"));
            model->setHeaderData(1, Qt::Horizontal, tr("category"));
            model->setHeaderData(2, Qt::Horizontal, tr("name"));
            model->setHeaderData(3, Qt::Horizontal, tr("press"));
            model->setHeaderData(4, Qt::Horizontal, tr("year"));
            model->setHeaderData(5, Qt::Horizontal, tr("author"));
            model->setHeaderData(6, Qt::Horizontal, tr("price"));
            model->setHeaderData(7, Qt::Horizontal, tr("collection"));
            model->setHeaderData(8, Qt::Horizontal, tr("stock"));

            ui->tableView->setModel(model);
            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        }
    }
    else //order2有，即为区间,必有order1
    {
        if(condition == nullptr)//默认按书号排序
        {
            condition = "BOOKID" ;
        }
        //有查询条件,order1,order2都有
        QSqlQueryModel *model = new QSqlQueryModel;
        QString sqlstr = "select * from book" ;
        sqlstr += " where " ;
        sqlstr += order1 ;
        sqlstr += " and " ;
        sqlstr += order2 ;
        sqlstr += " order by " ;
        sqlstr += condition ;
        sqlstr += " ;";
        model->setQuery(sqlstr);
        qDebug()<<sqlstr ;
        model->setHeaderData(0, Qt::Horizontal, tr("id"));
        model->setHeaderData(1, Qt::Horizontal, tr("category"));
        model->setHeaderData(2, Qt::Horizontal, tr("name"));
        model->setHeaderData(3, Qt::Horizontal, tr("press"));
        model->setHeaderData(4, Qt::Horizontal, tr("year"));
        model->setHeaderData(5, Qt::Horizontal, tr("author"));
        model->setHeaderData(6, Qt::Horizontal, tr("price"));
        model->setHeaderData(7, Qt::Horizontal, tr("collection"));
        model->setHeaderData(8, Qt::Horizontal, tr("stock"));

        ui->tableView->setModel(model);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

/*
 * Sample:
 * ( book_no_1, Computer Science, Computer Architecture, xxx, 2004, xxx, 90.00, 2 )
*/
void manager::on_addOneBook_clicked() //点击单本入库
{
    QString BOOKID = "\'" + ui->LINEBOOKID->text() + "\'" ;
    QString CATEGORY = "\'" +ui->LINECATEGORY->text() +"\'";
    QString NAME = "\'" + ui->LINENAME->text()+"\'";
    QString PRESS = "\'" + ui->LINEPRESS->text() + "\'" ;
    QString YEAR = ui->LINEYEAR->text();
    QString AUTHOR = "\'" + ui->LINEAUTHOR->text() + "\'";
    QString PRICE =ui->LINEPRICE->text();
    QString NUMBER=ui->LINENUMBER->text() ;

    QSqlQuery query(db) ;
    QString sqlstr = "select stock,collection from book where bookid = " + BOOKID + ";" ;
    if(query.exec(sqlstr))
    {
        if(query.first()) //有这本书了，stock = stock + number, collection = collection + number
        {
            qDebug()<<"book exists" ;
            int BookStock = query.value(0).toInt();
            BookStock += NUMBER.toInt() ;
            int BookCollection = query.value(1).toInt() ;
            BookCollection += NUMBER.toInt() ;
            NUMBER = QString::number(BookStock) ;
            QString NewCollection = QString::number(BookCollection) ;
            sqlstr = "update book set stock = " + NUMBER + ",collection = " + NewCollection + " where bookid = " + BOOKID + ";" ;
            if(query.exec(sqlstr)) //更新stock，collection,入库成功
            {
                qDebug()<<sqlstr ;
                QMessageBox::information(this,tr("Info"),tr("Add Book Finish!"));
            }
            else
            {
                QMessageBox::warning(this,tr("Warn"),tr("Add Book Fail!")) ;
            }
        }
        else //没有这本书，新增加这本书,collection = stock = number
        {
            qDebug()<<"No such book";
            sqlstr = "Insert into book (BOOKID,category,name,press,year,author,price,collection,stock) values ( " + BOOKID +"," + CATEGORY +"," + NAME +"," + PRESS + "," +YEAR +"," + AUTHOR + "," +PRICE + "," +NUMBER + "," +NUMBER + " );" ;
            qDebug()<<sqlstr ;
            if(query.exec(sqlstr))
            {
                qDebug()<<sqlstr ;
                QMessageBox::information(this,tr("Info"),tr("Add Book Finish!")) ;
            }
            else
            {
                QMessageBox::warning(this,tr("Warn"),tr("Information not correct!")) ;
            }
        }
    }
}


void manager::on_comboBox_currentIndexChanged(const QString &arg1) //点击combobox选择排序condition
{
    condition = ui->comboBox->currentText() ;
    qDebug()<<condition ;
}


void manager::on_AddBookFromFile_clicked() //点击批量入库按钮，从文件读入图书入库
{
    QFile bookFile("bookFile.txt") ;
    if(!bookFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("Warn"),tr("Can't open File!")) ;
    }
    else //成功打开文件，开始入库流程
    {
        QString BOOKID ;
        QString CATEGORY ;
        QString NAME ;
        QString PRESS ;
        QString YEAR ;
        QString AUTHOR ;
        QString PRICE ;
        QString NUMBER ;

        QSqlQuery query(db) ;
        QString sqlstr ;
        while(!bookFile.atEnd())
        {
            QByteArray Line = bookFile.readLine() ;
            QString tmp = (Line) ;
            BOOKID ="\'" + tmp.section(',',0,0).trimmed().remove(0,1) + "\'";//trimmed()去掉\n之类的
            CATEGORY ="\'" + tmp.section(',',1,1).trimmed() + "\'";
            NAME ="\'"+ tmp.section(',',2,2).trimmed()+"\'" ;
            PRESS ="\'"+ tmp.section(',',3,3).trimmed()+"\'" ;
            YEAR = tmp.section(',',4,4).trimmed() ;
            AUTHOR ="\'"+ tmp.section(',',5,5).trimmed()+"\'" ;
            PRICE = tmp.section(',',6,6).trimmed() ;
            int lastIndex = tmp.section(',',7,7).indexOf(')') ;
            NUMBER = tmp.section(',',7,7).trimmed().remove(lastIndex,1) ;
            qDebug()<<NUMBER ;
            sqlstr = "select stock,collection from book where bookid = " + BOOKID + ";" ;
            if(query.exec(sqlstr))
            {
                if(query.first()) //有这本书了，stock = stock + number, collection = collection + number
                {
                    qDebug()<<"book exists" ;
                    int BookStock = query.value(0).toInt();
                    BookStock += NUMBER.toInt() ;
                    int BookCollection = query.value(1).toInt() ;
                    BookCollection += NUMBER.toInt() ;
                    NUMBER = QString::number(BookStock) ;
                    QString NewCollection = QString::number(BookCollection) ;
                    sqlstr = "update book set stock = " + NUMBER + ",collection = " + NewCollection + " where bookid = " + BOOKID + ";" ;
                    if(query.exec(sqlstr)) //更新stock，collection,入库成功
                    {
                        qDebug()<<sqlstr ;
                        //QMessageBox::information(this,tr("Info"),tr("Add Book Finish!"));
                    }
                    else
                    {
                        //QMessageBox::warning(this,tr("Warn"),tr("Add Book Fail!")) ;
                    }
                }
                else //没有这本书，新增加这本书,collection = stock = number
                {
                    qDebug()<<"No such book";
                    sqlstr = "Insert into book (BOOKID,category,name,press,year,author,price,collection,stock) values ( " + BOOKID +"," + CATEGORY +"," + NAME +"," + PRESS + "," +YEAR +"," + AUTHOR + "," +PRICE + "," +NUMBER + "," +NUMBER + " );" ;
                    qDebug()<<sqlstr ;
                    if(query.exec(sqlstr))
                    {
                        qDebug()<<sqlstr ;
                        //QMessageBox::information(this,tr("Info"),tr("Add Book Finish!")) ;
                    }
                    else
                    {
                        //QMessageBox::warning(this,tr("Warn"),tr("Information not correct!")) ;
                    }
                }
            }
        }
        //文件读取结束
        QMessageBox::information(this,tr("Info"),tr("Add books from File finish!")) ;
    }
}


void manager::on_BorrowBook_clicked() //点击借书按钮
{
    QString BOOKID = ui->LineBookID->text() ;
    //先查询有没有这本书，有的话啥事没有
    QSqlQuery query(db) ;
    QString sqlstr ;
    sqlstr = "select * from book where bookid = '" ;
    sqlstr += BOOKID ;
    sqlstr += "';" ;
    bool foundBook = true ;
    if(query.exec(sqlstr))
    {
        qDebug()<<sqlstr ;
        if(!query.first())
        {
            foundBook = false ;
            qDebug()<<"No Such Book";
            QMessageBox::warning(this,tr("Borrow Fail!"),tr("No such Book!")) ;
        }
        else
        {
            foundBook = true ;
        }
    }
    //有这本书:
    //1. 如果该书还有库存，则借书成功，同时库存数减一。
    //2. 否则输出该书无库存，且输出最近归还的时间。
    if(foundBook)
    {
        sqlstr = "select stock from book where bookid = '"+BOOKID+"';" ;
        if(query.exec(sqlstr))
        {
            if(query.first())
            {
                qDebug()<<sqlstr ;
                int Stock = query.value(0).toInt() ; //这本书的库存
                if(Stock >= 1)//还有库存，还能借书
                {
                    //借书操作：
                    /*
                        1.book表库存 -- ;
                        2.borrow表加数据 ;
                    */
                    Stock -- ;
                    //book表stock --;
                    QString newStock = QString::number(Stock) ;
                    if(query.exec("update book set stock = "+newStock+" where bookid = '" + BOOKID + "';"))
                    {
                        qDebug()<<"Table book stock --" ;
                    }
                    //borrow表加数据
                    sqlstr = "insert into borrow (bookid,cardid,borrowtime,returntime,managerid) values ('"+BOOKID+"','"+CARDID+"',curdate(),'0-0-0','"+ManagerAccount+"' );" ;
                    if(query.exec(sqlstr))
                    {
                        qDebug()<<" Table borrow insert finish!" ;
                    }
                }
                else//无库存，无法借书
                {
                    //输出该书无库存，且输出最近归还的时间
                    QString lastReturnTime ;
                    //根据bookid从borrow表中找出最近归还时间
                    sqlstr = "select " ;
                    if(query.exec())

                    QMessageBox::warning(this,"Warn","The stock of the book is 0!\n Last return-time is "+lastReturnTime) ;
                }
            }
        }
    }
}


void manager::on_QueryCardID_clicked() //点击查询，根据借书卡号来显示已经借书的记录
{
    //需要用到TableView
    QSqlQueryModel *model = new QSqlQueryModel;
    QString sqlstr = "select bookid,name,category,press,year,author,price,collection,stock from borrow natural join book where cardid = ";
    QString tmp = ui->LineCardID->text() ;
    CARDID = tmp ;
    sqlstr += tmp ;

    model->setQuery(sqlstr);
    qDebug()<<sqlstr ;
    model->setHeaderData(0, Qt::Horizontal, tr("id"));
    model->setHeaderData(1, Qt::Horizontal, tr("category"));
    model->setHeaderData(2, Qt::Horizontal, tr("name"));
    model->setHeaderData(3, Qt::Horizontal, tr("press"));
    model->setHeaderData(4, Qt::Horizontal, tr("year"));
    model->setHeaderData(5, Qt::Horizontal, tr("author"));
    model->setHeaderData(6, Qt::Horizontal, tr("price"));
    model->setHeaderData(7, Qt::Horizontal, tr("collection"));
    model->setHeaderData(8, Qt::Horizontal, tr("stock"));

    ui->Borrow->setModel(model);
    ui->Borrow->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置成不可编辑
}

