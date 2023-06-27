#include "myfile_page.h"
#include "ui_myfile_page.h"
#include "QFileInfo"
#include "QFileDialog"
#include "QMessageBox"
#include "QHttpMultiPart"
#include "QNetworkAccessManager"
#include "QNetworkReply"
#include "QDataStream"
#include "cstdio"
#include "User_Name.h"
#include "QJsonObject"
#include "QJsonDocument"
#include "QMimeType"
#include "QMimeDatabase"
#include "QSqlDatabase"
#include "list_item.h"
#include "QSqlQuery"
#include "QListWidgetItem"
#include "QUrlQuery"
#include "QMenu"
MyFile_Page::MyFile_Page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyFile_Page)
{
    ui->setupUi(this);
    ui->label_file_name_pro->setText("null");
    ui->progressBar->setValue(0);
    ui->file_list->setAlternatingRowColors(true);
    connect(this,&MyFile_Page::Flush_List_Item,this,&MyFile_Page::Get_User_All_Flie);
    connect(ui->btn_upload,&QPushButton::clicked,this,&MyFile_Page::SendFile);
    connect(this,&MyFile_Page::Md5_Save_Failed,this,[=](QString File_Path,QString File_Md5){
        //qDebug()<<"Md5失败";
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart file_Part;
        QFile *file = new QFile(File_Path);
        file->open(QIODevice::ReadOnly);

        QNetworkAccessManager *Upload_manager = new QNetworkAccessManager(this);
        QFileInfo file_info(File_Path);
        QByteArray bytes = file->readAll();//读取文件内容

        QMimeDatabase db;
        QMimeType file_type = db.mimeTypeForFile(file_info.fileName());

        //QString User_Name;
        //设置传输方式以及需要携带的参数

        Get_User_Name *User_Name = Get_User_Name::Instance();

        QString arg = QString("form-data;user_name=\"%1\";filename=\"%2\";file_md5=\"%3\";file_type=\"%4\";file_size=\"%5\";")\
                .arg(User_Name->Name.toUtf8())\
                .arg(file_info.fileName().toUtf8())\
                .arg(File_Md5.toUtf8())\
                .arg(file_type.name().toUtf8())\
                .arg(file_info.size());

        file_Part.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant(arg));
        file_Part.setBody(bytes);//设置传输数据
        file->setParent(multiPart);//文件指针在multiPart析构时关闭文件同时析构

        multiPart->append(file_Part);

        QNetworkRequest request;
        request.setUrl(QUrl(Server_URL::UPLOAD_FILE_URL));//设置请求URL
        request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");

        QNetworkReply *reply = Upload_manager->post(request, multiPart);
        multiPart->setParent(reply);
        connect(reply, &QNetworkReply::uploadProgress, [=](qint64 bytesRead, qint64 totalBytes)//获取传输进度并改变进度条
        {
            if(totalBytes != 0)
            {
                ui->progressBar->setMaximum(totalBytes);
                ui->progressBar->setValue(bytesRead-qint64(1));
            }
        });
        connect(reply,&QNetworkReply::finished,[=]{//当服务器确定文件收到并在服务器创建资源时完成进度条并弹出提示框
            auto get_server_message = reply->readAll();
            //qDebug()<<"上传成功"<<get_server_message.toLatin1();
            QJsonDocument doc = QJsonDocument::fromJson(get_server_message);
            QJsonObject obj = doc.object();
            auto Get_Reply = obj.value("UpLoad Statue").toString();
            if(Get_Reply == "Success")
            {
                ui->progressBar->setMaximum(100);
                ui->progressBar->setValue(100);

                emit Flush_List_Item();
                QMessageBox::information(this,"成功","上传成功");
            }
            else
            {
                ui->progressBar->setMaximum(100);
                ui->progressBar->setValue(0);
                QMessageBox::warning(this,"失败","上传失败");
            }
        });

    });

    connect(ui->btn_Delete,&QPushButton::clicked,this,&MyFile_Page::On_Delete_Clicked);

    connect(ui->btn_download,&QPushButton::clicked,this,&MyFile_Page::On_DownLoad_Clicked);

    ui->file_list->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->file_list,&QListWidget::customContextMenuRequested,this,[=]{
        QMenu *right_menu = new QMenu(this);
        QAction *Download = new QAction(tr("Download This File"),this);
        QAction *Delete = new QAction(tr("Delete This File"),this);
        QAction *Shared = new QAction(tr("Shared This File"),this);
        QAction *Cancel_Shared = new QAction(tr("Cancel Shared This File"),this);

        right_menu->addAction(Download);
        right_menu->addSeparator();

        right_menu->addAction(Delete);
        right_menu->addSeparator();

        right_menu->addAction(Shared);
        right_menu->addSeparator();

        right_menu->addAction(Cancel_Shared);


        connect(Delete,&QAction::triggered,this,&MyFile_Page::On_Delete_Clicked);
        connect(Download,&QAction::triggered,this,&MyFile_Page::On_DownLoad_Clicked);
        connect(Shared,&QAction::triggered,this,&MyFile_Page::On_Shared_Clicked);
        connect(Cancel_Shared,&QAction::triggered,this,&MyFile_Page::On_Cancel_Shared_File_Clicked);

        right_menu->exec(QCursor::pos());
        delete Download;
        delete Delete;
        delete Shared;
        delete right_menu;
    });


}

