#ifndef MYFILE_PAGE_H
#define MYFILE_PAGE_H

#include <QWidget>
#include <QFileInfo>

namespace Ui {
class MyFile_Page;
}

class MyFile_Page : public QWidget
{
    Q_OBJECT

public:
    explicit MyFile_Page(QWidget *parent = nullptr);
    ~MyFile_Page();
    void Md5_Check(QString file_path);
    void SendFile();
    void Get_User_All_Flie();
    void On_Delete_Clicked();
    void On_DownLoad_Clicked();
    void On_Shared_Clicked();
    void On_Cancel_Shared_File_Clicked();
signals:
    void File_Info_Send(QFileInfo *be_tramit_file);
    void Md5_Save_Success();
    void Md5_Save_Failed(QString File_Path,QString File_Md5);
    void Flush_List_Item();
private:
    Ui::MyFile_Page *ui;
};

#endif // MYFILE_PAGE_H
