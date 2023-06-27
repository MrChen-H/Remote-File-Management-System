#ifndef ITEM_OF_SHARE_H
#define ITEM_OF_SHARE_H

#include <QWidget>

namespace Ui {
class item_of_share;
}

class item_of_share : public QWidget
{
    Q_OBJECT

public:
    explicit item_of_share(QWidget *parent = nullptr);
    ~item_of_share();

    void Set_File_Name(const QString file_name);
    void Set_Create_Time(const QString create_time);
    void Set_File_Type(const QString file_type);
    void Set_File_Shared_User(const QString Shared_User);
    void Set_File_Size(const QString File_Size);

private:
    Ui::item_of_share *ui;
};

#endif // ITEM_OF_SHARE_H
