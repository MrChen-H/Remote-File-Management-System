#include "register.h"
#include "ui_register.h"
#include "QRegularExpression"
#include "QRegularExpressionMatch"
#include "QMessageBox"
#include "Cheak_Format.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonValue"
#include "QNetworkAccessManager"
#include "QNetworkRequest"
#include "QNetworkReply"
#include "QJsonDocument"
#include "User_Name.h"
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    connect(ui->btn_register,&QPushButton::clicked,this,[=]{
        if(ui->lineEdit_email->text()==nullptr||ui->lineEdit_password->text()==nullptr||ui->lineEdit_name->text()==nullptr||ui->lineEdit_password_2->text()==nullptr||ui->lineEdit_telephone->text()==nullptr)
        {
            QMessageBox::information(this,"info","请完善信息!");
            return;
        }
        if(Check_format(ui->lineEdit_name->text(),"[^A-Za-z0-9_]+",20) == false||ui->lineEdit_name->text().size()<3)
        {
            QMessageBox::warning(this,"格式错误","用户名必须大于3字符小于20字符且只能由数字,字母,下划线组成!");
            return;
        }
        if(Check_format(ui->lineEdit_password->text(),"[\\s*\\\\]+",16) == false||ui->lineEdit_password->text().size()<6)
        {
            QMessageBox::warning(this,"格式错误","密码必须大于6位小于16位且不能包含'\\'与空格!");
            return;
        }
        if(ui->lineEdit_password->text() != ui->lineEdit_password_2->text())
        {
            QMessageBox::warning(this,"密码错误","两次输入的密码不一致!");
            return;
        }
        if(Check_telepohon(ui->lineEdit_telephone->text())==false)
        {
            QMessageBox::warning(this,"格式错误","请输入正确的电话!");
            return;
        }
        if(Check_Email(ui->lineEdit_email->text())==false)
        {
            QMessageBox::warning(this,"格式错误","请输入正确的邮箱!");
            return;
        }
        QJsonObject be_send_data;
        be_send_data.insert("user_name",ui->lineEdit_name->text());
        be_send_data.insert("nick_name",ui->lineEdit_nick_name->text());
        be_send_data.insert("password",ui->lineEdit_password->text());
        be_send_data.insert("telephone_number",ui->lineEdit_telephone->text());
        be_send_data.insert("email",ui->lineEdit_email->text());

        QNetworkAccessManager *server_connect = new QNetworkAccessManager(this);
        QNetworkRequest res;
        res.setUrl(QUrl(Server_URL::REGISTER_URL));
        res.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

        auto post_data = QJsonDocument(be_send_data).toJson(QJsonDocument::Compact);

        QNetworkReply *Req =  server_connect->post(res,post_data);
        //qDebug()<<be_send_data;

        connect(Req,&QNetworkReply::readyRead,this,[=]{
            auto data = Req->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject obj = doc.object();
            auto Ok_No = obj.value("info").toString();
            if(Ok_No=="Ok")
            {
               emit Register_Ok();
               Req->deleteLater();
               return;
            }
            else
            {
                QMessageBox::information(this,"注册失败",Ok_No);
                Req->deleteLater();
                return;
            }
        });

    });

}

Register::~Register()
{
    delete ui;
}
