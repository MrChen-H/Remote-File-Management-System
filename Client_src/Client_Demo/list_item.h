#ifndef LIST_ITEM_H
#define LIST_ITEM_H

#include <QWidget>

namespace Ui {
class List_Item;
}

class List_Item : public QWidget
{
    Q_OBJECT

public:
    explicit List_Item(QWidget *parent = nullptr);
    ~List_Item();
    void Set_File_Name(const QString file_name);
    void Set_Create_Time(const QString create_time);
    void Set_File_Type(const QString file_type);
    void Set_File_Shared_Statue(const QString shared_statue);
    void Set_File_Size(const QString File_Size);

private:
    Ui::List_Item *ui;
};

#endif // LIST_ITEM_H
