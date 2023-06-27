#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "QFileDialog"
#include "QMessageBox"
#include "User_Name.h"
#include "myfile_page.h"
#include "user_info.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("远程文件管理器");

    Dialog *login_win = new Dialog;
    login_win->show();
    ui->stackedWidget->setCurrentIndex(0);

    User_Info *user_info = new User_Info();

    connect(login_win,&Dialog::Login_OK,this,[=](QString Get_Name){
        QMessageBox::information(this,"登录成功","登录成功!");
        Get_User_Name *User_Name = Get_User_Name::Instance();
        User_Name->Name = std::move(Get_Name);
        emit ui->my_file_page->Flush_List_Item();
        emit login_win->User_Log_Out();
        login_win->hide();

        this->show();
    });

    connect(ui->btn_MyFile,&QPushButton::clicked,this,[=]{
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->btn_shared_list,&QPushButton::clicked,this,[=]{
        ui->shared_file_page->Flush_Page();
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(ui->btn_user_info,&QPushButton::clicked,this,[=]{
        emit user_info->Get_Info();
        user_info->show();
    });

    connect(user_info,&User_Info::Hide_Main_Window,[=]{
        user_info->hide();
        this->hide();
        login_win->show();
    });

}


MainWindow::~MainWindow()
{
    delete ui;
}

