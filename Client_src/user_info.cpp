#include "user_info.h"
#include "ui_user_info.h"
#include "User_Name.h"
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QMessageBox"

User_Info::User_Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::User_Info)
{
    ui->setupUi(this);
    Get_User_info();
    connect(this,&User_Info::Get_Info,this,&User_Info::Get_User_info);
    connect(ui->btn_logout,&QPushButton::clicked,this,&User_Info::Log_Out);
}

User_Info::~User_Info()
{
    delete ui;
}

void User_Info::Get_User_info()
{
    Get_User_Name *User = Get_User_Name::Instance();

    QSqlDatabase db;

    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
    }

    db.setHostName("192.168.118.134");
    db.setPort(3306);
    db.setDatabaseName("WebServer");
    db.setUserName("root");
    db.setPassword("chh20020729");

    db.open();

    QSqlQuery query;

    QString Select_Command = QString("SELECT nickname,phone,createtime,email FROM user WHERE name='%1'").arg(User->Name);

    query.exec(Select_Command);

    while(query.next())
    {
        QString nick_name= query.value(0).toString().toUtf8();
        QString phone= query.value(1).toString();
        QString create_time= query.value(2).toString();
        QString email= query.value(3).toString();

        ui->label_value_user_name->setText(User->Name);
        ui->label_value_nic_name->setText(nick_name);
        ui->label_value_telephone->setText(phone);
        ui->label_value_create_time->setText(create_time);
        ui->label_value_email->setText(email);
    }

    if(db.isOpen())
    {
        db.close();
    }
}

void User_Info::Log_Out()
{
    QMessageBox info_dilog;
    info_dilog.setText("警告");
    info_dilog.setWindowTitle("Warning");
    info_dilog.setInformativeText("确定要登出账户吗?");
    info_dilog.setStandardButtons(QMessageBox::Yes|QMessageBox::Cancel);
    info_dilog.setDefaultButton(QMessageBox::Cancel);

    info_dilog.setIcon(QMessageBox::Warning);

    int ret = info_dilog.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        return;
        break;
    }

    Get_User_Name *User = Get_User_Name::Instance();
    User->Name.clear();//清空用户名
    emit Hide_Main_Window();//隐藏主窗口
}
