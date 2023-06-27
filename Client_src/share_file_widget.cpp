#include "share_file_widget.h"
#include "ui_share_file_widget.h"
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QMimeType"
#include "item_of_share.h"
#include "QMimeDatabase"
#include "QMessageBox"
#include "QNetworkAccessManager"
#include "QNetworkReply"
#include "QNetworkRequest"
#include "User_Name.h"
#include "QUrlQuery"
#include "QFileDialog"
#include "QFile"
#include "QJsonDocument"
#include "QJsonObject"

Share_File_Widget::Share_File_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Share_File_Widget)
{
    ui->setupUi(this);
    ui->label_file_name_pro->setText("null");
    ui->progressBar->setValue(0);
    connect(this,&Share_File_Widget::Flush_Page,this,&Share_File_Widget::Flush_Shared_Page);
    connect(ui->btn_download,&QPushButton::clicked,this,&Share_File_Widget::DownLoad_Shared_File);
}

Share_File_Widget::~Share_File_Widget()
{
    delete ui;
}

void Share_File_Widget::Flush_Shared_Page()
{
    ui->file_list->clear();
    QSqlDatabase db;

    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
    }

    QString file_name;
    QString create_time;
    QString file_size;
    QString File_Md5;
    QString Shared_User;

    QMimeDatabase Type_db;
    //
        db.setHostName("192.168.118.134");
        db.setPort(3306);
        db.setDatabaseName("WebServer");
        db.setUserName("root");
        db.setPassword("chh20020729");

        bool ok = db.open();
        if (ok)
        {
            QString select = QString("SELECT * FROM share_file_list");
            QSqlQuery query;
            query.exec(select);


            while(query.next())
            {
                item_of_share * item = new item_of_share();
                Shared_User = query.value(0).toString();
                file_name = query.value(3).toString();
                create_time = query.value(2).toString();
                File_Md5 = query.value(1).toString();

                QString Select_File_Size = QString("SELECT size FROM file_info WHERE md5='%1'").arg(File_Md5);
                QSqlQuery size_query;
                size_query.exec(Select_File_Size);
                while(size_query.next())
                {
                    file_size = size_query.value(0).toString();
                }

                QMimeType file_type = Type_db.mimeTypeForFile(file_name);

                item->Set_Create_Time(create_time);
                item->Set_File_Name(file_name);
                item->Set_File_Type(file_type.name());
                item->Set_File_Size(file_size);
                item->Set_File_Shared_User(Shared_User);

                QListWidgetItem *pItem = new QListWidgetItem();
                pItem->setSizeHint(item->size());

                ui->file_list->addItem(pItem);
                ui->file_list->setItemWidget(pItem, item);
            }

        }
        if(db.isOpen())
        {
             db.close();
        }
}

void Share_File_Widget::DownLoad_Shared_File()
{
    QList<QListWidgetItem*> Selet_Ttem =  ui->file_list->selectedItems();

    if(Selet_Ttem.isEmpty())
    {
        QMessageBox::information(this,"提示","请选中需要下载的文件");
        return;
    }
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setWindowTitle("选择下载路径");

    QList<QString> selectDir;

    if(fileDialog->exec() == QFileDialog::Accepted)
    {
        selectDir = fileDialog->selectedFiles();
    }
    else
    {
       return;
    }

    QWidget *My_Widget = ui->file_list->itemWidget(Selet_Ttem[0]);
    QList<QLabel*> List_Item_Label = My_Widget->findChildren<QLabel*>();
    ui->label_file_name_pro->setText(List_Item_Label[0]->text());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    QUrl url(Server_URL::DOWNLOAD_URL);
    QUrlQuery query;

    query.addQueryItem("user_name",List_Item_Label[4]->text().toUtf8());
    query.addQueryItem("download_file_name",List_Item_Label[0]->text().toUtf8());
    query.addQueryItem("shared_statue","1");

    url.setQuery(query.query());

    request.setUrl(url);

    QNetworkReply *reply = manager->get(request);

    connect(reply,&QNetworkReply::finished,this,[=]{

        auto get_server_message = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(get_server_message);
        QJsonObject obj = doc.object();
        auto Get_Reply = obj.value("Down_Load_Statue").toString();
        if(reply->readBufferSize()<100)
        {
            if(Get_Reply=="Failed")
            {
                ui->progressBar->setMaximum(100);
                ui->progressBar->setValue(0);
                QMessageBox::warning(this,"失败","获取服务器资源出错");
                return;
            }
        }

        QFile file(selectDir.at(0)+"/"+List_Item_Label[0]->text());
        file.open(QIODevice::WriteOnly);
        file.write(get_server_message);
        file.close();

        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(100);

        QMessageBox::information(this,"成功","下载完成");
    });

    connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesRead, qint64 totalBytes)//获取传输进度并改变进度条
    {
        if(totalBytes != 0)
        {
            ui->progressBar->setMaximum(totalBytes);
            ui->progressBar->setValue(bytesRead-qint64(1));
        }
    });
}