MyFile_Page::~MyFile_Page()
{
    delete ui;
}

void MyFile_Page::Md5_Check(QString file_path)
{
    QFile *file = new QFile(file_path);//打开文件
    file->open(QIODevice::ReadOnly);
    QByteArray bytes = file->readAll();//读取文件内容
        //file->close();

    auto Get_Hash = QCryptographicHash::hash(bytes,QCryptographicHash::Md5);//获取文件MD5

    QString File_Md5 = Get_Hash.toHex();

    QNetworkAccessManager *Manager = new QNetworkAccessManager(this);
    QJsonObject be_send_data;
    QFileInfo file_info(file_path);

    Get_User_Name *User_Name = Get_User_Name::Instance();
    be_send_data.insert("file_md5",File_Md5);
    be_send_data.insert("user_name",User_Name->Name);
    be_send_data.insert("file_name",file_info.fileName());

    QNetworkRequest request;
    request.setUrl(QUrl(Server_URL::MD5_CHECK_URL));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    auto be_post_data = QJsonDocument(be_send_data).toJson(QJsonDocument::Compact);

    QNetworkReply *reply = Manager->post(request,be_post_data);

    connect(reply,&QNetworkReply::finished,[=]{
        auto get_server_reply = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(get_server_reply);
        QJsonObject obj = doc.object();
        auto Get_Reply = obj.value("allow_md5_save").toString();

        if(Get_Reply == "Yes")
        {
            ui->progressBar->setMaximum(100);
            ui->progressBar->setValue(100);

            emit Flush_List_Item();
            QMessageBox::information(this,"成功","秒传成功");
        }
        else
        {
            emit Md5_Save_Failed(file_path,File_Md5);
        }
    });
}

void MyFile_Page::SendFile()
{
    ui->progressBar->setValue(0);//重置进度条
    ui->progressBar->setMaximum(100);
    ui->label_file_name_pro->setText("null");//设置文本

    QString file_path = QFileDialog::getOpenFileName();//打开文件
    if(file_path == nullptr)//没有获取到文件路径则退出
    {
        return;
    }
    QFileInfo file_info(file_path);//获取File_info

    if(file_info.size()>1024*1024*700)//限制文件大小(看情况后期要不要做分段传输)
    {
         QMessageBox::warning(this,"警告","文件不能超过700MB大小");
         return;
    }

    ui->label_file_name_pro->setText(file_info.fileName()+":");//设置Label文本为当前打开文件

    this->Md5_Check(file_path);
}

