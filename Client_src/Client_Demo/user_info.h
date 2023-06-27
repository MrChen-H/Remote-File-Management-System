#ifndef USER_INFO_H
#define USER_INFO_H

#include <QWidget>

namespace Ui {
class User_Info;
}

class User_Info : public QWidget
{
    Q_OBJECT

public:
    explicit User_Info(QWidget *parent = nullptr);
    ~User_Info();
    void Get_User_info();
    void Log_Out();

signals:
    void Hide_Main_Window();
    void Get_Info();

private:
    Ui::User_Info *ui;
};

#endif // USER_INFO_H
