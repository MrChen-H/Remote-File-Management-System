#include "list_item.h"
#include "ui_list_item.h"

List_Item::List_Item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::List_Item)
{
    ui->setupUi(this);
}

List_Item::~List_Item()
{
    delete ui;
}

void List_Item::Set_File_Name(const QString file_name)
{
    ui->label_file_name->setText(file_name);
}

void List_Item::Set_Create_Time(const QString create_time)
{
    ui->label_file_create_time->setText(create_time);
}

void List_Item::Set_File_Type(const QString file_type)
{
    ui->label_file_type->setText(file_type);
}

void List_Item::Set_File_Shared_Statue(const QString shared_statue)
{
    if(shared_statue == "0")
    {
        ui->label_shared_statue->setText("No");
    }
    else if(shared_statue == "1")
    {
        ui->label_shared_statue->setText("Yes");
    }

}

void List_Item::Set_File_Size(const QString File_Size)
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
