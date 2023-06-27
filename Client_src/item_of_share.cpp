#include "item_of_share.h"
#include "ui_item_of_share.h"

item_of_share::item_of_share(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::item_of_share)
{
    ui->setupUi(this);
}

item_of_share::~item_of_share()
{
    delete ui;
}

void item_of_share::Set_File_Name(const QString file_name)
{
    ui->label_file_name->setText(file_name);
}

void item_of_share::Set_Create_Time(const QString create_time)
{
    ui->label_file_create_time->setText(create_time);
}

void item_of_share::Set_File_Type(const QString file_type)
{
    ui->label_file_type->setText(file_type);
}

void item_of_share::Set_File_Shared_User(const QString shared_user)
{

    ui->label_shared_user->setText(shared_user);


}

void item_of_share::Set_File_Size(const QString File_Size)
{
    long file_size = File_Size.toLong();

    if(file_size<1024)
    {
       ui->label_size->setText(QString("%1 B").arg(file_size));
       return;
    }
    if(file_size<1024*1024)
    {
        ui->label_size->setText(QString("%1 KB").arg(QString::number(((double)file_size/1024),'f',1)));
        return;
    }
    if(file_size<1024*1024*1024)
    {
        ui->label_size->setText(QString("%1 MB").arg(QString::number(((double)file_size/1024/1024),'f',1)));
        return;
    }
    if(file_size<1024*1024*1024*1024)
    {
        ui->label_size->setText(QString("%1 GB").arg(QString::number(((double)file_size/1024/1024/1024),'f',1)));
        return;
    }
}
