#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

signals:
    void Login_OK(QString user_Name);
    void User_Log_Out();

private slots:
    void on_btn_login_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
