#include "dialog.h"
#include "ui_dialog.h"
#include "QPushButton"
#include "register.h"
#include "QSqlDatabase"
#include "QMessageBox"
#include "QSqlQuery"
#include "qdebug.h"
#include "QSqlError"
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonValue"
#include "QFile"
#include "QNetworkAccessManager"
#include "QNetworkRequest"
#include "QNetworkReply"
#include "User_Name.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    Register *reg = new Register;//初始化注册窗口
    connect(ui->btn_reg,&QPushButton::clicked,this,[=]{//点击注册按钮弹出注册窗口
        reg->setParent(this);
        reg->setWindowFlags(Qt::Window);
        reg->setWindowModality(Qt::WindowModal);//设置窗口为模态
        reg->setWindowTitle("注册");
        reg->show();
    });

    connect(reg,&Register::Register_Ok,this,[=]{//当注册窗口发出注册成功信号退出注册窗口
        QMessageBox::information(this,"注册成功","账号注册成功");
        reg->close();
    });

    connect(this,&Dialog::User_Log_Out,[=]{
        ui->lineEdit_password->clear();
    });


}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btn_login_clicked()//登录点击事件
{
    if(ui->lineEdit_password->text().size()==0 || ui->lineEdit_user_name->text().size()==0)
    {
         QMessageBox::information(this,"密码或用户名错误","请输入用户名密码");
         return;
    }
    QJsonObject json;
    QJsonObject data;//将用户名和密码格式化成json格式
    data.insert("user_name",QJsonValue(ui->lineEdit_user_name->text()));
    data.insert("password",QJsonValue(ui->lineEdit_password->text()));
    json.insert("user",data);

     QNetworkAccessManager *manger = new QNetworkAccessManager(this);
     QNetworkRequest res;
     res.setUrl(QUrl(Server_URL::LOGIN_URL));//设置url
     res.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));//指定数据发送格式为Json

     auto post_data = QJsonDocument(json).toJson(QJsonDocument::Compact);

     QNetworkReply *Req =  manger->post(res,post_data);//接受服务器返回数据

     connect(Req,&QNetworkReply::readyRead,this,[=]{//服务器回传数据可读时
         auto data = Req->readAll();//读出所有数据
         QJsonDocument doc = QJsonDocument::fromJson(data);//转换成json格式方便解析
         QJsonObject obj = doc.object();
         auto Ok_No = obj.value("info").toString();//获取回传的信息
         if(Ok_No!="Ok")
         {
            QMessageBox::information(this,"密码或用户名错误",Ok_No);
            Req->deleteLater();
            return;
         }
         else
         {
            emit Login_OK(ui->lineEdit_user_name->text());//回传信息为ok时发送登录成功信号
            Req->deleteLater();
            return;
         }
     });

}
