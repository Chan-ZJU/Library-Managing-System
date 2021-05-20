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
                    QMessageBox::information(this,tr("Info"),tr("Borrow Book Finish!")) ;
                }
                else//无库存，无法借书
                {
                    //输出该书无库存，且输出最近归还的时间
                    QString lastReturnTime ;
                    //根据bookid从borrow表中找出最近归还时间
                    sqlstr = " select returntime from borrow where bookid = '"+BOOKID+"' order by returntime desc ;" ;
                    if(query.exec(sqlstr))
                    {
                        if(query.first())
                        {
                            lastReturnTime = query.value(0).toString();
                            qDebug()<<lastReturnTime ;
                        }
                    }//return 0000-00-00表示还未还书,但实际调试发现返回了空值，而非0000-00-00
                    if(lastReturnTime!="")
                        QMessageBox::warning(this,"Warn","The stock of the book is 0!\n Last return-time is "+lastReturnTime) ;
                    else
                        QMessageBox::warning(this,"Warn","The stock of the book is 0!\n No book has been returned") ;
                }
            }
        }
    }
}


void manager::on_QueryCardID_clicked() //点击查询，根据借书卡号来显示已经借书的记录(已还了就不显示)
{
    //需要用到TableView
    QSqlQueryModel *model = new QSqlQueryModel;
    QString sqlstr = "select bookid,name,category,press,year,author,price,collection,stock from borrow natural join book where cardid = '";
    QSqlQuery query(db) ;
    QString tmp = ui->LineCardID->text() ;
    CARDID = tmp ;
    sqlstr += tmp ;
    sqlstr += "' and returntime = '0000-00-00' ;" ;
    QString sqlstr1 = "select * from card where cardid = '"+CARDID+"' ;" ;
    //如果借书卡号不存在，提示不存在
    if(query.exec(sqlstr1))
    {
        if(!query.first())
        {
            QMessageBox::warning(this,tr("Warn"),tr("No such Card!")) ;
        }
        else
        {
            QMessageBox::information((this),tr("Info"),tr("Query Finish!")) ;
        }
    }

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


void manager::on_ReturnBook_clicked() //点击还书按钮
{
    /*
        如果该书在已借书籍列表内, 则还书成功, 同时库存加一.
        否则输出出错信息.
    */
    QString BOOKID = ui->LineBookID->text() ;
    QString sqlstr ;
    QString NewStock ;
    //还未还的书returntime是0000-00-00
    sqlstr = "select * from borrow where bookid = '"+BOOKID+"' and returntime = '0000-00-00' ;" ;
    QSqlQuery query(db) ;
    if(query.exec(sqlstr))
    {
        if(query.first())
        {
            //借过这本书，并且没有还过，开始还书
            //1. stock ++ ;
            //2. borrow表中更新returntime为当前日期curdate() ;
            sqlstr = "select stock from book where bookid = '"+BOOKID+"';" ;
            if(query.exec(sqlstr))
            {
                if(query.first())
                {
                    NewStock = query.value(0).toString() ;
                    int tmp = NewStock.toInt() ;
                    NewStock = QString::number(tmp+1) ;
                    sqlstr = "update book set stock = "+NewStock+" where bookid = '"+BOOKID+"' ;" ;
                    if(query.exec(sqlstr))//update stock
                    {
                        qDebug()<<"book returned and update stock" ;
                    }
                }
            }
            sqlstr = "update borrow set returntime = curdate() where bookid = '"+BOOKID+"'and returntime='0000-00-00' lIMIT 1 ;" ;
            if(query.exec(sqlstr))
            {
                QMessageBox::information(this,tr("Info"),tr("Return Book finish!")) ;
            }
        }
        else //没有借过这本书，或者已经还了，输出出错信息
        {
            QMessageBox::information(this,tr("Info"),tr("The book has been returned!")) ;
        }
    }
}


void manager::on_AddCard_clicked() //点击增加借书证按钮
{
    //先判断借书证存不存在，已存在就提示以下
    //不存在就增加
    QString CARDID = ui->LineCardID->text() ;
    QString sqlstr ;
    QString NAME, UNIT, TYPE ;
    QSqlQuery query(db) ;
    sqlstr = "select * from card where cardid = '"+CARDID+"' ;" ;
    if(query.exec(sqlstr))
    {
        if(query.first())//已经存在
        {
            QMessageBox::information(this,tr("Info"),tr("Card Exists!")) ;
        }
        else
        {
            //不存在，添加借书证
            NAME = ui->LineName->text() ;
            UNIT = ui->LineUnit->text();
            TYPE = ui->LineType->text() ;
            sqlstr = "insert into card values ('" + CARDID+"'," + " '"+NAME+"', '"+UNIT+"', '"+TYPE+"' ) ;" ;
            if(query.exec(sqlstr))
            {
                qDebug()<<sqlstr ;
                QMessageBox::information(this,tr("Info"),tr("Add Card Finish!")) ;
            }
        }
    }
}


void manager::on_DeleteCard_clicked() //点击删除借书证按钮
{
    //起初建表时设置card表cardid为PK，borrow表cardid为指向card.cardid的FK，且on delete cascade，理论上讲应该只要删card表，borrow记录就没了
    //实际验证确实如此
    //先查找有没有这个cardid
    QString CARDID = ui->LineCardID->text() ;
    QString sqlstr ;
    QSqlQuery query(db) ;
    sqlstr = "select * from card where cardid = '"+CARDID+"' ;" ;
    if(query.exec(sqlstr))
    {
        qDebug()<<sqlstr ;
        if(!query.first())//不存在，提示错误
        {
            QMessageBox::warning(this,tr("Warn"),tr("No such CARD!")) ;
        }
        else//存在这个借书证，要删除
            //只需要删除card表中记录就可，borrow设置外键，级联删除，无需操作
            //要判断borrow中是否还有没有还的书，否则无法注销
        {
            sqlstr = "select * from borrow where cardid = '"+CARDID+"' and returntime = '0000-00-00' ;" ;
            if(query.exec(sqlstr))
            {
                if(!query.first())//没有未还的书，直接删除card
                {
                    sqlstr = "delete from card where cardid = '"+CARDID+"' ;" ;
                    qDebug()<<sqlstr ;
                    if(query.exec(sqlstr))
                    {
                        qDebug()<<sqlstr ;
                        QMessageBox::information(this,tr("Info"),tr("Delete Card Finish!")) ;
                    }
                }
                else
                {
                    QMessageBox::warning(this,tr("Warn"),tr("Delete Fail! Book hasn't been returned!")) ;
                }
            }
        }
    }
}