void MyFile_Page::Get_User_All_Flie()
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
    QString shared_statue;
    QString file_size;
    QString File_Md5;

    QMimeDatabase Type_db;
    //
        db.setHostName("192.168.118.134");
        db.setPort(3306);
        db.setDatabaseName("WebServer");
        db.setUserName("root");
        db.setPassword("chh20020729");

        bool ok = db.open();
        if (ok){
            Get_User_Name *User = Get_User_Name::Instance();
            QString select = QString("SELECT * FROM user_file_list WHERE user='%1'").arg(User->Name);
            QSqlQuery query;
            query.exec(select);


            while(query.next())
            {
                List_Item * item = new List_Item();
                file_name = query.value(3).toString();
                create_time = query.value(2).toString();
                shared_statue = query.value(4).toString();
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
                item->Set_File_Shared_Statue(shared_statue);
                item->Set_File_Size(file_size);

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

void MyFile_Page::On_Delete_Clicked()
{

    QList<QListWidgetItem*> Selet_Ttem =  ui->file_list->selectedItems();
    if(Selet_Ttem.isEmpty())
    {
        QMessageBox::information(this,"提示","请选中需要删除的文件");
        return;
    }
    QMessageBox info_dilog;
    info_dilog.setText("警告");
    info_dilog.setWindowTitle("Warning");
    info_dilog.setInformativeText("文件一旦删除便无法恢复,确认删除吗?");
    info_dilog.setStandardButtons(QMessageBox::Yes|QMessageBox::Cancel);
    info_dilog.setDefaultButton(QMessageBox::Cancel);

    info_dilog.setIcon(QMessageBox::Warning);

    int ret = info_dilog.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        return;
        break;
    }

    QWidget *My_Widget = ui->file_list->itemWidget(Selet_Ttem[0]);
    QList<QLabel*> File_Name_Label = My_Widget->findChildren<QLabel*>();

    QNetworkRequest request;
    request.setUrl(QUrl(Server_URL::DELETE_FILE_URL));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkAccessManager *manager=new QNetworkAccessManager(this);

    QJsonObject be_send_data;
    Get_User_Name *User_Name = Get_User_Name::Instance();

    be_send_data.insert("user_name",User_Name->Name);
    be_send_data.insert("delete_file_name",File_Name_Label[0]->text());
    be_send_data.insert("delete_file_create_time",File_Name_Label[2]->text());

    auto be_post_data = QJsonDocument(be_send_data).toJson(QJsonDocument::Compact);

    QNetworkReply *reply = manager->post(request,be_post_data);

    connect(reply,&QNetworkReply::finished,[=]{

        auto get_server_reply = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(get_server_reply);
        QJsonObject obj = doc.object();
        auto Get_Reply = obj.value("Delete_Statue").toString();

        if(Get_Reply == "Success")
        {
            emit Flush_List_Item();
            QMessageBox::information(this,"成功","删除成功");
        }
        else
        {
            QMessageBox::warning(this,"失败","删除失败");
        }
    });

}

void MyFile_Page::On_DownLoad_Clicked()
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

    Get_User_Name *User_Name = Get_User_Name::Instance();

    QWidget *My_Widget = ui->file_list->itemWidget(Selet_Ttem[0]);
    QList<QLabel*> List_Item_Label = My_Widget->findChildren<QLabel*>();
    ui->label_file_name_pro->setText(List_Item_Label[0]->text());
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    QUrl url(Server_URL::DOWNLOAD_URL);
    QUrlQuery query;

    query.addQueryItem("user_name",User_Name->Name);
    query.addQueryItem("download_file_name",List_Item_Label[0]->text().toUtf8());
    query.addQueryItem("shared_statue","0");

    url.setQuery(query.query());

    request.setUrl(url);

    QNetworkReply *reply = manager->get(request);

    connect(reply,&QNetworkReply::finished,this,[=]{

        auto get_server_message = reply->readAll();
        //qDebug()<<"上传成功"<<get_server_message.toLatin1();
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

void MyFile_Page::On_Shared_Clicked()
{
    QMessageBox info_dilog;
    info_dilog.setText("info");
    info_dilog.setWindowTitle("提示");
    info_dilog.setInformativeText("是否要共享该文件,文件共享后所有用户都能下载该文件?");
    info_dilog.setStandardButtons(QMessageBox::Cancel|QMessageBox::Yes);
    info_dilog.setDefaultButton(QMessageBox::Yes);

    info_dilog.setIcon(QMessageBox::Information);

    int ret = info_dilog.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        return;
        break;
    }
    QList<QListWidgetItem*> Selet_Ttem =  ui->file_list->selectedItems();
    QWidget *My_Widget = ui->file_list->itemWidget(Selet_Ttem[0]);
    QList<QLabel*> List_Item_Label = My_Widget->findChildren<QLabel*>();

    Get_User_Name *User = Get_User_Name::Instance();
    QJsonObject Get_Json_Data;

    Get_Json_Data.insert("user_name",User->Name);
    Get_Json_Data.insert("create_time",List_Item_Label[2]->text());
    Get_Json_Data.insert("file_name",List_Item_Label[0]->text());

    auto be_send_data = QJsonDocument(Get_Json_Data).toJson(QJsonDocument::Compact);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(Server_URL::SHARED_URL);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QNetworkReply *reply = manager->post(request,be_send_data);

    connect(reply,&QNetworkReply::finished,this,[=]{
        auto get_server_message = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(get_server_message);
        QJsonObject obj = doc.object();
        auto Get_Reply = obj.value("Shared_Statue").toString();

        if(Get_Reply != "Ok")
        {
            if(Get_Reply==nullptr)
            {
                QMessageBox::warning(this,"warning","共享失败");
            }
            QMessageBox::warning(this,"warning",Get_Reply);
            return;
        }
        else
        {
            this->Get_User_All_Flie();//刷新界面
            QMessageBox::information(this,"infomation","共享成功!");
        }

    });

}

void MyFile_Page::On_Cancel_Shared_File_Clicked()
{
    QList<QListWidgetItem*> Selet_Ttem =  ui->file_list->selectedItems();

    QWidget *My_Widget = ui->file_list->itemWidget(Selet_Ttem[0]);
    QList<QLabel*> List_Item_Label = My_Widget->findChildren<QLabel*>();

    if(Selet_Ttem.isEmpty())
    {
        QMessageBox::information(this,"提示","请选中需要取消分享的文件");
        return;
    }
    else if(List_Item_Label[4]->text() == "No")
    {
        QMessageBox::information(this,"提示","该文件不是一个共享的文件");
        return;
    }
    QMessageBox info_dilog;
    info_dilog.setText("警告");
    info_dilog.setWindowTitle("Warning");
    info_dilog.setInformativeText("取消后其他用户将无法看到这个文件,确认要取消文件的分享吗?");
    info_dilog.setStandardButtons(QMessageBox::Yes|QMessageBox::Cancel);
    info_dilog.setDefaultButton(QMessageBox::Cancel);

    info_dilog.setIcon(QMessageBox::Warning);

    int ret = info_dilog.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        return;
        break;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;

    request.setUrl(QUrl(Server_URL::CANCEL_SHARED_URL));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QJsonObject be_send_json;
    Get_User_Name *User = Get_User_Name::Instance();

    be_send_json.insert("user_name",User->Name);
    be_send_json.insert("file_create_time",List_Item_Label[2]->text());
    be_send_json.insert("file_name",List_Item_Label[0]->text());

    auto post_data = QJsonDocument(be_send_json).toJson(QJsonDocument::Compact);

    QNetworkReply *reply = manager->post(request,post_data);

    connect(reply,&QNetworkReply::finished,this,[=]{

        auto get_server_message = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(get_server_message);
        QJsonObject obj = doc.object();
        auto Get_Reply = obj.value("Cancel_Shared").toString();

        if(Get_Reply != "Success")
        {
            if(Get_Reply==nullptr)
            {
                QMessageBox::warning(this,"warning","取消共享失败");
            }
            QMessageBox::warning(this,"warning","取消共享失败");
            return;
        }
        else
        {
            this->Get_User_All_Flie();//刷新界面
            QMessageBox::information(this,"infomation","取消共享成功!");
        }
    });
}
